#!/usr/bin/bash

g++ src/client/*.cpp src/client.cpp -o client.bin \
    && ./client.bin
