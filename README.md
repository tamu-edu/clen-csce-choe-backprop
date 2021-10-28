----------------------------------------------------------------------------
Date: 		Fri Nov  9 13:31:23 CST 2001
Author: 	Yoonsuck Choe <choe@tamu.edu>
URL:		http://www.cs.tamu.edu/faculty/choe/src/backprop-1.6.tar.gz
Version: 	1.6
Copyright: 	GNU Public License (all source and this README file).
----------------------------------------------------------------------------

		Backpropagation with Bias Unit

			    Contents

		I.   Source file description
		II.  How to build
		III. How to run
		IV.  Tips
		V.   Suggested extensions
		VI.  References

----------------------------------------
I. Source file description:
----------------------------------------

1. Makefile		: Makefile

2. bp.cc		: main backprop program

3. conf/		: directory containing sample configuration files

4. matrix.h		: matrix class -- includes vector class

5. real.h		: some useful real number defs and macros.

----------------------------------------
II. How to build
----------------------------------------

1. To build, simply do

	make

   The result is an executable "bp".

   If possible, gcc and gmake are recommended. These are all available
   under various unix and windows platforms: www.gnu.org.

----------------------------------------
III. How to run 
----------------------------------------

1. There are four example configuration files 

	and.conf
	equiv.conf 
	or.conf
	xor.conf

   in the conf/ directory.

2. Run the "bp" command as follows:

	./bp conf/and.conf

   i.e., bp <configfile>.

3. The format of the configuration file is described at the beginning of the 
   bp.cc source file.

   To repeat:

		Comments: do not enter these in the config file
   2 4 1        number of input units, hidden units, output units
   4            number of training input-output patterns (we have 4 below)
   0.001 0.0 1  learning rate, momentum (not used), biasflag (0:use 1:don't)
   0 0 0        training 1 : input value1, input value2, output value 1
   0 1 1        training 2 : ..
   1 0 1	training 3 : ..
   1 1 0	training 4 : ..

   The output is a bit of a mess, so you shall "grep" for interesting stuff:

	./bp conf/and.conf > and.out

	grep ERR and.out			: see how error decreases
	grep OUT and.out			: see how the output evolves

	egrep "TARGET\|OUT" and.out		: compare target and output

   For example, to extract the current epoch and error,

	awk '{print $2,$3}' and.out > and.dat

   and plot and.dat with matlab or some other package.

   An example of the error plot for xor.conf is in conf/xor.png.
 
----------------------------------------
IV. Tips
----------------------------------------

1. Try playing with the learning rate; increase or decrease.
   Copy existing config files and modify the copy.

2. Try running with or without the bias unit.

3. Try changing the output representation. For example, instead of 1 and 0
   for truth and false, try 1 0, and 0 1, i.e. use two output units.

4. Try different number of hidden units. 

----------------------------------------
V. Suggested extensions
----------------------------------------

1. Saving of trained weights to a file and loading from weight file.

	- generate weight files from current state.
	- when starting the program, allow extra command line arguments
	  to specify weight file(s).

2. Testing trained network with novel input.

	- test how the network does with new inputs.
	- Actually, this is now implemented. You can append test data
	  below the training data. In the second line in the conf file,
	  indicate how many training samples you've included as the 2nd
	  number.

3. Implement the momentum term.

4. Make BETA (the sigmoid slope parameter) specifiable in the config file.
   It is #defined in bp.cc currently.

5. Make halting condition specifiable in the config file. Currently, 
   average squared error < 0.001 is the halting condition.

6. Minor: replace fscanf() with iostream functions.

7. Minor: give more structure (or better formatting) to the output.
   Allow command line options to allow/disallow certain kinds of output.

----------------------------------------
VI. References
----------------------------------------

John Hertz and Anders Krogh and Richard G. Palmer, "Introduction to the Theory of Neural Computation", Addison-Wesley, Reading, MA, 1991.

