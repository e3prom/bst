# Binary String Toolkit
## Summary
The Binary String Toolkit or BST for short is a rather simple utility to
convert binary strings to various formats suitable for inclusions in source
codes, such as those used to develop exploits and Proof of Concepts in the
security field.

## Features
 * Convert and dump binary files to standard output in a binary string format.
 * Convert a plain hexadecimal input string to an hex escaped version.
 * Generate a sequence of bad characters for detecting bad chars in buffers.
 * Limit the width of binary strings for a better readability of your exploit codes.
 * Format binary strings in your favorite programming or scripting language's syntax, such as:
   * C/C++
   * Python
   * PowerShell
 * Perform automatic variable block indentation ready for copy/paste inclusions.

## Dependencies
 * POSIX C Library
 * C Compiler
   * GCC
   * LLVM Clang
 * GNU Make
 * Git

## Building
To build and install the 'bstrings' binary on your system, simply do:
```
$ git clone https://github.com/e3prom/bst
$ cd bst
$ make
# by default, bstrings is installed in /usr/local/bin.
$ sudo make install
```

## Usage
The below example show how an assembled shellcode can be quickly dumped (-D)
to standard output in a hexadecimal escaped (-x) binary string of 16 hexadecimal
digits width (or 8 bytes), with Python syntax formatting and an indentation level
of 4 space characters:
```
$ bstrings --verbose -x -D lnx-execve-setreuid-x86_64 -w8 -i 4 --syntax=python
[*] Convert hexadecimal input to an escaped binary string.
[+] Binary string width is limited to 8 bytes.
[+] Output binary string using python language syntax.
[+] Indentation level set to 4 space character(s).
    shellcode =  ""
    shellcode += "\x31\xc0\x48\x89\xc7\x48\x89\xc6"
    shellcode += "\x48\x89\xc2\xb0\x71\x0f\x05\x31"
    shellcode += "\xc0\x50\x48\xbb\x2f\x62\x69\x6e"
    shellcode += "\x2f\x2f\x73\x68\x53\x48\x89\xe7"
    shellcode += "\x50\x48\x89\xe6\x57\x48\x89\xe2"
    shellcode += "\xb0\x3b\x0f\x05"
```

You can also use bstrings to output an automatically indented bad character
sequence, and thus in your favorite programming language:
```
$ bstrings --verbose -b -w12 -i 4 --syntax=c -n badchar
[*] Generating bad character binary string.
[+] Binary string width is limited to 12 bytes.
    unsigned char badchar[] =
        "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c"
        "\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18"
        "\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24"
        "\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30"
        "\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c"
        "\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48"
        "\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54"
        "\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60"
        "\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c"
        "\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78"
        "\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84"
        "\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90"
        "\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c"
        "\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8"
        "\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4"
        "\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf\xc0"
        "\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc"
        "\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8"
        "\xd9\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4"
        "\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0"
        "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc"
        "\xfd\xfe\xff"
```

For a list of supported command-line options, simply execute bstrings with the ```--help``` switch:
```
$ bstrings --help
Usage: bstrings [OPTION]...
 Convert input to specified binary string format.

 At least one of the below options must be given:
    -D, --dump-file=FILE    Convert and Dump RAW content of file FILE
    -x, --hex-escape        Escape input hexadecimal string
    -b, --gen-badchar       Generate a bad character sequence string
 
 The below switches are optional:
    -f, --file=FILE         Read hex input from file FILE instead of stdin
    -w, --width=BYTES       Break binary strings to specified length in bytes
    -s, --syntax=LANG       Output variable using syntax of LANG
    -i, --indent=LENGTH     Perform indentation for given character length
    -n, --var-name=VAR      Specify string variable name (verbose mode)
    -h, --help              Display this help
       --interactive        Enter interactive mode
       --verbose            Enable verbose output
       --version            Print version information

 The below languages are supported (case-sensitive):
    C                       C Programming Language
    python                  Python Programming Language
    powershell              PowerShell Automation and Scripting Language

```

## Contribution
Feel free to contribute to this project by submitting your codes and by reporting issues and bugs

## Software License
This software is licensed under the terms of the GNU General Public License.
