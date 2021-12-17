#!/bin/bash

./day16 > day16-form 2>/dev/null

unwrap () {
	sed -i 's/\s*(/\n(/g' $1
	sed -i 's/)\s*/)\n/g' $1
}

wrap () {
	LINE=""
	while read line; do LINE="${LINE} ${line}"; done < day16-form
	echo "$LINE" > day16-form
}

for i in {1..21}; do
	unwrap day16-form
	
	mv day16-form day16-form.tmp
	cat day16-form.tmp | grep -Ev '^\s*$' | awk '{
		if(($1 ~ /[(].*/)&&($NF ~ /[)]/)) {
			if ($1=="(>") { #Greater than
				if($2 > $3) print "1";
				else print "0";
			} else
			if ($1=="(<") { #Lower than
				if($2 < $3) print "1";
				else print "0";
			} else
			if ($1=="(=") { #Equal than
				if($2 == $3) print "1";
				else print "0";
			} else
			if ($1=="(min") { #Min
				min=$2;
				for(i=3; i<NF; i++) if($i<min) min=$i;
				print min;
			} else
			if ($1=="(max") { #Max
				max=$2;
				for(i=3; i<NF; i++) if($i>max) max=$i;
				print max;
			} else
			if ($1=="(*") { #Multiplication
				prod=$2;
				for(i=3; i<NF; i++) prod*=$i;
				print prod;
			} else
			if ($1=="(+") { #Addition
				sum=$2;
				for(i=3; i<NF; i++) sum+=$i;
				print sum;
			} else
			{ print $0; }
			
		}
		else { print $0; }
	}' > day16-form

	wrap day16-form
done

cat day16-form


