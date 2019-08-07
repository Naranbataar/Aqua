# Aqua
[![CodeFactor](https://www.codefactor.io/repository/github/naranbataar/aqua/badge)](https://www.codefactor.io/repository/github/naranbataar/aqua)
[![License: Unlicense](https://img.shields.io/badge/license-Unlicense-blue.svg)](http://unlicense.org/)

A minimal tool that implements Reed-Solomon algorithm to protect files against corruption

## Installation
You need just a C compiler with the default libraries, and `make`
```shell
# Compiles to `aqua`
make
# Optional, copy the binary to /usr/local/bin
sudo make install
```

## Example
```shell
# Protect the file (defaults to 10 symbols per 256 bytes)
cat file | aqua protect > file2
# Corrupts 5 bytes of the first part of the file
dd if=/dev/urandom of=file2 bs=1 count=5 seek=0 conv=notrunc 
# Corrupts 5 bytes of the another part of the file
dd if=/dev/urandom of=file2 bs=1 count=5 seek=1000 conv=notrunc
# File can still be fully recovered
cat file2 | aqua purify
```
