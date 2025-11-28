#!/bin/bash

# Script to check whether a number is prime

read -p "Enter a number: " num

# Negative numbers, 0 and 1 are not prime
if [ "$num" -le 1 ]; then
    echo "$num is NOT a prime number."
    exit 0
fi # marks the ending of ifelse block

# Check divisibility
is_prime=1

for (( i=2; i*i<=num; i++ ))
do
    if (( num % i == 0 )); then
        is_prime=0
        break
    fi
done

if [ $is_prime -eq 1 ]; then
    echo "$num IS a prime number."
else
    echo "$num is NOT a prime number."
fi