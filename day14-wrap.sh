#!/bin/bash

INPUT=`head -n 1 day14-input.txt`
./day14 $INPUT 14 1 0 | sed 's/\(.\)/\1\n/g' | awk 'BEGIN {prev=""} {if(prev!="") print prev $1; prev=$1}' > day14-20rounds.txt

UNIQS=`grep -Eo '[A-Z]' day14-input.txt | sort | uniq`
for i in $UNIQS; do
	for j in $UNIQS; do
		./day14 "${i}${j}" 26 0 1 > ${i}${j}.accel
	done
done

rm -f day14-pivot

cat day14-20rounds.txt | while read line; do
	cat ${line}.accel >> day14-pivot
done

printf "${INPUT: -1}\t1" >> day14-pivot

cat day14-pivot | datamash -s -g 1 sum 2 > day14-totals

cat day14-totals

MAX=`cat day14-totals | awk '{print $2}' | sort -n | tail -n 1`
MIN=`cat day14-totals | awk '{if ($2!="0") print $2}' | sort -n | head -n 1`

echo Difference $(($MAX-$MIN))

