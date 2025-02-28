# Define directories
SRC_DIR = src
INC_DIR = inc

# Define the output executable
OUTPUT = app

# Gather all source files (.c) in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Use gcc to compile the files
CC = gcc
CFLAGS = -I$(INC_DIR)

# Default target to build the app
all: $(OUTPUT)

# Rule to link the object files into the final executable
$(OUTPUT): $(SRC_FILES)
	$(CC) -o $(OUTPUT) $(SRC_FILES) $(CFLAGS)

# Clean up build files
clean:
	rm -f $(OUTPUT)

.PHONY: all clean

