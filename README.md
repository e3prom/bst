# Binary String Toolkit
## Summary
The Binary String Toolkit or BST for short is a rather simple utility to
convert binary strings to various formats suitable for later inclusions in
source codes such as those used to develop exploits in the security field.

## Features
 * Dump files content directly to a terminal in a binary string format.
 * Convert a plain hexadecimal input to an escaped binary string.
 * Output a "bad character" sequence in a hexadecimal escaped binary string.
 * Limit output binary string width for easier readability.
 * Format output in your favorite programming language for quick copy&paste
   inclusions in source codes.
 * Perform indentation of the binary string output.

## Dependencies
 * POSIX C Library
 * C Compiler
   * GCC
   * Clang
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
The below example show how an assembled shellcode can be quickly dumped (-D) to
standard output in a hexadecimal escaped (-x) binary string of 8 hex digits
width (or 4 bytes), with python syntax formatting:
```
$ bstrings -D lnx-execve-setreuid-x86_64 -x --syntax=python -w8
buffer += "\x31\xc0\x48\x89\xc7\x48\x89\xc6"
buffer += "\x48\x89\xc2\xb0\x71\x0f\x05\x31"
buffer += "\xc0\x50\x48\xbb\x2f\x62\x69\x6e"
buffer += "\x2f\x2f\x73\x68\x53\x48\x89\xe7"
buffer += "\x50\x48\x89\xe6\x57\x48\x89\xe2"
buffer += "\xb0\x3b\x0f\x05"
```

For a list of supported command-line options, simply consult the command's
help:
```
$ bstrings --help
[...]
```

## Software License
This software is licensed under the terms of the GNU General Public License.
