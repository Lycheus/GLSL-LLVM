#!/bin/bash
#g++ -pipe -Wall -W -O2 -DQT_NO_DEBUG -I/usr/lib64/qt-3.3/mkspecs/default -I. -I/usr/lib64/qt-3.3/include -o shm_client shm_client.cpp
g++ -pipe -Wall -W -O2 -DQT_NO_DEBUG -I/usr/lib/qt-3.3/mkspecs/default -I. -I. -I/usr/lib/qt-3.3/include -o shm_client shm_client.cpp
make
