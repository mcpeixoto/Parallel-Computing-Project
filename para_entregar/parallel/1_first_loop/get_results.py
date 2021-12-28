
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
lens_vector = [1, 10, 100, 1000, 10000, 100000, 1000000]
nums_bucket = [1, 10, 100, 1000, 10000, 100000]


# If results.csv exists
if os.path.isfile("results.csv"):
    # Ask user if he wants to delete it
    print("[!] Results.csv already exists, do you want to delete it? (y/n)")
    while True:
        answer = input()
        if answer == "y":
            os.system("rm results.csv")
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
            os.system("rm slurm-*")
            time.sleep(0.5)
            print("\n")
            print("-"*5)
            print(f"Currenty at Test {iteration} of {total_tests} ({round((iteration/total_tests)*100,2)}% Complete): \nthread = {thread} \nlen_vector = {len_vector} \nnum_bucket = {num_bucket}")
            print("-"*5)
            # Load original code
            bucket_original = load_file("bucket_original.c")

            # Replace the number of threads
            bucket_original = bucket_original.replace("num_threads(64)", "num_threads(" + str(thread) + ")")

            # Replace len vector
            bucket_original = bucket_original.replace("tam_bucket 10000", "tam_bucket " + str(len_vector))

            # Replace the number of buckets
            bucket_original = bucket_original.replace("num_bucket 10", "num_bucket " + str(num_bucket))

            # Write the current test to bucket.c
            with open("bucket.c", 'w') as f:
                f.write(bucket_original)
            
            # Run the code
            os.system("make clean && make && sbatch --partition=cpar time5x.sh")

            # Wait untill "slurm-*" file appears
            time_to_wait = 2
            i = 1
            while True:
                slurms = glob(join(os.getcwd(), "*.out"))
                
                slurms = [x for x in slurms if 'bucket.out' not in x] # bucket.out dava problemas xD

                if len(slurms) >= 1 and 'sys' in load_file(slurms[0]): # O ficheiro existe e carregou até a ultima linha
                    print("Results detected!")
                    break
                else:
                    print(f"Waiting for results for {i*(time_to_wait+i/2)}s..")
                    time.sleep(time_to_wait+i/2) # Fancy waiting
                i += 1

            # Raise error if len(slurms) != 1
            if len(slurms) != 1:
                raise Exception("More than one slurm file detected")

            # Load file
            slurm_file = str(load_file(slurms[0]))
            print(slurm_file)
            # Get real time
            try:
                real_time_line = slurm_file.split("\n")[-4]
                minutes = float(real_time_line.split("m")[0].split('real')[1])
                seconds = float(real_time_line.split("m")[1].split("s")[0])

                real_time = (minutes*60 + seconds) / 5 # we divide by 5 because we run 5 times
                print("-"*5)
                print("Total time:")
                print(f"{minutes}m {seconds}s / 5 = {real_time}s")

                if not 'Is sorted? Yes' in slurm_file:
                    #print("Slurm file:\n", slurm_file)
                    raise Exception("Is not sorted")

            except:
                print("[!] Error getting real time, setting to None")
                real_time = None
                print("Slurm file:\n", slurm_file)


            # Write to csv
            with open("results.csv", 'a') as f:
                f.write("{},{},{},{}\n".format(thread, len_vector, num_bucket, real_time))

            

            iteration += 1

            # Delete bucket.c
            os.system("rm bucket.c")

            time.sleep(1) # Se nao fica too fast for the OS xD

            #input("sad")
            

os.system("rm slurm-*")





# write 