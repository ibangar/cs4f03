CFLAGS+=-Wall
CFLAGS+=-O2
CFLAGS+=-fopenmp

INCLUDES+=-I./

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)

default: julia_omp julia_acc_s julia_acc_d

%.o : %.c
	gcc $(CFLAGS) $(INCLUDES) -c $^ -o $@

julia_omp: $(OBJ)
	gcc $(CFLAGS) omp/julia.c $(INCLUDES) $^ -o julia_omp

julia_acc_s: $(OBJ)
	gcc $(CFLAGS) acc_s/julia.c $(INCLUDES) $^ -o julia_acc_s

julia_acc_d: $(OBJ)
	gcc $(CFLAGS) acc_d/julia.c $(INCLUDES) $^ -o julia_acc_d

# this runs are on Mac. On Linux, e.g. penguin, replace open by gthumb
run0: 
	./julia 0 -0.4 0.6  -0.2 -0.1 1  1.1 1000 1000 1000  image.bmp ; open image.bmp

run1: julia
	./julia 0 -0.4 0.6  -0.2 -0.1 1  1.1 1000 1000 3000  image.bmp ; open image.bmp

run2: julia
	./julia 0 -0.4 0.6  -0.181862 -0.181772 1.019085 1.019175 1000 1000 100  image.bmp ; open image.bmp

run3: julia
	./julia 0 -0.4 0.6  -0.181862 -0.181772 1.019085 1.019175 2000 2000 10000  image.bmp ; open image.bmp

run4: julia
	./julia 1 -0.8  0.156  -2 2 -2 2 1000 1000 100  image.bmp ; open image.bmp

run5: julia
	./julia 1 -0.8  0.156  -2 2 -2 2 1000 1000 1000  image.bmp ; open image.bmp

run6: julia
	./julia 1 -0.8  0.156  -.2 .2 -.2 .2 4000 4000 10000  image.bmp ; open image.bmp

run7: julia
	./julia 1 0.285 0.01  -2 2 -2 2 1000 1000 1000  image.bmp ; open image.bmp

run8: julia
	./julia 1 0.285 0.01  -.2 .2 -.2 .2 1000 1000 1000  image.bmp ; open image.bmp

run9: 
	./julia 0 -0.4 0.6  -2 2 -2 2  1000 1000 1000  image.bmp ; open image.bmp



clean:
	@rm -rf $(OBJ) julia *~ *.bak *.bmp julia_omp julia_acc_s julia_acc_d
