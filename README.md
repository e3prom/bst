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
![binary string toolkit
example](https://github.com/e3prom/bst/raw/master/docs/examples/bstrings.gif)

The below example show how an assembled shellcode can be quickly dumped (-D) to
standard output in a hexadecimal escaped (-x) binary string of 8 hex digits
width (or 4 bytes):
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

For a list of supported command-line options, simply consult the command's
help:
```
$ bstrings --help
[...]
```

## Software License
This software is licensed under the terms of the GNU General Public License.
