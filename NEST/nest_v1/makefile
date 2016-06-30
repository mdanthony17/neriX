
all: rootNest

rootNest: rootNest.cc nerix.hh
	g++ -g -o rootNest `root-config --cflags --libs` rootNest.cc

clean:
	rm -f rootNest
