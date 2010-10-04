#!/bin/bash

for i in `seq 10000 10000 100000`; do
	for j in `seq 0.01 0.01 0.5`; do
		for k in `seq 0.01 0.01 0.5`; do
			echo "---------------------------------------------------------"
			echo -e "WU\tThreshold\tAmount"
			echo -e "$i\t$j\t$k"
			mpirun -machinefile $1 -np $2 ./primes $i $j $k
			echo "---------------------------------------------------------"
		done
	done
done
