import os
import re

def merge_and_sort_out_files(directory, output_file):
    data = []

    # Iterate through all .out files in the specified directory
    for filename in os.listdir(directory):
        if filename.endswith('.out'):
            # Extract the number of threads and size from the filename
            match = re.match(r'run_omp_GOL_(\d+)_(\d+)\.out', filename)
            if match:
                num_threads = int(match.group(1))
                size = int(match.group(2))

                # Read the contents of the .out file
                file_path = os.path.join(directory, filename)
                file = open(file_path, 'r')  # Open the file
                contents = file.read().strip()  # Read and strip any extra whitespace
                file.close()  # Close the file

                # Append size and contents to the data list
                data.append((size, contents))

    # Sort the data by size (the first element of the tuple)
    data.sort(key=lambda x: x[0])

    # Write the sorted data to the output file
    output = open(output_file, 'w')  # Open output file for writing
    for size, contents in data:
        output.write('GameOfLife: Size %d %s\n' % (size, contents))  # Old-style formatting
    output.close()  # Close the output file

if __name__ == "__main__":
    input_directory = os.getcwd()  # Current working directory
    output_file = 'merged_output.out'  # Output file name

    merge_and_sort_out_files(input_directory, output_file)
 

