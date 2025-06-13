#!/bin/bash

# Install Python dependencies
pip install -r requirements.txt

# Compile the C++ AES binary
mkdir -p build
g++ -o build/AES AES/AES.cpp
