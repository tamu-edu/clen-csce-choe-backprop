all: bp

CPLUS = g++

.cc.o: 
	$(CPLUS) -O2 -c $(CFLAGS) $*.cc

# backprop
bp: bp.o 
	$(CPLUS) $(CFLAGS)  -o $@ bp.o $(LIBS)

# clean
clean:
	@echo 'rm -f *.o'; rm -f *.o bp core

