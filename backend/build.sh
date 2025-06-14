#!/bin/bash
set -e

# Install Python dependencies
pip install -r requirements.txt


# Build C++ AES binary
mkdir -p build
g++ -o build/AES ../AES/AES.cpp
chmod +x build/AES
