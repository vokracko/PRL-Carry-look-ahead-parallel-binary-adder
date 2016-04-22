#!/bin/bash

> time.dat

for procnum in 1 2 4 8 16 32 64 ; do
	i=0
	sum=0

	x=""
	y=""

	for i in $(seq 1 $procnum) ; do
		(($RANDOM & 1)) && r=1 || r=0
		x=$x$r
		(($RANDOM & 1)) && r=1 || r=0
		y=$y$r

	done

	echo $x > numbers
	echo $y >> numbers
	
	for tries in {1..10} ; do
		while read t; do
			((i++))
			sum=$(echo "$t + $sum" | bc)
		done < <(./test $procnum 2>&1 > /dev/null)
	done

	avg=$(echo "scale=10; $sum / $i" | bc)
	echo "$procnum $avg" >> time.dat
done
