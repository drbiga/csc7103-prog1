#!/usr/bin/bash

g++ src/server/*.cpp src/server/command/*.cpp src/server.cpp -o server.bin \
    && ./server.bin
