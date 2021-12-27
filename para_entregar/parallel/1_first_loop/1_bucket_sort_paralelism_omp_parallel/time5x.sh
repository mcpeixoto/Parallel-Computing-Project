#!/bin/bash


MY=/home/a92838/Trabalho/para_entregar/parallel/1_first_loop/1_bucket_sort_paralelism_omp_parallel

time for i in {1..5}; do $MY/bucket.out; done