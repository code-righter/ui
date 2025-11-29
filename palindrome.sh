#!/bin/bash

# Script to check whether a string is a palindrome

read -p "Enter a string: " str

# Remove spaces 
cleaned=$(echo "$str" | tr -d ' ' )

# Reverse the string
rev_str=$(echo "$cleaned" | rev)

# Compare original and reversed strings
if [ "$cleaned" = "$rev_str" ]; then
    echo "\"$str\" is a palindrome."
else
    echo "\"$str\" is NOT a palindrome."
fi