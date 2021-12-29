from glob import glob
import os

# Get all .csv in current directory recursively
csv_files = glob('**/*.csv', recursive=True)

csv_directory = 'csvs'
# If the directory does not exist, create it
if not os.path.exists(csv_directory):
    os.makedirs(csv_directory)

for csv in csv_files:
    if not "csvs" in csv:
        # Move each .csv file to the csvs directory
        os.rename(csv, os.path.join(csv_directory, csv.split("/")[-2]+".csv"))