#!/bin/bash

FILENAME="ERROR"

rm -f scanner*.orig
rm -f scanner*.r*txt
rm -f day19-next

echo "0" > day19-next

while read line; do
	if [ "${line:0:3}" == "---" ]; then
		SCANNO=`echo "$line" | grep -Eo '[0-9][0-9]*'`
		FILENAME="scanner${SCANNO}.orig"
	fi

	echo "$line" >> $FILENAME
done < day19-input.txt.bak

cp scanner0.orig scanner0.r00.x0.y0.z0.txt


while [ true ]; do
	rm day19-input.txt	# Construct new input
	for i in {0..35}; do
		ls scanner$i.r* >/dev/null 2>/dev/null
		if [ $? -eq 0 ]; then
			cat scanner$i.r* >> day19-input.txt
		else
			cat scanner$i.orig >> day19-input.txt
		fi
	done


	cat day19-next | while read FIRST; do
		printf "Scanner to check:\n${FIRST}\n"
		for i in {0..35}; do
			touch day19-new
#			echo Checking for existence of scanner$i.r*
			ls scanner$i.r* >/dev/null 2>/dev/null
			if [ $? -ne 0 ]; then
#				printf "Checking against $i\n"
				./day19-gen $FIRST $i >> day19-new 2>/dev/null
			fi

		done
	done

	echo Next time check:
	cat day19-new
	mv day19-new day19-next
	NEXTNO=`wc -l day19-next | awk '{print $1}'`
	echo Supposedly we should be checking $NEXTNO scanners on next run
	if [ $NEXTNO -eq 0 ]; then break
	fi
done

cat day19-input.txt | grep ',' | sort | uniq > day19-uniq

printf "Number of unique points: "
wc -l day19-uniq

ls scanner*.r* | sed 's/.*x\([0-9-]*\)\.y\([0-9-]*\)\.z\([0-9-]*\)\.txt/\1 \2 \3/' > day19-scanners

rm -f day19-manhattan
while read -r x1 y1 z1; do
	while read -r x2 y2 z2; do
		echo $x2 $x1 $y2 $y1 $z2 $z1 
	done < day19-scanners
done < day19-scanners | awk 'function abs(v) {return v < 0 ? -v : v} {
	print abs($1-$2)+abs($3-$4)+abs($5-$6)
}' >> day19-manhattan


printf "Longest taxicab distance: "
sort -n day19-manhattan | tail -n 1

