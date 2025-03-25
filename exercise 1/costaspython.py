#!/usr/bin/env python2.5
from __future__ import with_statement

import os
import re

# Function to extract the time number from the specific format
def extract_time(text):
    match = re.search(r'Time\s([\d\.]+)', text)
    return match.group(1) if match else None

# Get list of all files in the current directory
files = [f for f in os.listdir('.') if os.path.isfile(f)]

with open('output', 'w') as output_file:
    for file_name in files:
        with open(file_name, 'r') as f:
            content = f.read()
            time_value = extract_time(content)
            if time_value:
                output_file.write('%s "%s"\n' % (file_name, time_value))

