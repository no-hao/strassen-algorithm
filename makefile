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

# Define the target executable names
TARGET_APP = ./bin/matrix_app
TARGET_CREATE_CSV = ./bin/create_csv

# Define the directories
SRC_DIR = src
DATA_DIR = data
BIN_DIR = bin
OUTPUT_DIR = output
RESULT_DIR = result

# Define the sources and objects for create_csv
CREATE_CSV_SOURCES = $(SRC_DIR)/create_csv.c
CREATE_CSV_OBJECTS = $(CREATE_CSV_SOURCES:.c=.o)

# Create the output directory if it doesn't exist
$(shell mkdir -p $(OUTPUT_DIR))

# Create the data directory if it doesn't exist
$(shell mkdir -p $(DATA_DIR))

# Create the result directory if it doesn't exist
$(shell mkdir -p $(RESULT_DIR))


# Default target: all
all: $(TARGET_APP) $(TARGET_CREATE_CSV)

# Link object files to create the target executable
$(TARGET_APP): $(SRC_DIR)/main.o $(SRC_DIR)/matrix.o $(SRC_DIR)/brute_force.o $(SRC_DIR)/strassen.o $(SRC_DIR)/combined.o $(SRC_DIR)/file_utils.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^


$(TARGET_CREATE_CSV): $(CREATE_CSV_OBJECTS) $(SRC_DIR)/file_utils.o $(SRC_DIR)/matrix.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -lm

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean target: remove object files and the target executables
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET_APP) $(TARGET_CREATE_CSV)
