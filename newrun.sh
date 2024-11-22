#!/bin/bash

# Set paths to libcurl
CURL_INCLUDE_DIR=/usr/include/x86_64-linux-gnu/curl/
CURL_LIB_DIR=/usr/local/lib/libcurl.so
# Compile the program
gcc -I $CURL_INCLUDE_DIR -L $CURL_LIB_DIR main.c cJSON.c data.c -o main -lcurl 
./main
