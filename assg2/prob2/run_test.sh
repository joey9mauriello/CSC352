#!/bin/bash
myExecutable="./changeBase"
exExecutable="./exChangeBase"
failedCases=""
passedCases=""
numberOfCases=4
for ((X=1; ${X} <= numberOfCases; X++));do
	$myExecutable <test_$X.txt >myOut
	echo $? >>myOut
	./exChangeBase <test_$X.txt >exOut
	echo $? >>exOut
	if [ -z diff myOut exOut ]; then
		passedCases+="$i "
	else
		passedCases+="$i"
	fi
done