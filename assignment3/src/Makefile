TEST_JULIA=julia_acc_s

CFLAGS+=-Wall
CFLAGS+=-O2
CFLAGS+=-fopenmp

INCLUDES+=-I./

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)

default: julia_serial julia_omp julia_acc_s julia_acc_d

%.o : %.c
	gcc $(CFLAGS) $(INCLUDES) -c $^ -o $@ -lrt

julia_serial: $(OBJ)
	gcc $(CFLAGS) serial/julia.c $(INCLUDES) $^ -o julia_serial -lrt

julia_omp: $(OBJ)
	gcc $(CFLAGS) omp/julia.c $(INCLUDES) $^ -o julia_omp -lrt

julia_acc_s: $(OBJ)
	pgcc -acc -Minfo -ta=nvidia,cc13 acc_s/julia.c $(INCLUDES) $^ -o julia_acc_s -lrt

julia_acc_d: $(OBJ)
	pgcc -acc -Minfo -ta=nvidia,cc13 acc_d/julia.c $(INCLUDES) $^ -o julia_acc_d -lrt

# this runs are on Mac. On Linux, e.g. penguin, replace open by gthumb
run0: 
	./$(TEST_JULIA) 0 -0.4 0.6  -0.2 -0.1 1  1.1 1000 1000 1000  image.bmp ; open image.bmp

run1: $(TEST_JULIA)
	./$(TEST_JULIA) 0 -0.4 0.6  -0.2 -0.1 1  1.1 1000 1000 3000  image.bmp ; open image.bmp

run2: $(TEST_JULIA)
	./$(TEST_JULIA) 0 -0.4 0.6  -0.181862 -0.181772 1.019085 1.019175 1000 1000 100  image.bmp ; open image.bmp

run3: $(TEST_JULIA)
	./$(TEST_JULIA) 0 -0.4 0.6  -0.181862 -0.181772 1.019085 1.019175 2000 2000 10000  image.bmp ; open image.bmp

run4: $(TEST_JULIA)
	./$(TEST_JULIA) 1 -0.8  0.156  -2 2 -2 2 1000 1000 100  image.bmp ; open image.bmp

run5: $(TEST_JULIA)
	./$(TEST_JULIA) 1 -0.8  0.156  -2 2 -2 2 1000 1000 1000  image.bmp ; open image.bmp

run6: $(TEST_JULIA)
	./$(TEST_JULIA) 1 -0.8  0.156  -.2 .2 -.2 .2 4000 4000 10000  image.bmp ; open image.bmp

run7: $(TEST_JULIA)
	./$(TEST_JULIA) 1 0.285 0.01  -2 2 -2 2 1000 1000 1000  image.bmp ; open image.bmp

run8: $(TEST_JULIA)
	./$(TEST_JULIA) 1 0.285 0.01  -.2 .2 -.2 .2 1000 1000 1000  image.bmp ; open image.bmp

run9: 
	./$(TEST_JULIA) 0 -0.4 0.6  -2 2 -2 2  1000 1000 1000  image.bmp ; open image.bmp



clean:
	@rm -rf $(wildcard *.o) $(TEST_JULIA) *~ *.bak *.bmp julia_serial julia_omp julia_acc_s julia_acc_d
