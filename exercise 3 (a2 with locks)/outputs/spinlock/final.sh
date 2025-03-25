#!/bin/bash

# Open finals.out file for writing (create or overwrite)
output_file="finals.out"
> "$output_file"  # This clears the file if it already exists

# Loop through all .out files
for file in *.out; do
    # Extract the number of threads from the filename (e.g., 8.out -> 8)
    threads=$(basename "$file" .out)
    
    # Search for the line containing the total time and extract it using sed
    total_time=$(grep "total =" "$file" | awk -F'total = ' '{print $2}' | awk '{print $1}')
    
    # Check if we found the total time and print the result
    if [ -n "$total_time" ]; then
        echo "Threads: $threads, Total Time: $total_time" >> "$output_file"
    else
        echo "Threads: $threads, Total Time: Not found" >> "$output_file"
    fi
done

echo "Results saved in $output_file"

