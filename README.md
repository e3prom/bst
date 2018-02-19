# Binary String Toolkit
## Introduction
The Binary String Toolkit or BST for short is a rather simple utility to
convert binary strings into various formats suitable for later inclusions in
source codes such as those used to develop exploits in the security field.

## Features
 * Convert an hexadecimal input to an escaped binary string.
 * Output a "bad characters" sequence in a hexadecimal escaped binary string.
 * Limit binary strings width for better readability in source codes.

## Dependencies
 * POSIX C Library
 * GNU Make
 * Git

## Building
To build the main 'bstrings' binary, simply invoke make:
```
$ make
```

## Running
The below example simply shows how an assembled shellcode can be easily send to
'bstrings' to generate an escaped string of 8 hexadecimal digits width:
```
$ xxd -ps lnx-execve-setreuid-x86_64 | bstrings -x -w4
\x31\xc0\x48\x89
\xc7\x48\x89\xc6
\x48\x89\xc2\xb0
\x71\x0f\x05\x31
\xc0\x50\x48\xbb
\x2f\x62\x69\x6e
\x2f\x2f\x73\x68
\x53\x48\x89\xe7
\x50\x48\x89\xe6
\x57\x48\x89\xe2
\xb0\x3b\x0f\x05
```

For more commands and a description of all available command-line switches,
simply consult the command's help:
```
$ bstrings --help
```

## Software License
This project is licensed under the terms of GNU General Public License.
