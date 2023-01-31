#!/bin/bash

INCLUDES_PATH=/usr/local/include
LIBS_PATH=/usr/local/lib
CONF_PATH=/etc/ld.so.conf.d
LIBCONN_PATH=./src

echo building libconn...
bash build.sh
echo complete.

echo installing libconn...
echo copying incldues to $INCLUDES_PATH...
sudo cp $LIBCONN_PATH/*.h $INCLUDES_PATH
sudo cp libconn.so $LIBS_PATH

echo setting up linker for libcon
sudo cp libconn.conf $CONF_PATH
sudo ldconfig

echo cleaning up...
rm *.so
rm *.o
echo complete.