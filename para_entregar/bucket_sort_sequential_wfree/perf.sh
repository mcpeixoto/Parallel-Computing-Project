#!/bin/bash


MY=/home/a92838/Trabalho/buck_sort_sequential

perf record  ./bucket.out 
perf report -n > perfreport