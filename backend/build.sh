#!/bin/bash
set -e

# Install Python dependencies
python -m venv venv
source venv/bin/activate
python -m pip install -r requirements.txt

# Build C++ AES binary
mkdir -p build
g++ -o build/AES ../AES/AES.cpp
chmod +x build/AES
