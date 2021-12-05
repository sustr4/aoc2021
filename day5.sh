#!/bin/bash

cat day5-input.txt | awk -F"[, ]" '{
	if ($1==$4) {
		low=($2>$5)?$5:$2;
		high=($2>$5)?$2:$5;
		for (i=low; i<=high; i+=1) 
			print $1 "," i
	} else if ($2==$5) {
		low=($1>$4)?$4:$1;
		high=($1>$4)?$1:$4;
		for (i=low; i<=high; i+=1) 
			print i "," $2
	}
}' | sort | uniq -c | grep -Ev '^\s*1\s' | wc -l


cat day5-input.txt | awk -F"[, ]" '
function abs(v) {return v < 0 ? -v : v}
{
	stepx=($1>$4)?-1:($1==$4)?0:1;
	stepy=($2>$5)?-1:($2==$5)?0:1;

	n=($2!=$5)?abs($5-$2):abs($4-$1);

	for (i=0; i<=n; i+=1)
		print $1+i*stepx "," $2+i*stepy
}' | sort | uniq -c | grep -Ev '^\s*1\s' | wc -l

