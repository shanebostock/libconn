#!/bin/bash

#Compile libconn
echo Compiling libconn

LIBCONN_PATH=./src

g++ -c -Wall -Werror -fpic $LIBCONN_PATH/connection.cpp 
g++ -shared -o libconn.so connection.o

echo complete