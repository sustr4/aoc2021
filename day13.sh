#!/bin/bash

cat day13-input.txt | grep "fold along y=" | awk -F"=" 'BEGIN {print "fixy () {\n\tRET=$1"} {print "\tif [ $RET -gt " $2 " ]; then RET=$((" $2 "+" $2 "-$RET)); fi" } END { print "\tprintf $RET\n}"}' > day13-fix.sh
cat day13-input.txt | grep "fold along x=" | awk -F"=" 'BEGIN {print "fixx () {\n\tRET=$1"} {print "\tif [ $RET -gt " $2 " ]; then RET=$((" $2 "+" $2 "-$RET)); fi" } END { print "\tprintf $RET\n}"}' >> day13-fix.sh

source day13-fix.sh

cat day13-input.txt | egrep -E '^[0-9]' | while IFS=, read -r x y; do
	fixx $x
	printf " -"
	fixy $y
	printf "\n"
done > day13.dat

gnuplot -c day13.plt 
evince day13.pdf
