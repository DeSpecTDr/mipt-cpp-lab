#!/usr/bin/env bash

# stdin=$(cat)
path=$1
file="${path##*/}"
# echo "$file"
# exit
# ext="${file##*.}"
name="${file%.*}"

mkdir -p build
o="./build/$name"
# shift
g++ -o "$o" -lm -Wall -Winvalid-pch -O2 -std=c++20 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS -ftrapv -fstrict-flex-arrays=3 -fstack-clash-protection -fstack-protector-strong -fcf-protection=full -I/home/user/Projects/cpp/pch/ "$path"
if [ "$2" = "v" ]; then
time valgrind -q "$o"
else
$o
fi
