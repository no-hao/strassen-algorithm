# Define the compiler to be used (GNU C Compiler)
CC = gcc

# Define the compiler flags
# -Wall: Enable all warning messages
# -Wextra: Enable extra warning messages
# -Werror: Treat warnings as errors
# -std=c99: Use the C99 language standard
CFLAGS = -Wall -Wextra -Werror -std=c99

# Define the include path (where to look for header files)
INCLUDES = -I./include

# Define the target executable name
TARGET = ./bin/matrix_app

# Use wildcard to collect all .c files in the src directory
SOURCES = $(wildcard src/*.c)

# Replace the .c extension with .o for object files
OBJECTS = $(SOURCES:.c=.o)

# Declare all and clean as phony targets
# This tells Make that they don't produce a file with the same name as the target
.PHONY: all clean

# Default target: all
all: $(TARGET)

# Link object files to create the target executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean target: remove object files and the target executable
clean:
	rm -f $(OBJECTS) $(TARGET)
