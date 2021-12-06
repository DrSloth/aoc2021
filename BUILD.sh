#!/bin/sh

set -xe

gcc ./$1 -o ./target/test -std=c99 -O2 -m64 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual \
    -Wstrict-prototypes -Wmissing-prototypes
