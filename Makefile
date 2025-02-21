all: bp

CPLUS = g++

.cc.o: 
	$(CPLUS) -O2 -c $(CFLAGS) $*.cc

# backprop
bp: bp.o 
	$(CPLUS) $(CFLAGS)  -o $@ bp.o $(LIBS)

# profiler
prof: 
	$(CPLUS) -pg -o $@ bp.cc $(LIBS)

check:
	gprof -p -b prof gmon.out 

# clean
clean:
	@echo 'rm -f *.o'; rm -f *.o bp core

