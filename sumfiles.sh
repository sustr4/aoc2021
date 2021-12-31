#!/bin/bash

SUM="0"
for i in {-97823..97897}; do

	f=`echo $i | sed 's/-/neg/'`.txt

	if [ -f $f ]; then
		echo "Reading file $f (${SUM})"
		read no < $f
	fi
	SUM=`echo $SUM + $no | bc`

done

echo $SUM
