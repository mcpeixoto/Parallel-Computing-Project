#!/bin/bash


MY=/home/a92841/Trabalho-Programa--o-Paralela/para_entregar/3_bucket_sort_sequential_improved_sort
perf record  ./bucket.out 
perf report -n > perfreport