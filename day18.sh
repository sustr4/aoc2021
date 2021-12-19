#!/bin/bash

DEBUG=1

if [ ! -f day18-form ]; then
	echo '[' > day18-form
	head -n 1 day18-input.txt >> day18-form
	echo ',' >> day18-form
	head -n 2 day18-input.txt | tail -n 1 >> day18-form
	echo ']' >> day18-form
fi


tokenize () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	unwrap $FILE
	sed -i 's/\s\s*/\n/g' $FILE
	sed -i 's/,/\n,\n/g' $FILE
}

unwrap () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	sed -i 's/\s*\[/\n[ /g' $FILE
	sed -i 's/]\s*/ ]\n/g' $FILE
}

wrap () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	LINE=""
	while read line; do LINE="${LINE} ${line}"; done < $FILE
	echo "$LINE" > $FILE
}

removeSpaces () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	sed -i 's/\s//g' $FILE
}

explode () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	unwrap $FILE
	cat $FILE | awk -F'[ ,]' 'BEGIN {depth=0;count=0;} {
		if ($1=="[") depth+=1;
		if((count==0)&&(depth>4)&&($1=="[")&&($NF=="]")) {
			$1="+"; $NF="+"; print $0; count+=1;
		}
		else {
			print $0;
		}

		if ($NF=="]") depth-=1;
	}' > $FILE.tmp
	mv $FILE.tmp $FILE
}

moveOperands () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	wrap $FILE
	#move operand left
	sed -i 's/\([0-9][0-9]*\)\([^+^0-9]*\)[+]\([ ][ ]*\)\([0-9][0-9]*\)/ \1+\4 \2\3 /' $FILE
	#move operand right
	sed -i 's/\([0-9][0-9]*\)\([ ][ ]*\)[+]\([^+^0-9]*\)\([0-9][0-9]*\)/ \2\3 \1+\4 /' $FILE
}

addZeroes () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	wrap $FILE
	sed -i 's/,\s*]/, 0 ]/g' $FILE
	sed -i 's/\[\s*,/[ 0 ,/g' $FILE
	sed -i 's/[+]\s*[0-9][0-9]*/0/g' $FILE
	sed -i 's/[0-9][0-9]*\s*[+]/0/g' $FILE
}

countChars () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	grep -Eo '\S' $FILE | wc -l
}

makeSums () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	tokenize $FILE
	while read line; do
		echo $line | grep -E '[0-9]\+[0-9]' >/dev/null
		if [ $? -eq 0 ]; then
			echo $line | bc
		else
			echo $line
		fi
	done < $FILE > $FILE.tmp
	mv $FILE.tmp $FILE
}

makeMagnitudes () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	unwrap $FILE
	cat $FILE | awk -F'[ ,]' 'BEGIN {depth=0;count=0;} {
		if($1=="["&&$NF=="]") { print 3*$2 + 2*$3; }
		else print $0;
	}' > $FILE.tmp
	mv $FILE.tmp $FILE
}

split () {
	if [ "$1" == "" ]; then FILE="day18-form"; else FILE="$1"; fi
	tokenize $FILE
	COUNT=0
	while read line; do
		echo $line | grep -E '[0-9][0-9]' >/dev/null
		if [ $? -eq 0 ] && [ $COUNT -eq 0 ]; then
			echo "[ `echo ${line}/2 | bc` , `echo "(${line}+1)/2" | bc` ]"
			COUNT=1
		else
			echo $line
		fi
	done < $FILE > $FILE.tmp
	mv $FILE.tmp $FILE
}


	wrap
	removeSpaces



	while [ true ]; do
		cp day18-form before

		while [ true ]; do
			cp day18-form before-explode
			explode
			moveOperands
			makeSums	
			addZeroes
			removeSpaces
			printf "Explode -> "; cat day18-form | sed 's/\s//g'
			diff -u before-explode day18-form >/dev/null
			if [ $? -eq 0 ]; then break; fi
		done

		split

		wrap 
		removeSpaces 
		printf "Split   -> "; cat day18-form
		diff -u before day18-form >/dev/null
		if [ $? -eq 0 ]; then break; fi
	done

	cp day18-form day18-mag
	wrap day18-mag
	removeSpaces day18-mag

	while [ true ]; do
		cp day18-mag before

		makeMagnitudes day18-mag

		wrap day18-mag
		removeSpaces day18-mag

		diff -u before day18-mag >/dev/null
		if [ $? -eq 0 ]; then break; fi
	done

	cat day18-mag >> day18-mag-all



