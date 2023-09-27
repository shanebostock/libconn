#!/bin/bash

#Compiling Test

echo Compiling Test
gcc -Wall -Werror test.c -lrt -pthread -lconn -o test

echo complete