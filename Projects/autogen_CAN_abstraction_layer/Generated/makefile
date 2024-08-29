# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Target executable
TARGET = testss

# Source files
SRCS = tests.c brightside.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

# Run the program
run: $(TARGET)
	./$(TARGET)
