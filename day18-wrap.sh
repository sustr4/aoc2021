#!/bin/bash

head -n 1 day18-input.txt > day18-form

tail -n +2 day18-input.txt | while read line; do

	TASK="[`cat day18-form`,$line]"
	echo $TASK
	echo $TASK >  day18-form
	./day18.sh
done


