#!/bin/bash
myExecutable="./mymake"
exExecutable="./exMymake"
failedCases=""
passedCases=""
numberOfCases=3
targets=("spellcheck", "utils.h", "utils.o")
for ((X=1; ${X} <= numberOfCases; X++));do
	for target in targets;do
		$myExecutable <test_$X.txt >myOut
		echo $? >>myOut
		$exExecutable <test_$X.txt >exOut
		echo $? >>exOut
		diffResult=$(diff myOut exOut)
		if [ -z "$diff_result" ]; then
			passedCases+="$X "
		else
			failedCases+="$X "
		fi
	done
done

echo "PASSED CASES: $passedCases"

echo "FAILED CASES: $failedCases"