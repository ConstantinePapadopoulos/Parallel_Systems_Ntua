MPICC = mpicc
CFLAGS = -Wall -Wextra -Wno-unused -O3 
LDFLAGS = -lm

H_FILES = utils.h
SRC = mpi_skeleton.c utils.c
OBJ = $(SRC:.c=.o)
EXEC = RedBlack_mpi

all: $(EXEC)

$(EXEC): $(OBJ)
	$(MPICC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

mpi_skeleton.o: mpi_skeleton.c $(H_FILES)
	$(MPICC) $(CFLAGS) -c $< -o $@

utils.o: utils.c $(H_FILES)
	$(MPICC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) *.err *.out

