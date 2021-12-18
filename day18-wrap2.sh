#!/bin/bash


while read line1; do
	while read line2; do
		if [ "$line1" != "$line2" ]; then
			TASK="[$line2,$line1]"
			echo $TASK
			echo $TASK > day18-form
			./day18.sh
			TASK="[$line1,$line2]"
			echo $TASK
			echo $TASK > day18-form
			./day18.sh
		fi
	done < day18-input.txt
done < day18-input.txt

cat day18-mag-all | sort -n
