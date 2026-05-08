#!/bin/bash

gcc 4_matrix.c -o matrix -O3 -pthread

> data.txt

for N in 500 1000 1500 2000 2500; do
    for t in 1 2 4 8 16 32 64; do
        if [ $t -le $N ]; then
            echo "Testing N=$N threads=$t"
            ./matrix $N $t >> data.txt
        fi
    done
done