#!/bin/bash


MY=/home/a92838/Trabalho/para_entregar/sequential/4_bucket_sort_w_perf
perf record  ./bucket.out 
perf report -n > perfreport