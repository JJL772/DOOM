#!/bin/bash

files=`find . -iname "*.c"`
for file in $files; do
	clang-format -i $file
done
