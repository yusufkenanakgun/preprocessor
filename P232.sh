#!/bin/bash

# Check if the input argument is provided
if [ $# -ne 1 ]; then
    echo "Usage: $0 <C source file>"
    exit 1
fi

INPUT_FILE=$1
PREPROCESSOR="preprocessor"
OUTPUT_FILE="expanded.c"

# Check if the input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: File '$INPUT_FILE' not found!"
    exit 1
fi

# Compile the preprocessor program
gcc -o $PREPROCESSOR preprocessor.c

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo "Error: Compilation of preprocessor.c failed!"
    exit 1
fi

# Run the preprocessor program with the input file
./$PREPROCESSOR $INPUT_FILE

# Check if the expanded.c file was created
if [ -f "$OUTPUT_FILE" ]; then
    echo "Success: '$OUTPUT_FILE' generated."
else
    echo "Error: '$OUTPUT_FILE' was not generated!"
    exit 1
fi

# Clean up by removing the preprocessor executable
rm $PREPROCESSOR