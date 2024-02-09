#!/bin/bash

directory=${2:-.}
my_executable="./bacon"
example_executable="./exBacon"

# ANSI color codes
BLUE='\033[0;34m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

total_cases=0
passed_cases=0

# Updated test_case function
test_case() {
    local testcase_num=$1
    local flags=$2
    local input_data=$3
    local file_name=$4

    total_cases=$((total_cases + 1))
    echo -e "${BLUE}TESTCASE $total_cases:${NC}"

    {
        timeout 2 "$my_executable" $flags $file_name > myOut$total_cases 2>myErr$total_cases <<EOL
$input_data
EOL
    } &> /dev/null

    myRV=$?
    echo "$myRV" > myReturn$total_cases

    if [ $myRV -eq 124 ]; then
        echo -e "      [${RED} FAILED - Timed out ${NC}]"
    elif [ $myRV -gt 128 ]; then
        echo -e "      [${RED} FAILED - Abnormal termination ${NC}]"
    else
        "$example_executable" $flags $file_name > exOut$total_cases 2>exErr$total_cases <<EOL
$input_data
EOL

        exRV=$?
        echo "$exRV" > exReturn$total_cases

        diffVar=$(diff -Z myOut$total_cases exOut$total_cases)

        if [ -z "$diffVar" ]; then
            echo -e "      [${GREEN} stdout - PASSED ${NC}]"
            passed_cases=$((passed_cases + 1))
        else
            echo -e "      [${RED} stdout - FAILED ${NC}]"
        fi

        if [ $myRV -eq $exRV ]; then
            echo -e "      [${GREEN} return code - PASSED ${NC}]"
            passed_cases=$((passed_cases + 1))
        else
            echo -e "      [${RED} return code - FAILED ${NC}]"
        fi

        my_err_lines=$(wc -l < myErr$total_cases)
        ex_err_lines=$(wc -l < exErr$total_cases)

        if [ "$my_err_lines" -eq "$ex_err_lines" ]; then
            echo -e "      [${GREEN} stderr - PASSED ${NC}]"
            passed_cases=$((passed_cases + 1))
        else
            echo -e "     [${RED} stderr - FAILED ${NC}]"
        fi
        (valgrind --leak-check=full "$my_executable" $flags $file_name <<EOL > /dev/null) 2> valOut$total_cases
$input_data
EOL
        grepVar=$(grep "ERROR SUMMARY: 0 errors" valOut$total_cases)

        if [ -z "$grepVar" ]; then
            echo -e "      [${RED} valgrind - FAILED ${NC}]"
        else
            echo -e "      [${GREEN} valgrind - PASSED ${NC}]"
            passed_cases=$((passed_cases + 1))
        fi

        grepVar=$(grep "All heap blocks were freed -- no leaks are possible" valOut$total_cases)

        if [ -z "$grepVar" ]; then
            echo -e "      [${RED} memory free - FAILED ${NC}]"
        else
            echo -e "      [${GREEN} memory free - PASSED ${NC}]"
            passed_cases=$((passed_cases + 1))
        fi
    fi
}

#format: testcase num, -l flag, user input, directory
test_case 1 "-l" $'Kevin Bacon' "testcases/exMovieFile"
test_case 2 "-l" $'Matt Damon\nKevin Bacon' "testcases/exMovieFile"
test_case 3 "" $'Matt Damon' "testcases/exMovieFile"
test_case 4 "" $'Matt' "testcases/exMovieFile"
test_case 5 "" $'Matt Damon' "testcases/emptyFile"
test_case 6 "-l" $'Matt Damon' "testcases/emptyFile"
test_case 7 "-l" $'Ben Brewer' "testcases/longLink"
test_case 8 "-l" $'Freddy' "testcases/longLink"
test_case 9 "" $'Freddy' "testcases/longLink"
test_case 10 "" $'Matt Damon\nKevin Bacon\nChristmas\nBen Brewer' "testcases/NoActorKevinBacon"
test_case 11 "-l" $'Matt Damon\nKevin Bacon\nChristmas\nBen Brewer' "testcases/NoActorKevinBacon"
test_case 12 "-l -l -l -l -l" $'Matt Damon\nKevin Bacon\nChristmas\nBen Brewer' "testcases/NoActorKevinBacon"
test_case 13 "-l -l -l -l -l" $'Matt Damon\nKevin Bacon\nChristmas\nBen Brewer' "testcases/NoActorKevinBacon -l"
test_case 14 "-l -l -l -l -l" $'Matt Damon\nKevin Bacon\nChristmas\nBen Brewer' "testcases/NoActorKevinBacon -z"
test_case 15 "" $'Matt Damon\nKevin Bacon\nChristmas\nBen Brewer' ""
test_case 16 "testcases/exMovieFile" $'Matt Damon\nKevin Bacon\nChristmas\nBen Brewer' "testcases/NoActorKevinBacon"
test_case 17 "-l" $'Matt Damon' "testcases/FileDoesNotExist"
test_case 18 "-l" $'' "testcases/exMovieFile"
test_case 19 "" $'' "testcases/exMovieFile"
test_case 20 "-l" $'Ben Brewer' "testcases/badMovieFile"
test_case 21 "" $'Ben Brewer' "testcases/badMovieFile"
test_case 22 "" $'Joey M' "testcases/badActorFile"
test_case 23 "-l" $'Joey M' "testcases/badActorFile"
test_case 24 "" $'Joey M' "testcases/badActorFile2"
test_case 25 "-l" $'Joey M' "testcases/badActorFile2"

print_score() {
    ((total_cases *= 5))
    if [ $total_cases -eq 0 ]; then
        percentage=0
    else
        percentage=$(( (passed_cases * 100) / total_cases ))
    fi
    echo -e "\n${BLUE}EXPECTED GRADE:${NC} $percentage% ($passed_cases/$total_cases cases passed)"
}

print_score