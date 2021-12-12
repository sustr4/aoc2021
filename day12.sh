#!/bin/bash

TASK=1 #Possible values: 1, 2

cat day12-input.txt | awk -F"-" '{print $1 "-" $2 "\n" $2 "-" $1}' | sort | grep -v -- "-start" | grep -v "end-" > day12.pat

score () {
	echo $1 | grep -Eo '[-][a-z][a-z]*' | sort | uniq -c | awk 'BEGIN {prod=1} {prod*=$i} END {print prod}'
}

cat day12.pat | awk -F"-" '{
	if($1!=last) printf "/p\ns/^\\(.*-" $1 "\\)$/\\1-" $2;
	else printf "\\n\\1-" $2;
	last=$1
} END {print "/p"}' | sed 's/\*-start/\*start/' | tail -n +2 > day12.sed

rm day12-single*sed

PATTERNS=`wc -l day12.sed | awk '{print $1}'`

for i in `seq 1 $PATTERNS`; do
	tail -n +${i} day12.sed | head -n 1 > day12-single-${i}.sed
done

echo "start" > cave

if [ -e cave.tmp ]; then rm cave.tmp; fi
if [ -e cave-ends ]; then rm cave-ends; fi
for i in {1..100}; do
	printf "$i\t"
	for pat in day12-single*sed; do
		cat cave | sed -n -f "${pat}" >> cave.tmp
	done
	rm cave
	touch cave
	cat cave.tmp |  while read line; do
		SCORE=`score $line`
		if [ $SCORE -le $TASK ]; then
			echo $line >> cave
		fi
	done
	rm cave.tmp

	grep -E "-end$" cave >> cave-ends
	grep -Ev "-end$" cave >> cave.tmp

	mv cave.tmp cave

	wc -l cave
done

printf "Solution, Task $TASK: "
cat cave-ends | sort | uniq | wc -l

