# Binary String Toolkit
## Summary
The Binary String Toolkit or BST for short is a rather simple utility to
convert binary strings to various formats suitable for later inclusions in
source codes such as those used to develop exploits in the security field.

## Features
 * Dump file in copy/paste friendly binary string format.
 * Convert a plain hexadecimal input to an escaped binary string.
 * Output a "bad characters" sequence in a hexadecimal escaped binary string.
 * Limit binary strings to specified width for better readability in source codes.

## Dependencies
 * POSIX C Library
 * GNU Make
 * Git

## Building
To build the 'bstrings' binary, simply invoke make:
```
$ make
# install bstrings to /usr/local/bin
$ make install
```

## Running
The below example show how an assembled shellcode can be easily and quickly
dumped (-D) to standard output in a hexadecimal escaped (-x) binary string of 8
hexadecimal digits (or 4 bytes) width (-w):
```
./bstrings -x -D lnx-execve-setreuid-x86_64 -w4
\x31\xC0\x48\x89
\xC7\x48\x89\xC6
\x48\x89\xC2\xB0
\x71\x0F\x05\x31
\xC0\x50\x48\xBB
\x2F\x62\x69\x6E
\x2F\x2F\x73\x68
\x53\x48\x89\xE7
\x50\x48\x89\xE6
\x57\x48\x89\xE2
\xB0\x3B\x0F\x05
```

For a list of supported commands and the description of all available
command-line switches, simply consult the command's help:
```
$ bstrings --help
[...]
```

## Software License
This software is licensed under the terms of GNU General Public License.
