#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <filename.c>"
    exit 1
fi

filename=$1
output_filename="junktemp.out"

gcc -Wall -o "$output_filename" "$filename" 2> error_log.txt

    error_count=$(grep -o 'error' error_log.txt | wc -l)
    warning_count=$(grep -o 'warning' error_log.txt | wc -l)

    #echo "$error_count $warning_count"


rm -f error_log.txt
rm -f junktemp.out
