# g) This make file compiles the project and produces the binary executable file matvec

SOURCES=$(wildcard src/*.c)

LIBRARY+=-lmpi

default:
	mkdir -p build
	mpicc -Wall $(SOURCES) -o build/matvec $(LIBRARY)

clean:
	rm -fr build
