#!/bin/bash

#Compiling Test

echo Compiling Test
g++ -Wall -Werror test.cpp -lrt -pthread -lconn -o test

echo complete