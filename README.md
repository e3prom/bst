# Binary String Toolkit
## Summary
The Binary String Toolkit or BST for short is a rather simple utility to
convert binary strings to various formats suitable for later inclusions in
source codes such as those used to develop exploits in the security field.

## Features
 * Dump file in a copy/paste friendly binary string format.
 * Convert a plain hexadecimal input to an escaped binary string.
 * Output a "bad characters" sequence in a hexadecimal escaped binary string.
 * Limit output binary strings to specified width for easier readability.

## Dependencies
 * POSIX C Library
 * C Compiler (GCC, clang)
 * GNU Make
 * Git

## Building
To build the 'bstrings' binary, simply invoke make:
```
$ make
# by default, install bstrings to /usr/local/bin
$ make install
```

## Running
The below example show how an assembled shellcode can be easily and quickly
dumped (-D) to standard output in a hexadecimal escaped (-x) binary string of
8 hexadecimal digits width (4 bytes):
```
$ bstrings -x -D lnx-execve-setreuid-x86_32 -w4
\x31\xc0\x89\xc3
\x89\xc2\x89\xc1
\xb0\x46\xcd\x80
\x31\xc0\x50\x68
\x2f\x2f\x73\x68
\x68\x2f\x62\x69
\x6e\x89\xe3\x50
\x89\xe2\x53\x89
\xe1\xb0\x0b\xcd
\x80
```

For a list of supported command-line options and switches, simply consult the
command's help:
```
$ bstrings --help
[...]
```

## Software License
This software is licensed under the terms of GNU General Public License.
