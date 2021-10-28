//----------------------------------------------------------------------------
// 
//	file: matrix.h
// 
//	function: matrix template class 
//		Only works for scalars or structs that do not
//		need constructors.
//
//	author:   Yoonsuck Choe <yschoe@cs.utexas.edu>
//	creation: Sat Oct 31 00:54:48 CST 1998
// 
//	$Revision: 1.3 $ $Date: 2000/02/22 03:54:53 $
//	$Log: matrix.h,v $
//	Revision 1.3  2000/02/22 03:54:53  yschoe
//	added
//	1. print matrix to a file (given a file pointer)
//	2. load from ascii file
//
//	Revision 1.2  2000/02/08 11:25:21  yschoe
//	*** empty log message ***
//
//	Revision 1.1  1999/02/09 07:42:25  yschoe
//	Initial revision
//
//
//----------------------------------------------------------------------------
#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "real.h"

using namespace std;

template <class T>
class matrix {

  int cols, rows;

public:

  T** mat;

  matrix(int c, int r) { mat = alloc_local_matrix(c,r); };
  ~matrix() { delete_matrix(); };

  void reset_matrix() {

  	// this may not work for non-scaler types
	for (int i=0; i<cols; ++i) {
	   for (int j=0; j<rows; ++j) {
	      mat[i][j] = (T)0;
	   }
        }

  }

  void reset () { reset_matrix(); }

  int get_cols (void) { return cols; }
  int get_rows (void) { return rows; }

  void print_matrix() {

  	// the template class must have "<<" defined

	// print column vectors as row vectors
	if (cols==1) {
		for (int j=0; j<rows; ++j) {
	      		cout << mat[0][j] << " ";
	   	}
	   	cout << endl;
        } else {
	// print out matrices as usual
		for (int j=0; j<rows; ++j) {
		   for (int i=0; i<cols; ++i) {
		      cout << mat[i][j] << " ";
		   }
		   cout << endl;
		}
	}
  }

  void print() { print_matrix(); }

  void print_matrix(FILE *fp) {

  	// the template class must have "<<" defined

	// print column vectors as row vectors
	if (cols==1) {
		for (int j=0; j<rows; ++j) {
	      		cout << mat[0][j] << " ";
	   	}
	   	cout << endl;
        } else {
	// print out matrices as usual
		for (int j=0; j<rows; ++j) {
		   for (int i=0; i<cols; ++i) {
	      		fprintf(fp,"%f ",(float)mat[i][j]);
		   }
		   cout << endl;
		}
	}
  }

  void print(FILE *fp) { print_matrix(fp); }

  REAL    sum() {

	REAL sum=0;
	for (int j=0; j<rows; ++j) {
	   for (int i=0; i<cols; ++i) {
	     	sum += mat[i][j];	 
	   }
        }
	return sum;
  }

  static REAL    sum(T **mat, int cols, int rows) {

	REAL sum=0;
	for (int j=0; j<rows; ++j) {
	   for (int i=0; i<cols; ++i) {
	     	sum += mat[i][j];	 
	   }
        }
	return sum;
  }
	
	
  REAL    avg() {

	REAL sum=0;
	for (int j=0; j<rows; ++j) {
	   for (int i=0; i<cols; ++i) {
	     	sum += mat[i][j];	 
	   }
        }
	return (sum/(REAL)(cols*rows));
  }

  void  minmax(REAL* min, REAL* max) {

        *min = 99999;
        *max = -99999;

        for (int i=0; i<cols; ++i) {
           for (int j=0; j<rows; ++j) {
              *min = (*min>mat[i][j])?mat[i][j]:*min;
              *max = (*max<mat[i][j])?mat[i][j]:*max;
           }
        }
  }

  void scale(T maxrange=1) {

	REAL min;
	REAL max;

	minmax(&min, &max);
        for (int i=0; i<cols; ++i)
           for (int j=0; j<rows; ++j) 
		mat[i][j] = (T)(maxrange * (mat[i][j]-min)/(max-min));
	    
  }

  void print_cartesian_matrix() {
 
	// n
	// |
 	// |	  print in this format
	// 0 -- m  
  	// the class must have "<<" defined
	for (int j=rows-1; j>=0; --j) {
	   for (int i=0; i<cols; ++i) {
	      cout << mat[i][j] << " ";
	   }
	   cout << endl;
        }

  }

  void print_cartesian () { print_cartesian_matrix(); }

  static void print_matrix(T **mat, int cols, int rows=1) {

  	// the class must have "<<" defined
	for (int j=0; j<rows; ++j) {
	   for (int i=0; i<cols; ++i) {
	      cout << mat[i][j] << " ";
	   }
	   cout << endl;
        }

  }

  static void print(T **mat, int cols, int rows=1) {print_matrix(mat,cols,rows);}

  static void scaleup(T **mat, int cols, int rows, T scaleup) {

	for (int j=0; j<rows; ++j) {
	   for (int i=0; i<cols; ++i) {
	      mat[i][j] *= scaleup;
	   }
        }

  }

  void random_matrix() {

  	// this may not work for non-scaler types
	for (int i=0; i<cols; ++i) {
	   for (int j=0; j<rows; ++j) {
	      mat[i][j] = (T)drand48();
	   }
        }

  }

  void random() { random_matrix(); }

  void export_matrix(T** destination) {

	for (int i=0; i<cols; ++i) {
	   for (int j=0; j<rows; ++j) {
	      destination[i][j] = mat[i][j];
	   }
	}
  } 	

  void import_matrix(T** source) {

	for (int i=0; i<cols; ++i) {
	   for (int j=0; j<rows; ++j) {
	      mat[i][j] = source[i][j];
	   }
	}
  } 	

  void import(T** source) { import_matrix(source); }

  void import_matrix(matrix<T> *source, int x, int y) {

	// import matrix and put it at (x,y)
	// cout << "inside" << endl;
	int end_col = MIN(x+source->cols-1,cols-1);
	int end_row = MIN(y+source->rows-1,rows-1);

	// cout << x << "," << y << " " << end_col << "," << end_row << endl;

	for (int i=x; i<=end_col; ++i) {
	   for (int j=y; j<=end_row; ++j) {
	      mat[i][j] += source->mat[i-x][j-y];
	   }
	}
  } 	

  void import(matrix<T> *source, int x, int y){import_matrix(source,x,y);}

  // calculate dot product like a vector :
  //   return value = A dot B = Sum(c_ij), where c_ij = a_ij * b_ij;
  T vector_dot_product(T** B) {

	T c=0;

	for (int i=0; i<cols; ++i) {
	   for (int j=0; j<rows; ++j) {
	      c += mat[i][j]*B[i][j];
	   }
	}

	return c;
  } 	

  // calculate dot product like a vector :
  //   return value = A dot B = Sum(c_ij), where c_ij = a_ij * b_ij;
  T vector_dot_product(T **A, T** B) {

	T c=0;

	for (int i=0; i<cols; ++i) {
	   for (int j=0; j<rows; ++j) {
	      c += A[i][j]*B[i][j];
	   }
	}

	return c;
  } 	

  void load_real_matrix(char *filename) {
	
	FILE *fp;
	REAL data;
	
	fp = fopen(filename,"r");
	if (!fp) {
		cerr << "matrix::load_matrix: error loading " << filename << endl;
	}

	int j = 0;
	while (!feof(fp)) {
		for (int i=0; i<cols; ++i) {
			fscanf(fp,"%f",&data);
			mat[i][j] = (T)data;
		}
		j ++;	
	}
	fclose(fp);

  }


private: 

  T** alloc_local_matrix(int c, int r)  {
	
	T** temp;

	cols = c;
	rows = r;

	temp = new T* [cols];
	for (int i=0; i<cols; ++i) {
		temp[i] = new T [rows];
	}
	return temp;
  }

  void delete_matrix()  {
	
	for (int i=0; i<cols; ++i) {
		delete [] mat[i];
	}
	delete mat;
  }
};

// subclass vector : this can only be a row vector, to exploit the
// row-major allocation used in the matrix class

template <class T>
class vector : public matrix<T> {

public:

  T *vec;

  vector(int r):matrix<T>(1,r) { vec=this->mat[0]; };
  ~vector() { this->delete_matrix(); };

};


#endif

