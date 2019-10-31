#!/bin/sh

scripts=`find . -name "*.sh" -not -path "$0"`
for script in `ls *.sh`
do
	if [ "./$script" = "$0" ]; then
		continue
	fi
	script=$(echo "$script" | tr -d '\r')
	echo "Test:[$script]"
	echo "============================================================="
	./$script
	echo ""
	echo "============================================================="
done
