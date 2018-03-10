# Binary String Toolkit
## Summary
The Binary String Toolkit or BST for short is a rather simple utility to
convert binary strings to various formats suitable for later inclusions in
source codes such as those used to develop exploits in the security field.

## Features
 * Dump files content to standard output in a binary string format.
 * Convert a plain hexadecimal input to an escaped binary string.
 * Output a bad character sequence in various format for inclusion in exploit
 codes.
 * Limit output binary string width for a better readability in source codes.
 * Format output in your favorite programming language syntax.
 * Perform binary string variable block indentation.

## Dependencies
 * POSIX C Library
 * C Compiler
   * GCC
   * LLVM Clang
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
The below example show how an assembled shellcode can be quickly dumped (-D)
to standard output in a hexadecimal escaped (-x) binary string of 16 hexadecimal
digits width (8 bytes), with python syntax formatting and an indentation of 4
space characters:
```
$ bstrings --verbose -x -D lnx-execve-setreuid-x86_64 --syntax=python -w8 -n shellcode --indent=4
[*] Convert hexadecimal input to an escaped binary string.
[+] Binary string width is limited to 8 bytes.
[+] Output binary string for python language syntax.
[+] Indentation level set to 4 character(s).
    shellcode =  ""
    shellcode += "\x31\xc0\x48\x89\xc7\x48\x89\xc6"
    shellcode += "\x48\x89\xc2\xb0\x71\x0f\x05\x31"
    shellcode += "\xc0\x50\x48\xbb\x2f\x62\x69\x6e"
    shellcode += "\x2f\x2f\x73\x68\x53\x48\x89\xe7"
    shellcode += "\x50\x48\x89\xe6\x57\x48\x89\xe2"
    shellcode += "\xb0\x3b\x0f\x05"
```

For a list of supported command-line options, simply consult the command's
help:
```
$ bstrings --help
[...]
```

## Software License
This software is licensed under the terms of the GNU General Public License.
