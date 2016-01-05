# Makefile

# Macros
CXX=g++
# CXXFLAGS=-O0 -g -Wall -Wno-deprecated -fPIC -Iinclude $(shell geant4-config --cflags) $(shell root-config --cflags)
CXXFLAGS=-O2 -Wall -Wno-deprecated -fPIC -Iinclude $(shell geant4-config --cflags) $(shell root-config --cflags)
LD=g++
LDFLAGS=-O

# program name
PROG=nerixsim_9

# object files
# OBJS=$(PROG).o src/NerixVectorDict.o $(patsubst %.cc,%.o,$(wildcard src/*.cc))
OBJS=$(PROG).o $(patsubst %.cc,%.o,$(wildcard src/*.cc))

# libraries
#LIBS=-lm $(shell geant4-config --libs) $(shell root-config --libs)
LIBS=-lm $(shell geant4-config --libs) $(shell root-config --libs)

# top rule
all: $(PROG)

# linking
$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROG)

include/LinkDef.h: Makefile
		echo '#pragma link C++ class vector<vector<float> >+;' >> $@
include/NerixVector.hh: Makefile
		echo '#include <vector>' > $@
src/NerixVectorDict.cc: include/NerixVector.hh include/LinkDef.h
		rootcint -f $@ -c $^
#         mv $^ include/
		sed -i -e 's|NerixVectorDict.h|NerixVectorDict.hh|' $@
		sed -i -e 's|include/NerixVector.hh|NerixVector.hh|' src/NerixVectorDict.h
#         mv $@ src/
		mv src/NerixVectorDict.h include/NerixVectorDict.hh

# how to compile c sources
%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) -c $<

# cleaning
clean:
	rm -f $(PROG) $(OBJS) include/LinkDef.h include/NerixVector.hh include/NerixVectorDict.hh src/NerixVectorDict.cc
	 
