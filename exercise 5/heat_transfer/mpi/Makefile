MPICC = mpicc
CFLAGS = -Wall -Wextra -Wno-unused -O3 
LDFLAGS = -lm

H_FILES = utils.h
SRC = jacobi.c utils.c
OBJ = $(SRC:.c=.o)
EXEC = jacobi_mpi

all: $(EXEC)

$(EXEC): $(OBJ)
	$(MPICC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

jacobi.o: jacobi.c $(H_FILES)
	$(MPICC) $(CFLAGS) -c $< -o $@

utils.o: utils.c $(H_FILES)
	$(MPICC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) *.err *.out

