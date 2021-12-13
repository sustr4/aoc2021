#!/bin/bash

for COORD in x y; do
	cat day13-input.txt | grep "fold along ${COORD}=" | awk -v coord="${COORD}" -F"=" 'BEGIN {print "fix" coord " () {\n\tRET=$1"} {print "\tif [ $RET -gt " $2 " ]; then RET=$((" $2 "+" $2 "-$RET)); fi" } END { print "\tprintf $RET\n}"}'; done > day13-fix.sh

source day13-fix.sh

cat day13-input.txt | egrep -E '^[0-9]' | while IFS=, read -r x y; do
	printf "`fixx $x` -`fixy $y`\n"
done > day13.dat

gnuplot -c day13.plt 
evince day13.pdf
