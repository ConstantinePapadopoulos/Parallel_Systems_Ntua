# Compiler
MPICC = mpicc

# Executable name
TARGET = info

# Source files
SRC = info.c

# Flags (add optimization/debug flags as needed)
CFLAGS = -O2

# Default rule
all: $(TARGET)

# Rule to compile the executable
$(TARGET): $(SRC)
	$(MPICC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean rule to remove generated files
clean:
	rm -f $(TARGET)

