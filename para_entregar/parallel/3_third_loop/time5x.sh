#!/bin/bash


MY=/home/a92841/Trabalho-Programa--o-Paralela/para_entregar/parallel/2_second_loop/1_bucket_sort_paralelism_omp_parallel

time for i in {1..5}; do $MY/bucket.out; done