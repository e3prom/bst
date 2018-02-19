/* vi:set tw=78 ts=8 sw=4 sts=4 et:
 *
 * Binary String Toolkit
 *
 * Copyright (C) 2018 Nicolas Chabbey
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/*
 * bstrings.c - main program source file
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "include/bool.h"
#include "include/version.h"

#define BADCHAR_HEX_SEQLEN  510     /* badchar hex digits sequence length */

/* declare the 'verbose_flag' global integer */
static int verbose_flag;
/* declare the 'interactive_flag' global integer */
static int interactive_flag;

static void print_usage(FILE *stream, char *program_name)
{
    fprintf(stream, "Usage: %s [OPTION]...\n", program_name);
    fprintf(stream, " Convert input to specified binary string format.\n\n");
    fprintf(stream, " At least one of the below switches must be given:\n\
    -x, --hex-escape    Convert hexadecimal input to escaped binary string\n\
    -b, --gen-badchar   Generate a bad character sequence string\n\
    \n");
    fprintf(stream, " The below switches are optional:\n\
    -w, --width=bytes   Break binary strings to specified length in bytes\n\
    -h, --help          Display this help\n\
       --interactive    Enter interactive mode\n\
       --verbose        Enable verbose output\n\
       --version        Print version information\n\
    \n");
}

static void print_version(FILE *stream, char *program_name)
{
    fprintf(stream, "Binary String Toolkit %s\n", program_version);
    if(program_build != NULL) {
    fprintf(stream, "Build version %s, compiled the %s\n", program_build,
            program_build_time);
    }
    fprintf(stream, "Copyright (C) 2018 Nicolas Chabbey\n");
    fprintf(stream, "This program is free software: you can redistribute it "
                    "and/or modify it\nunder the terms of the GNU General "
                    "Public License as published by the\nFree Software "
                    "Foundation, either version 2 of the License, or \n"
                    "(at your option) any later version.\n"
                    "This program has absolutely no warranty.\n");
    fprintf(stream, "For help enter \"%s --help\"\n", program_name);
}

void output_hex_escaped_string(char *ptr_char_array, int *array_size,
                               int string_width)
{
    /* declare integer i and c */
    int i, c;

    /* initialize the hex escaped character array index to zero.
     * it keep track of the number of hex digits present in the binary string.
     */
    int ai = 0;

    /* initialize integer 'invalidhexchar' to be used as a counter. */
    int invalidhexchar = 0;

    /* if interactive flag set, start the binary string on a new line */
    if (interactive_flag)
        putchar('\n');

    /* for every character of the character array 'char_array'
    * loop through the body until we reach the end of the array.
    */
    for (i = 0; i < *array_size; i++) {
        c = ptr_char_array[i];

        /* filter out any characters outside of the hexadecimal ASCII character
         * range. The below conditional statement may need some improvement.
         */
        if ((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102)) {
            /* if the hex escaped char array index is divible by two, we've a
             * pair of hexadecimal characters (or byte), we need to escape
             * using the binary string escape characters '\' and 'x'.
             */
            if (ai % 2 == 0) {
                /* if string_width is non-default */
                if (string_width != 0) {
                    /* ensure integer 'ai' is non-zero so we don't insert a
                     * new line character in the first row of output. put a
                     * new line character every string_width's byte value.
                     */
                    if ((ai != 0) && (ai % (string_width*2) == 0)) {
                        putchar('\n');
                    }
                }
                putchar('\\');
                putchar('x');
                putchar(c);
            } else {
                putchar(c);
            }

            /* Increase the hex escaped character array index so we keep track
             * of the pair of hexadecimal byte inside the binary string.
             */
            ai++;

        /* catches all non-hexadcimal characters, excepted the end-of-file,
         * the new line and the null characters respectively.
         */
        } else if ((c != EOF) && (c != 10) && (c != 0)) {
            invalidhexchar++;
        }
    }

    /* put newline character after the binary string */
    putchar('\n');

    if ((verbose_flag == true) && (invalidhexchar > 0)) {
        fprintf(stdout, "[-] Warning: %d non-hexadecimal character(s) "
                        "detected in input.\n", invalidhexchar);
    }
}

char * allocate_dynamic_memory(int alloc_size)
{
    /* use malloc() to allocate dynamic memory and then return to the caller
     * function the memory location allocated on the heap.
     */
    char *ptr = malloc(alloc_size);

    /* error handling: on errors malloc() returns NULL. */
    if (ptr == NULL) {
        printf("%d byte(s) memory allocation error.", alloc_size);
        exit(EXIT_FAILURE);
    }

    return ptr;
}

char * change_dynamic_memory(char *ptr, int new_size) {
    /* call to realloc() to change the size of the memory block pointed to by
     * the pointer 'ptr' with the new size value in 'new_size'.
     */
    char *new_ptr = realloc(ptr, new_size);

    /* error handling: on errors realloc() returns NULL. */
    if (new_ptr == NULL) {
        printf("%d byte(s) memory re-allocation error.", new_size);
        exit(EXIT_FAILURE);
    }

    return new_ptr;
}

char * generate_badchar_sequence()
{
    /* declare integer i */
    int i;

    /* initialize bad char. character array pointer by calling
     * allocate_dynamic_memory() function with a fixed allocation size of 510
     * bytes, the latter being the length we need to hold all hex digits.
    */
    char *ptr_badchar_array = allocate_dynamic_memory(sizeof(char) *
                                                      BADCHAR_HEX_SEQLEN);

    /* initialize length integer */
    int length = 0;

    /* simple linear hex digits generator */
    for (i = 1; i < 256; i++) {
        /* leveraging sprintf() to store hexadecimal digits to the character
         * array. -- To check for buffer overflow condition.
         */
        length += sprintf(ptr_badchar_array+length, "%02x", i);
    }

    /* return the character array pointer to the caller function */
    return ptr_badchar_array;
}

char * read_and_store_char_input(int *array_size)
{
    /* declare integer 'c' which will hold input character. */
    int c;
    /* initialize integer 'i' which will be used as an array index. */
    int i = 0;

    /* if in interactive mode */
    if (interactive_flag)
        printf("[+] Hit CTRL-D twice to terminate input.\n");

    /* initialize character array pointer 'ptr_char_array' by calling
     * allocate_dynamic_memory() function.
     */
    char *ptr_char_array = allocate_dynamic_memory(sizeof(char));

    /* store each input character into the character array 'ptr_char_array'
     * until we reach EOF.
     */
    while ((c = getchar()) != EOF) {
        ptr_char_array[i] = (char)c;
        ptr_char_array = change_dynamic_memory(ptr_char_array, sizeof(char) *
                                              (*array_size+=1));
        i++;
    }

    /* return a pointer to caller function */
    return ptr_char_array;
}

int main(int argc, char *argv[])
{
    /* initialize all variables needed for command-line options handling using
     * the GNU C Library's getopt_long() function.
     */
    int opt;

    /* initialize program's options flags */
    bool doOutputHexEscapedString, doOutputBadCharString,
         doLimitBinaryStringWidth = false;

    /* initialite string_width to the default value of zero. */
    int string_width = 0;

    /* getopt_long()'s long_options struct */
    static struct option long_options[] = {
        /* verbosity flags */
        {"verbose",     no_argument,    &verbose_flag, 1},
        {"quiet",       no_argument,    &verbose_flag, 0},
        {"interactive", no_argument,    &interactive_flag, 1},
        /* program actions */
        {"hex-escape",  no_argument,    NULL, 'x'},
        {"gen-badchar", no_argument,    NULL, 'b'},
        /* program options */
        {"width",       required_argument,  NULL, 'w'},
        /* version option */
        {"version",     no_argument,    NULL, '@'},
        /* help option */
        {"help",        no_argument,    NULL, 'h'},
        {0, 0, 0, 0}
    };

    /* using getopt_long() from GNU C library to parse command-line options. */
    while ((opt = getopt_long(argc, argv, ":hvxbw::",
                              long_options, NULL)) != -1) {
        switch (opt) {
            /* handle getopt_long() return values */
            case 0:     /* getopt_long() set a flag, keep going */
                break;
            case 1:     /* getopt_long() points at a command-line argument */
                break;
            case ':':   /* an argument option is missing */
                fprintf(stderr, "%s: option `-%c' require an argument.\n",
                        argv[0], optopt);
                exit(EXIT_FAILURE);
            case '?':   /* invalid option given */
            case 'h':   /* user ask for help */
            default:
                print_usage(stderr, argv[0]);
                exit(EXIT_FAILURE);
            case '@':
                print_version(stderr, argv[0]);
                exit(EXIT_SUCCESS);
            /* program's options */
            case 'v': verbose_flag = 1; break;
            case 'x': doOutputHexEscapedString = true; break;
            case 'b': doOutputBadCharString = true; break;
            case 'w':   /* binary string width option */
                doLimitBinaryStringWidth = true;
                /* make sure optarg is non-null before performing operation */
                if (optarg != NULL) {
                    /* using atoi() from the GNU C library to read the binary
                     * string width from the command-line -w|--width option
                     * argument.
                     */
                    string_width = atoi(optarg);
                }
                break;
        }
    }

    /* simple conditional check to ensure at least a valid option is given at
     * the command-line, otherwise call print_usage() function.
     */
    if ((optind < argc) || argc == 1) {
        print_usage(stdout, argv[0]);
        exit(EXIT_SUCCESS);
    }

    /* if -x|--hex-escape option is given */
    if (doOutputHexEscapedString == true) {
        /* initialize integer 'array_size' */
        int array_size = 1;
        /* toggle verbosity if flag set */
        if (verbose_flag == true) {
            printf("[*] Convert hexadecimal input to a hex escaped binary string.\n");
            if (doLimitBinaryStringWidth == true) {
                printf("[+] Binary string width is limited to %d bytes.\n",
                       string_width);
            }
        }
        /* call to read_and_store_char_input() */
        char *ptr_char_array = read_and_store_char_input(&array_size);
        /* call to output_hex_escaped_string() */
        output_hex_escaped_string(ptr_char_array, &array_size, string_width);
    }

    /* if -b|--gen-badchar option is given */
    if (doOutputBadCharString == true) {
        /* initialize integer 'array_size' to BADCHAR_HEX_SEQLEN bytes */
        int array_size = BADCHAR_HEX_SEQLEN;
        /* toggle verbosity if flag set */
        if (verbose_flag == true) {
            printf("[*] Generate bad character binary string:\n");
            if (doLimitBinaryStringWidth == true) {
                printf("[+] Binary string width is limited to %d bytes.\n",
                       string_width);
            }
        }
        /* call to generate_badchar_sequence() */
        char *ptr_char_array = generate_badchar_sequence();
        /* call to output_hex_escaped_string() */
        output_hex_escaped_string(ptr_char_array, &array_size, string_width);
    }

    return 0;
}
