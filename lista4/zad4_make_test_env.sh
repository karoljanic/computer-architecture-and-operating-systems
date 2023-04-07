#!/bin/bash

rm -r test_env

mkdir test_env
mkdir test_env/subdirectory

head -c 1M /dev/urandom > test_env/file1.bin 
head -c 2M /dev/urandom > test_env/file2.bin 
head -c 3M /dev/urandom > test_env/file3.bin 
head -c 4M /dev/urandom > test_env/file4.bin 
head -c 5M /dev/urandom > test_env/file5.bin 
head -c 6M /dev/urandom > test_env/subdirectory/file6.bin 
touch test_env/empty.bin
touch test_env/subdirectory/empty.bin 

cp test_env/file1.bin test_env/file1copy.bin 
cp test_env/file2.bin test_env/subdirectory/file2copy.bin 
cp test_env/file3.bin test_env/file3copy.bin 
cp test_env/file3.bin test_env/subdirectory/file3copycopy.bin
