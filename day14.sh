#!/bin/bash

declare -A RULE
declare -A COMBOS
declare -A NEWCOMBOS

# Separate rules from input
cat day14-input.txt | grep -- '->' | awk '{print $1 " " $3}' > day14-rules

# Separate string from input

# Generate combinations
UNIQS=`grep -Eo '[A-Z]' day14-input.txt | sort | uniq`
for i in $UNIQS; do
	for j in $UNIQS; do
		COMBOS["${i}${j}"]=0
	done
done
START="1`head -n 1 day14-input.txt`2"
LEN=$((${#START}-2))
for i in `seq 0 $LEN`; do
	COMBOS["${START:$i:2}"]=1
done



for i in ${!COMBOS[@]}; do NEWCOMBOS["$i"]=0; done

for i in {1..10}; do
	while read -r PAIR INS; do
		if [ "$PAIR" == "" ]; then continue; fi
		NEW1="${PAIR:0:1}${INS}"
		NEW2="${INS}${PAIR:1:1}"
		if [ ${COMBOS["$PAIR"]} -gt 0 ]; then
			COMBOS["$NEW1"]=$((${COMBOS["$NEW1"]}+${COMBOS["$PAIR"]}))
			COMBOS["$NEW2"]=$((${COMBOS["$NEW2"]}+${COMBOS["$PAIR"]}))
			COMBOS["$PAIR"]=0
		fi
	done < day14-rules
done

for i in ${!COMBOS[@]}; do
  printf "${i:0:1}\t${COMBOS[$i]}\n"
  printf "${i:1:1}\t${COMBOS[$i]}\n"
done > day14-counts

datamash -s groupby 1 sum 2 < day14-counts

