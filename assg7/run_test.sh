#!/bin/bash
myExecutable="./shortestPaths"
exExecutable="./exShortestPaths"
failedCases=""
passedCases=""
numberOfCases=7
for ((X=1; ${X} <= numberOfCases; X++));do
	$myExecutable test_$X.txt <input_$X.txt >myOut$X
	echo $? >>myOut
	$exExecutable test_$X.txt <input_$X.txt >exOut$X
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