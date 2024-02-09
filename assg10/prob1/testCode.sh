#!/bin/bash
# Joey Mauriello
# testCode.sh
# Runs test cases on the given program


# run_tests() -- runs all tests
run_tests() {
    file=$1
    test_runs "$file"
    exit=$?
    if [[ $exit -eq 0 ]];then
        test_stdout "$file"
        test_return "$file"
        test_stderr "$file"
        test_valgrind "$file"
        test_memory "$file"
    fi
}

# test_runs() -- tests if the program will run
test_runs() {
    file=$1
    {
    timeout 2 ./"$program_name" < "$test_case_dir"/"$file" &> /dev/null
    } &> /dev/null
    ret=$?
    if [[ $ret -eq 124 ]]; then
        echo "*** testcase: $file [ FAILED - Timed out ]"
        return 1
    elif [[ $ret -gt 128 ]]; then
        echo "*** testcase: $file [ FAILED - Abnormal termination ]"
        return 1
    else
        return 0
    fi

}

# test_stdout() -- tests standard out
test_stdout() {
    file=$1
    status="NULL"
    ./"$program_name" < "$test_case_dir"/"$file" 1> myOut 2> myErr
    ./"$refex" < "$test_case_dir"/"$file" 1> exOut 2> exErr

    if diff -Z myOut exOut &> /dev/null; then
        status="PASSED"
    else
        status="FAILED"
    fi

    echo "*** testcase: $file [ stdout - $status ]"

}

# test_return() -- tests the return code
test_return() {
    file=$1
    status="NULL"
    ./"$program_name" < "$test_case_dir"/"$file" &> /dev/null
    myExit=$?
    ./"$refex" < "$test_case_dir"/"$file" &> /dev/null
    exExit=$?

    if [ "$myExit" -eq "$exExit" ];then
        status="PASSED"
    else
        status="FAILED"
    fi

    echo "*** testcase: $file [ return code - $status ]"
}

# test_stderr() -- tests the standard error
test_stderr() {
    file=$1
    status="NULL"
    ./"$program_name" < "$test_case_dir"/"$file" 2> myErr 1> /dev/null 
    ./"$refex" < "$test_case_dir"/"$file" 2> exErr 1> /dev/null 

    if [ -s myErr ] && [ -s exErr ]; then
        status="PASSED"
    elif [ ! -s myErr ] && [ ! -s exErr ]; then
        status="PASSED"
    else
        status="FAILED"
    fi

    echo "*** testcase: $file [ stderr - $status ]"
}

# test_valgrind() -- tests valgrind for 0 errors
test_valgrind() {
    file=$1
    status="NULL"
    valgrind ./"$program_name" < "$test_case_dir"/"$file" &> myOut



    if grep -q "ERROR SUMMARY: 0" myOut;then
        status="PASSED"
    else
        status="FAILED"
    fi

    echo "*** testcase: $file [ valgrind - $status ]"
}

# test_memory() -- tests that all memory has been freed
test_memory() {
    file=$1
    status="NULL"
    valgrind ./"$program_name" < "$test_case_dir"/"$file" &>myOut

    if grep -q "All heap blocks were freed -- no leaks are possible" myOut;then
        status="PASSED"
    else
        status="FAILED"
    fi

    echo "*** testcase: $file [ memory free - $status ]"
}

if [ $# -lt 1 ] || [ $# -gt 2 ];then
    echo "Invalid command line usage" >&2
    exit 1
fi

program_name=$1
test_case_dir=${2:-.}

if [ ! -x "$program_name" ]; then
    echo "Program cannot be executed" >&2
    exit 1
fi

if [ ! -d "$test_case_dir" ]; then
    echo "Test case directory not found" >&2
    exit 1
fi

refex="ex${program_name^}"

if [ ! -x "$refex" ]; then
    echo "Reference executable cannot be executed" >&2
    exit 1
fi

for test_case in "$test_case_dir"/test_"${program_name}"*;do
    test_case="${test_case#*/}"
    run_tests $test_case

    

    

done



exit 0



