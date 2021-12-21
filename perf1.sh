#!/bin/bash


MY=/home/a92841/Trabalho-Programa--o-Paralela
gcc --version ## get compiler version
perf --version 
cat /proc/cpuinfo
perf report -n > perfreport
