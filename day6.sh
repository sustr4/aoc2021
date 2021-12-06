#/bin/bash

cat day6-input.txt | sed 's/,/\n/g' | grep -Ev '^$' > output.txt

for i in {1..100}; do
	echo $i;
	mv  output.txt output.tmp
	cat output.tmp | awk '{
		i=$1-1;
		if (i<0) {
			print 6;
			print 8;
		} else {
			print i;
		}
	}' > output.txt

done

wc -l output.txt
