#!/bin/bash
myExecutable="./changeBase"
exExecutable="./exChangeBase"
failedCases=""
passedCases=""
numberOfCases=4
for ((X=1; ${X} <= numberOfCases; X++));do
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

echo "PASSED CASES: $passedCases"

echo "FAILED CASES: $failedCases"