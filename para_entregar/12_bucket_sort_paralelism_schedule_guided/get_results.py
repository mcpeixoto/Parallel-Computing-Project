
# Load file
import os
import sys
import time
from glob import glob
from os.path import join
import time

def load_file(file_name):
    with open(file_name, 'r') as f:
        return f.read()


threads = [2, 4, 8, 16, 32, 64, 128]
lens_vector = [1, 10, 100, 1000, 10000, 100000, 1000000, 10000000]
nums_bucket = [1, 10, 100, 1000, 10000, 100000, 1000000]

i = 1
total_tests = len(threads) * len(lens_vector) * len(nums_bucket)
for thread in threads:
    for len_vector in lens_vector:
        for num_bucket in nums_bucket:
            # Remove slurm file
            os.system("rm slurm-*")
            print("\n")
            print("-"*5)
            print(f"Currenty at Test {i} of {total_tests} ({round((i/total_tests)*100,2)}\% Complete): \nthread = {thread} \nlen_vector = {len_vector} \nnum_bucket = {num_bucket}")
            print("-"*5)
            # Load original code
            bucket_original = load_file("bucket_original.c")

            # Replace the number of threads
            bucket_original = bucket_original.replace("num_threads(64)", "num_threads(" + str(thread) + ")")

            # Replace len vector
            bucket_original = bucket_original.replace("tam_bucket 10000", "tam_bucket " + str(num_bucket))

            # Replace the number of buckets
            bucket_original = bucket_original.replace("num_bucket 10", "num_bucket " + str(thread))

            # Write the current test to bucket.c
            with open("bucket.c", 'w') as f:
                f.write(bucket_original)
            
            # Run the code
            os.system("make clean && make && sbatch --partition=cpar time5x.sh")

            # Wait untill "slurm-*" file appears
            while True:
                slurms = glob(join(os.getcwd(), "*.out"))
                
                slurms = [x for x in slurms if 'bucket.out' not in x] # bucket.out dava problemas xD

                if len(slurms) >= 1:
                    print("Results detected!")
                    break
                else:
                    print("Waiting for results..")
                    time.sleep(2)

            # Raise error if len(slurms) != 1
            if len(slurms) != 1:
                raise Exception("More than one slurm file detected")

            # Load file
            slurm_file = str(load_file(slurms[0]))


            real_time = float(slurm_file.split("m")[-3].split("s")[0]) / 5 # we divide by 5 because we run 5 times

            print("-"*5)
            print("Total time:", real_time)

            # Write to csv
            with open("results.csv", 'a') as f:
                f.write("{},{},{},{}\n".format(thread, len_vector, num_bucket, real_time))

            

            i += 1
            #time.sleep(1)

            # input("check")

            # Delete bucket.c
            os.system("rm bucket.c")

            time.sleep(0.5) # Se nao fica too fast for the OS xD
            

            





# write 