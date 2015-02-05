SOURCES=$(wildcard src/*.c)

LIBRARY+=-lmpi
LIBRARY+=-lgmp
LIBRARY+=-lm

default:
	mkdir -p build
	mpicc -Wall $(SOURCES) -o build/primegap $(LIBRARY)

pdf:
	mkdir -p build
	enscript -p ./build/code.ps ./src/*
	ps2pdf ./build/code.ps ./build/code.pdf

clean:
	rm -fr build
