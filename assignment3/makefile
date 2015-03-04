PROGRAM=fractals

SOURCES=$(wildcard src/*.c)

LIBRARY+=-lm

default:
	mkdir -p build
	gcc -Wall $(SOURCES) -o build/$(PROGRAM) $(LIBRARY)

clean:
	rm -fr build
