#!/bin/bash


MY=/home/a92841/Trabalho-Programa--o-Paralela/buck_sort_sequential

perf record  ./bucket.out 
perf report -n > perfreport