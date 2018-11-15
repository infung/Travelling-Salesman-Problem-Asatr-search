#!/bin/bash

for citynum in 14 15 16
do 
	echo "$citynum cities: "
	sum=0
	index=0
	for file in MyInstances/${citynum}/*; do 
		./tsp < ${file} > output
		typeset cur=$(cat output)
		echo -n "$cur "
		sum="$( bc <<<"$sum+$cur")"
		index=$(($index+1))
	done
	average="$( bc -l <<<"$sum/10")"
	echo "average = $average"
done

