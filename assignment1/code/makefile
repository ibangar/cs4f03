# g) This make file compiles the project and produces the binary executable file matvec

SOURCES=$(wildcard src/*.c)

LIBRARY+=-lmpi

default:
	mkdir -p build
	mpicc -Wall $(SOURCES) -o build/matvec $(LIBRARY)

pdf:
	mkdir -p build
	enscript -p ./build/code.ps ./src/*
	ps2pdf ./build/code.ps ./build/code.pdf

clean:
	rm -fr build
