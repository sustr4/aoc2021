#!/bin/bash

declare -A RULE

cat day14-input.txt | grep -- '->' > day14-rules

while read -r PAIR ARR INS; do RULE["${PAIR}"]="${INS}"; done < day14-rules

head -n 1 day14-input.txt | sed 's/\(.\)/\1\n/g' | grep -Ev '^$' > day14-poly

for i in {1..10}; do
	PREV=""
	while read CHAR; do
		if [ "$PREV" != "" ]; then
			printf "$PREV\n${RULE["${PREV}${CHAR}"]}\n"
		fi
		PREV="$CHAR"
	done < day14-poly > day14-poly.new
	echo $PREV >> day14-poly.new

	mv day14-poly.new day14-poly
	wc -l day14-poly
done

sort -o day14-poly day14-poly
cat day14-poly | uniq -c | sort -n

