

import os
import sys
import time
from glob import glob
from os.path import join
import time
import subprocess


def load_file(file_name):
    with open(file_name, 'r') as f:
        return f.read()


threads = [2, 4, 8, 10, 12, 16, 24, 32, 48, 64]
lens_vector = [1000000, 800, 6500]
nums_bucket =  [10, 1000]


# If results_omp.csv exists
if os.path.isfile("results_omp.csv"):
    # Ask user if he wants to delete it
    print("[!] results_omp.csv already exists, do you want to delete it? (y/n)")
    while True:
        answer = input()
        if answer == "y":
            os.system("rm results_omp.csv")
            break
        elif answer == "n":
            break
        else:
            print("[!] Answer with 'y' or 'n'")

iteration = 1
total_tests = len(threads) * len(lens_vector) * len(nums_bucket)
for thread in threads:
    for len_vector in lens_vector:
        for num_bucket in nums_bucket:
            # Remove slurm file
            
            time.sleep(0.5)
            print("\n")
            print("-"*5)
            print(f"Currenty at Test {iteration} of {total_tests} ({round((iteration/total_tests)*100,2)}% Complete): \nthread = {thread} \nlen_vector = {len_vector} \nnum_bucket = {num_bucket}")
            print("-"*5)
            # Load original code
            bucket_original = load_file("bucket_ompTimer.c")

            # Replace the number of threads
            bucket_original = bucket_original.replace("num_threads(64)", "num_threads(" + str(thread) + ")")

            # Replace len vector
            bucket_original = bucket_original.replace("tam_bucket 10000", "tam_bucket " + str(len_vector))

            # Replace the number of buckets
            bucket_original = bucket_original.replace("num_bucket 10", "num_bucket " + str(num_bucket))

            # Write the current test to bucket.c
            with open("bucket.c", 'w') as f:
                f.write(bucket_original)

            time.sleep(0.5)
            
            # Run the code
            process = subprocess.Popen("make clean && make && srun --partition=cpar bucket.out", shell=True, stdout=subprocess.PIPE)

            process_return = str(process.stdout.read())
            # print(process_return)      

            real_time = float(process_return.split("-")[-2])

            print("Real time:", real_time)

            # Write to csv
            with open("results_omp.csv", 'a') as f:
                f.write("{},{},{},{}\n".format(thread, len_vector, num_bucket, real_time))

            

            iteration += 1

            # Delete bucket.c
            os.system("rm bucket.c")

            time.sleep(1) # Se nao fica too fast for the OS xD

            #input("sad")
            

os.remove("bucket.out")


