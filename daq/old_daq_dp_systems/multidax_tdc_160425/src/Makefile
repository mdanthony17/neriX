# Makefile

# Macros
CXX=g++
CXXFLAGS=-O0 -g -Wall -Wno-deprecated -fPIC -DLINUX -I. $(shell root-config --cflags)
#CXXFLAGS=-O2 -Wall -Wno-deprecated -fPIC -DLINUX -I. $(shell root-config --cflags)
LD=g++
LDFLAGS=-O

# program name
PROG=multiDAX

# object files
OBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

# libraries
LIBS=-lCAENVME -lm -lxdio $(shell root-config --libs)

# top rule
all: $(PROG)

# linking
$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) $(LIBS) $(OBJS) -o $(PROG)

# how to compile cpp sources
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<

# cleaning
clean:
	rm -f $(PROG) $(OBJS)
	 
