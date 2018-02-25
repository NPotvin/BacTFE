#!/bin/bash

executions=16
threads=4
sortFile="array.txt"

echo "Execution of C++"
# TMP=$(date +%s%3N)
./Cpp/out/mergeSort -f $sortFile -m $threads -e $executions
# TMP=$(( $(date +%s%3N) - $TMP ))
# echo "Total time spent (calculated from outside the program)" $TMP "ms"
# echo "Time per execution spent (calculated from outside the program)" $(( $TMP / $executions )) "ms"
echo ""

echo "Execution of C++"
# TMP=$(date +%s%3N)
./Cpp/out/mergeSort -f $sortFile -s -e $executions
# TMP=$(( $(date +%s%3N) - $TMP ))
# echo "Total time spent (calculated from outside the program)" $TMP "ms"
# echo "Time per execution spent (calculated from outside the program)" $(( $TMP / $executions )) "ms"
echo ""

echo "Execution of Java"
# TMP=$(date +%s%3N)
java -cp Java/class:Java/commons-cli-1.4/* MergeSortMain -f $sortFile -m $threads -e $executions
# TMP=$(( $(date +%s%3N) - $TMP ))
# echo "Total time spent (calculated from outside the program)" $TMP "ms"
# echo "Time per execution spent (calculated from outside the program)" $(( $TMP / $executions )) "ms"
echo ""

echo "Execution of Java"
# TMP=$(date +%s%3N)
java -cp Java/class:Java/commons-cli-1.4/* MergeSortMain -f $sortFile -s -e $executions
# TMP=$(( $(date +%s%3N) - $TMP ))
# echo "Total time spent (calculated from outside the program)" $TMP "ms"
# echo "Time per execution spent (calculated from outside the program)" $(( $TMP / $executions )) "ms"
echo ""
