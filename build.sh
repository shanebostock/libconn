#!/bin/bash

#Compile libconn
echo Compiling libconn

LIBCONN_PATH=./src

gcc -c -Wall -Werror -fpic $LIBCONN_PATH/helperfuncs.c $LIBCONN_PATH/listener.c $LIBCONN_PATH/talker.c
gcc -shared -o libconn.so helperfuncs.o listener.o talker.o

echo complete