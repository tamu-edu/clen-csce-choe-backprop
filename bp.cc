//----------------------------------------------------------------------------
// 
//	file: bp.c
// 
//	function: implements vanila backprop with bias unit
//	author:   Yoonsuck Choe <yschoe@cs.utexas.edu>
//	creation: Sat Nov  3 14:37:07 CST 2001
// 
//	$Revision: 1.8 $ $Date: 2017/02/16 19:15:30 $
//	$Log: bp.cc,v $
//	Revision 1.8  2017/02/16 19:15:30  yschoe
//	fixed warning about fscanf.
//
//	Revision 1.7  2017/02/16 19:11:59  yschoe
//	fixed bug near line 200 where biasFlag was not handled correctly.
//	added "TEST" text for test output dump.
//
//	Revision 1.6  2001/11/03 20:57:29  yschoe
//	added more comments.
//
//	Revision 1.5  2001/11/03 20:46:03  yschoe
//	cleaned up. 1d arrays made to be all vector<double>
//
//	Revision 1.1  1999/02/09 07:56:44  yschoe
//	Initial revision
//
// Sample input file
// -- cut here --
// 2 4 1	number of input units, hidden units, output units
// 4 2		number of training input-target, number of test input-target
// 0.001 0.0 1	learning rate, momentum (not used), biasflag (0:use 1:don't)
// 0 0 0	training 1 : input value1, input value2, output value 1
// 0 1 1	training 2 : ..
// 1 0 1
// 1 1 0
// -- cut here --
//
//	Tips:
//		- you can use any number of input, hidden, output units
//
//----------------------------------------------------------------------------
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "matrix.h"

using namespace std;

// BETA in the sigmoid activation functin: f(x) = tanh(BETA*x);
#define BETA	0.1

//--------------------------------------------------
// network size, # of training input
//--------------------------------------------------

int inpSize, hidSize, outSize;
int numInp;		// number of training inputs
int numTestInp;		// number of test inputs
int biasFlag;		// bias flag : 0 = no bias, 1 = with bias

double eta, mom; 	// learning rate eta. momentum mom (not used).

int fs;  		// fscanf default

//--------------------------------------------------
// activities (after activation function)
//--------------------------------------------------
vector<double> *input;
vector<double> *hidden;
vector<double> *output;
vector<double> *target;

//--------------------------------------------------
// net input
//--------------------------------------------------
vector<double> *netHidden;
vector<double> *netOutput;

//--------------------------------------------------
// deltas
//--------------------------------------------------
vector<double> *deltaHidden;
vector<double> *deltaOutput;

//--------------------------------------------------
// weights : mat->mat[from][to]
//--------------------------------------------------
matrix<double> *input2hidden;
matrix<double> *hidden2output;
matrix<double> *inp2hidMom;
matrix<double> *hid2outMom;

//--------------------------------------------------
// input and target
//--------------------------------------------------
matrix<double> *dataIn;	// training
matrix<double> *dataOut;

matrix<double> *dataTestIn; // testing
matrix<double> *dataTestOut;

//----------------------------------------------------------------------------
// setupNetwork
//----------------------------------------------------------------------------
void setupNetwork (int inpSize, int hidSize, int outSize, int numInp) {

  cerr 	<< "inpSize = " << inpSize << " hidSize = " << hidSize 
  	<< " outSize = " << outSize  << endl;

  input 	= new vector<double>(inpSize); 
  hidden 	= new vector<double>(hidSize); 
  output 	= new vector<double>(outSize);
  target	= new vector<double>(outSize);

  netHidden 	= new vector<double>(hidSize);
  netOutput 	= new vector<double>(outSize);

  netHidden 	= new vector<double>(hidSize);
  netOutput 	= new vector<double>(outSize);

  deltaHidden 	= new vector<double>(hidSize);
  deltaOutput 	= new vector<double>(outSize);

  input2hidden 	= new matrix<double>(inpSize,hidSize);
  hidden2output = new matrix<double>(hidSize,outSize);

  inp2hidMom 	= new matrix<double>(inpSize,hidSize);
  hid2outMom = new matrix<double>(hidSize,outSize);

  input2hidden->random_matrix(); 
  hidden2output->random_matrix(); 

  dataIn	= new matrix<double>(numInp,inpSize);
  dataOut	= new matrix<double>(numInp,outSize);

  dataTestIn	= new matrix<double>(numTestInp,inpSize);
  dataTestOut	= new matrix<double>(numTestInp,outSize);

}
//----------------------------------------------------------------------------
// processConfig
//----------------------------------------------------------------------------
void processConfig (char *filename) {
 
  FILE *fp;

  biasFlag = 0; // default is with no bias.
  
  if (!(fp=fopen(filename,"r"))) {
	cerr << "processConfig(): file error " << filename << endl;
	exit(0);
  }

  fs=fscanf(fp,"%d %d %d\n",&inpSize, &hidSize, &outSize);
  fs=fscanf(fp,"%d %d\n",&numInp,&numTestInp);
  fs=fscanf(fp,"%lf %lf %d\n",&eta,&mom,&biasFlag);

  cout << "eta = " << eta << " biasFlag = " << biasFlag << endl;

  // setup network : if biasFlag == 1, turn on bias
  if (biasFlag) {
	inpSize +=1;
	hidSize +=1;
  }
  setupNetwork(inpSize,hidSize,outSize, numInp);

  for (int i=0; i<numInp; ++i) {
	// input
	for (int j=0; j<inpSize-biasFlag; ++j) {
		fs=fscanf(fp,"%lf",&(dataIn->mat[i][j]));
	}
	if (biasFlag) dataIn->mat[i][inpSize-biasFlag]=1.0;
	// output
	for (int j=0; j<outSize; ++j) {
		fs=fscanf(fp,"%lf",&(dataOut->mat[i][j]));
	}
	fs=fscanf(fp,"\n");
  }

  for (int i=0; i<numTestInp; ++i) {
	// Test input
	for (int j=0; j<inpSize-biasFlag; ++j) {
		fs=fscanf(fp,"%lf",&(dataTestIn->mat[i][j]));
	}
	if (biasFlag) dataTestIn->mat[i][inpSize-biasFlag]=1.0;
	// output
	for (int j=0; j<outSize; ++j) {
		fs=fscanf(fp,"%lf",&(dataTestOut->mat[i][j]));
	}
	fs=fscanf(fp,"\n");
  }

  fclose(fp);

}

//----------------------------------------------------------------------------
// actFunc
//----------------------------------------------------------------------------
double actFunc(double x) {
  // 1. sigmoid
  // return (1.0/(1.0+exp(-2.0*BETA*x)));

  // 2. tanh : this function works better than the sigmoid
  return (tanh(BETA*x));
}

//----------------------------------------------------------------------------
// actFuncDeriv : arg is f(x)
//----------------------------------------------------------------------------
double actFuncDeriv(double fx) {

  // 1. f'(netinput) for sigmoid
  // return 2.0*BETA*fx*(1.0-fx);

  // 2. f'(netinput) for tanh
  return (1-fx*fx);
}

//----------------------------------------------------------------------------
// feedForward
//----------------------------------------------------------------------------
void feedForward(vector <double> *inp, matrix<double> *wgt, vector <double> *netOut, vector <double> *out){
 
  int inpSize = wgt->get_cols();
  int outSize = wgt->get_rows();

  // set bias unit : the last input unit is the bias unit, and should
  // be set to 1.0 if the biasFlag is on.
  if (biasFlag) { 
	inp->vec[inpSize-1] = 1.0;
  }

  for (int t=0; t<outSize; ++t) {
	netOut->vec[t] = 0.0;
  	for (int i=0; i<inpSize; ++i) {
		netOut->vec[t] += inp->vec[i]*wgt->mat[i][t];
	}
	out->vec[t] = actFunc(netOut->vec[t]);
  }

} 

//----------------------------------------------------------------------------
// feedBack 
//----------------------------------------------------------------------------
void feedBack(vector <double> *inp, matrix<double> *wgt, vector <double> *actOut, vector <double> *out){
 
  int inpSize = wgt->get_rows();
  int outSize = wgt->get_cols();

  for (int t=0; t<outSize; ++t) {
	out->vec[t] = 0.0;
  	for (int i=0; i<inpSize; ++i) {
		out->vec[t] += inp->vec[i]*wgt->mat[t][i];
	}
	out->vec[t] *= actFuncDeriv(actOut->vec[t]);	
  }

} 

//----------------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------------
int main (int argc, char *argv[]) {

  if (argc!=2) {
	cerr << "usage: bp <configfile>" << endl;
 	exit(0);
  }

  processConfig(argv[1]); 

  cout << endl;
  cout << "dataIn" << endl ;
  dataIn->print_matrix();
  cout << endl;
  cout << "dataOut" << endl ;
  dataOut->print_matrix();

  int i=0;
  while (1) {

     double err = 0.0, errSum=0.0;
     //-------------------------------------------------- for
     for (int t=0; t<numInp; ++t) {
	// 1. Get training data
	for (int j=0; j<inpSize; ++j) input->vec[j] = dataIn->mat[t][j];
	for (int j=0; j<outSize; ++j) target->vec[j] = dataOut->mat[t][j];
	
	// 2. feedforward
	feedForward(input,input2hidden,netHidden,hidden);
	feedForward(hidden,hidden2output,netOutput,output);

	// 3. get error
	for (int j=0; j<outSize; ++j) {
		err = target->vec[j]-output->vec[j];	
		deltaOutput->vec[j] = actFuncDeriv(output->vec[j])*err;
		errSum += err*err;
	} 
   	// cout << endl;

	// 4. back prop delta
	feedBack(deltaOutput,hidden2output,hidden,deltaHidden);

	// 5. Adapt weight : online training
	for (int j=0; j<outSize; ++j) 
	   for (int k=0; k<hidSize; ++k) {
		hidden2output->mat[k][j] += eta * deltaOutput->vec[j] 
					  * hidden->vec[k];
	   }

	for (int j=0; j<hidSize; ++j) 
	   for (int k=0; k<inpSize; ++k) {
		input2hidden->mat[k][j] += eta * deltaHidden->vec[j]
					  * input->vec[k];
	   }

	// 6. Print status
        if (i%100==0) {
		input->print(); cout << " : out " ; 
		output->print(); cout << " : target " ; 
		target->print(); cout << " : delta " ; 
		deltaOutput->print(); cout << " : deltaHidden " ; 
		deltaHidden->print(); cout << endl; cout << endl;

		cout << " input2hidden : " << endl;
		input2hidden->print_matrix();
		cout << " hidden2output : " ;
		hidden2output->print_matrix();
		cout << endl;

		cout << " : hidden "; hidden->print();
		cout << endl;

		cout << "TARGET: "; target->print(); 
		cout << endl;
		cout << "\t\tOUT " << t <<" " ; output->print();
		cout << endl;
         }
    } // end of one epoch
    //-------------------------------------------------- end for

    ++i;
    if (i%10000==0) { // print average squared error
    	cout << "ERR " << (i/10000) << " " << (errSum/numInp) << endl;
    }
    if (errSum/numInp < 0.001) {
	cerr << "Target err reached." << endl;
	break;
    }

  }

  // Get output from test data
  double err=0, errSum = 0.0; 
  for (int k=0; k<numTestInp; ++k) {


	// 1. Get test data
	for (int j=0; j<inpSize; ++j) input->vec[j] = dataTestIn->mat[k][j];
	for (int j=0; j<outSize; ++j) target->vec[j] = dataTestOut->mat[k][j];
	
	// 2. feedforward
	feedForward(input,input2hidden,netHidden,hidden);
	feedForward(hidden,hidden2output,netOutput,output);

	// 3. get error
	for (int j=0; j<outSize; ++j) {
		err = target->vec[j]-output->vec[j];	
		errSum += err*err;
	} 
	cout << "TEST TARGET: "; target->print(); 
	cout << endl;
	cout << "\t\tTEST OUT " << k <<" " ; output->print();
	cout << endl;
   	cout << "TEST ERR " << k << " " << err << endl;
  }

  if (numTestInp!=0) {
  	cout << "Avg Test ERR = " << (errSum/numTestInp) << endl;
  }

}

