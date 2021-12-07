# AOC 2021 solutions

## Building

## Utility
The utility includes data structure and utility functions that are created 
over the course of this month for the solution of the problems.

## Day 01

### Problem 1
Given an array `arr` with integer elements find the count the number of times that 
the check `a[n] > a[n+1]` holds true.

### Problem 2
Given an array `arr` with integer elements find the count the number of times that 
the check `a[n] + a[n+1] + a[n+2] > a[n+1] + a[n+2] + a[n+3]` holds true.

## Day 02

### Problem 1
Given an array of unary instructions which can mutate a 'depth' value and increment a 'horizontal'
value. Calculate the depth and horizontal values after executing the instructions. 

### Problem 2
Similiar to Problem 1 but the instructions function slightly differently

### Day 03

### Problem 1
Given an array of binary numbers, create a number consisting of the most common bits for each
individual bit index. Calculate the product of the generated number and its inverse.

### Problem 2
Given an array of binary numbers, iterate each column of bits, reduce the array as long as not
all bits at the index are equally common. Then keep only the first number containing 1 in the idx.
Do the same again, but search for least common and keep number with 0 at the end.
