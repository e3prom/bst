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
#include <string.h>
#include "include/bool.h"
#include "include/version.h"

#define BADCHAR_HEX_SEQLEN  511     /* badchar hex digits sequence length */
#define MAX_FILENAME_LENGTH 512     /* max filename length on filesystems */
#define MAX_ARGUMENT_LENGTH 255     /* max length of option's argument */


/* declare the 'verbose_flag' global integer */
static int verbose_flag;
/* declare the 'interactive_flag' global integer */
static int interactive_flag;

static void print_usage(FILE *stream, char *program_name)
{
    fprintf(stream, "Usage: %s [OPTION]...\n", program_name);
    fprintf(stream, " Convert input to specified binary string format.\n\n");
    fprintf(stream, " At least one of the below options must be given:\n\
    -D, --dump-file=FILE    Dump content of file FILE in hexadecimal format\n\
    -x, --hex-escape        Escape input hexadecimal string\n\
    -b, --gen-badchar       Generate a bad character sequence string\n\
    \n");
    fprintf(stream, " The below switches are optional:\n\
    -f, --file=FILE         Read input from file FILE instead of stdin\n\
    -w, --width=BYTES       Break binary strings to specified length in bytes\n\
    -s, --syntax=LANG       Output variable using language syntax of LANG\n\
    -i, --indent=LENGTH     Perform indentation for given character length\n\
    -n, --var-name=VAR      Specify binary string variable name (verbose)\n\
    -h, --help              Display this help\n\
       --interactive        Enter interactive mode\n\
       --verbose            Enable verbose output\n\
       --version            Print version information\n\
    \n");
    fprintf(stream, " The below languages are supported (case-sensitive):\n\
    C                       C Programming language\n\
    python                  Python Programming language\n\
    \n");
}

static void print_version(FILE *stream, char *program_name)
{
    /* version & copyright */
    fprintf(stream, "Binary String Toolkit (%s)\n", program_version);
    fprintf(stream, "Copyright (C) 2018 Nicolas Chabbey\n");
    /* license statement */
    fprintf(stream, "This program is free software: you can redistribute it "
                    "and/or modify it\nunder the terms of the GNU General "
                    "Public License as published by the\nFree Software "
                    "Foundation, either version 2 of the License, or \n"
                    "(at your option) any later version.\n"
                    "This program has absolutely no warranty.\n");
    /* source code, bug reporting & documentation */
    fprintf(stream, "Source code, bug reporting and documentation available "
                    "at:\n<https://github.com/e3prom/bst>\n");
    /* build & build time */
    if(program_build != NULL) {
    fprintf(stream, "Git Build %s\nCompiled on %s\n", program_build,
            program_build_time);
    }
    /* help */
    fprintf(stream, "For help enter \"%s --help\"\n", program_name);
}

/* struct 'bstring' definition */
struct bstring {
    /* declare pointer to character array 'ptr_char_array' */
    char *ptr_char_array;
    /* declare pointer to integer 'ptr_array_size' */
    int *ptr_array_size;
    /* declare integer 'output_lang' */
    int output_lang;
    /* declare integer 'string_width' */
    int string_width;
    /* declare integer 'indent_width' */
    int indent_width;
    /* declare pointer to character array 'ptr_var_name' */
    char *ptr_var_name;
};

void output_hex_escaped_string(struct bstring *ptr_bstr)
{
    /* declare integer i, c and ic */
    int i, c, ic;

    /* initialize the hex escaped character array index to zero.
     * it keep track of the number of hex digits present in the binary string.
     */
    int ai = 0;

    /* initialize local integer 'indent_width' to the value of the
     * 'indent_width' integer in struct pointed by 'ptr_bstr' pointer.
     */
    int indent_width = ptr_bstr->indent_width;

    /* initialize integer 'invalidhexchar' to be used as a counter. */
    int invalidhexchar = 0;

    /* if interactive flag set, start the binary string on a new line */
    if (interactive_flag)
        putchar('\n');

    /* if verbose flag set, perform indentation and output variable name */
    if (verbose_flag) {
        /* indentation loop */
        if (indent_width > 0) {
            for (ic = 0; ic < indent_width; ic++) {
                /* put space character (decimal: 32) */
                putchar(32);
            }
        }
        /* print variable name matching specified language */
        switch (ptr_bstr->output_lang) {
            case 1:
                if (ptr_bstr->ptr_var_name != NULL)
                    printf("unsigned char %s[] =\n", ptr_bstr->ptr_var_name);
                else
                    printf("unsigned char buffer[] =\n");
                break;
            case 2:
                if (ptr_bstr->ptr_var_name != NULL)
                    printf("%s =  \"\"\n", ptr_bstr->ptr_var_name);
                else
                    printf("buffer =  \"\"\n");
                break;
        }
    }

    /* for every character of the character array 'char_array'
     * loop through the body until we reach the end of the array.
     * Dont't forget to account for the extra byte of the array_size's size in
     * order to only print characters within the input string.
     */
    for (i = 0; i < *ptr_bstr->ptr_array_size-1; i++) {
        /* initialize c to the i(th) element of the array */
        c = ptr_bstr->ptr_char_array[i];

        /* filter out any characters outside of the hexadecimal ASCII character
         * range.
         */
        switch (c) {
            /* if the character is within the hexadecimal characters range.
             * ranges within switch case's constants are supported by GCC.
             */
            case 48 ... 57:         // 0-9
            case 65 ... 70:         // A-F
            case 97 ... 102:        // a-f
                /* if the hex escaped char array index is divible by two,
                 * we've pair of hexadecimal characters (or byte), we need to
                 * escape using the binary string escape characters '\' and
                 * 'x' respectively.
                 */
                if (ai % 2 == 0) {
                    /* if string_width is non-default or specified. */
                    if (ptr_bstr->string_width != 0) {
                        /* if true, we are at the end of the line.
                         * ensure integer 'ai' is non-zero so we don't insert
                         * a new line character in the first row of output.
                         * put a new line character every string_width's byte
                         * value.
                         */
                        if (ai % (ptr_bstr->string_width*2) == 0) {
                            switch (ptr_bstr->output_lang) {
                                case 1:     /* C syntax */
                                    /* ensure we don't put an extra character
                                     * at the start of the binary string row
                                     * by verifying 'ai' is not zero.
                                     */
                                    if (ai != 0) { putchar('\"'); }
                                    if (ai != 0) { putchar('\n'); }
                                    /* indentation loop, repeat space
                                     * character (decimal 32) for indent_width
                                     * value + 4 (or standard indent size).
                                     */
                                    for (ic = 0; ic < indent_width+4; ic++) {
                                        putchar(32);
                                    }
                                    putchar('\"');
                                    break;
                                case 2:     /* Python syntax */
                                    if (ai != 0) { putchar('\"'); }
                                    if (ai != 0) { putchar('\n'); }
                                    /* indentation loop */
                                    for (ic = 0; ic < indent_width; ic++) {
                                        putchar(32);
                                    }
                                    /* if variable name specified */
                                    if (ptr_bstr->ptr_var_name != NULL) {
                                        printf("%s += ",
                                               ptr_bstr->ptr_var_name);
                                    } else {
                                        printf("buffer += ");
                                    }
                                    putchar('\"');
                                    break;
                                default:
                                    if (ai != 0) { putchar('\n'); }
                           }
                        }
                    }
                    putchar('\\');
                    putchar('x');
                    putchar(c);
                } else {
                    putchar(c);
                }
                /* Increase the hex escaped character array index so we keep
                 * track of the pair of hexadecimal byte inside the binary
                 * string.
                 */
                ai++;
                break;
            default:        /* all non-hexadecimal characters */
                /* catches all non-hexadcimal characters, excepted the
                 * end-of-file, the new-line and the null characters
                 * respectively.
                 */
                switch (c) {
                    case EOF: break;    // End of File
                    case 10: break;     // New Line
                    case 0: break;      // Null
                    default: invalidhexchar++;
                }
        }
    }

    /* we've reached the end of the binary string output. */
    switch (ptr_bstr->output_lang) {
        case 1: putchar('\"'); break;
        case 2: putchar('\"'); break;
    }
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

char * change_dynamic_memory(char *ptr, int new_size)
{
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
        /* leveraging snprintf() to store hexadecimal digits to the character
         * array pointed by 'ptr_badchar_array'.
         */
        length += snprintf(ptr_badchar_array+length, 3, "%02x", i);
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

char * read_from_file(char *filename, int *array_size, int mode)
{
    /* declare integer 'c' */
    int c;
    /* declare pointer to FILE 'ptr_file_read' */
    FILE *ptr_file_read;
    /* initialize char array pointer 'ptr_char_array' to NULL */
    char *ptr_char_array = NULL;

    /* initialize pointer 'ptr_file_read' */
    ptr_file_read = fopen(filename, "r");

    /* if ptr_file_read is null, return an error and exit */
    if (ptr_file_read == NULL) {
        printf("Error: input filename \"%s\" cannot be read.\n",
                filename);
        exit(EXIT_FAILURE);
    }

    /* before continuing let see in which mode we're in */
    /* mode 1+: we read file and store content on the heap */
    if (mode >= 1) {
        /* initialize char array index */
        int i = 0;

        /* declare character array 'xc' of size '3' which is enough to hold
         * two hexadecimal digits + null-termination character.
         */
        char xc[3];

        /* initialize character array pointer 'ptr_char_array' by calling
         * allocate_dynamic_memory() function.
         */
        switch (mode) {
            case 1:
                ptr_char_array = allocate_dynamic_memory(sizeof(char));
                break;
            case 2:
                ptr_char_array = allocate_dynamic_memory(sizeof(char)*2);
                *array_size += 1;
                break;
        }

        while ((c = getc(ptr_file_read)) != EOF) {
            switch (mode) {
                /* mode 1: we simply store the character in buffer. */
                case 1:
                    ptr_char_array[i] = (char)c;
                    /* the array size will be the number of characters + one
                     * after we reach EOF character in input.
                     */
                    ptr_char_array = change_dynamic_memory(ptr_char_array,
                                                           sizeof(char) *
                                                           (*array_size+=1));
                    i++;
                    break;
                /* mode 2: here we use 'snprintf()' to format each read
                 * character to hexadecimal. As two hex digits equal one byte,
                 * we must grow our destination character array accordingly.
                 */
                case 2:
                    snprintf(xc, 3, "%02x", c);
                    ptr_char_array[i] = (char)xc[0];
                    ptr_char_array[i+1] = (char)xc[1];
                    ptr_char_array = change_dynamic_memory(ptr_char_array,
                                                           sizeof(char) *
                                                           (*array_size+=2));
                    i+=2;
                    break;
            }
        }
    /* otherwise: we read from file and output to stdout directly */
    } else {
        /* get next character from file using getc() until we reach EOF */
        while ((c = getc(ptr_file_read)) != EOF) {
            printf("%02x", c);
        }
    }

    /* close opened file handler */
    fclose(ptr_file_read);

    /* return pointer to 'ptr_char_array' */
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
         doHexDumpFile, doReadFromFile, doLimitBinaryStringWidth,
         doLanguageDecoration, doPerformIndentation = false;

    /* declare 'fread_filename' character array */
    char fread_filename[MAX_FILENAME_LENGTH+1];

    /* initialize pointer 'ptr_bstr' for struct type 'bstring'.
       the struct is allocated and stored on the heap.
     */
    struct bstring *ptr_bstr = malloc(sizeof *ptr_bstr);

    /* initialize pointer 'ptr_char_array' in struct pointed by 'ptr_bstr' */
    ptr_bstr->ptr_char_array = allocate_dynamic_memory(sizeof(char));

    /* initialize pointer 'ptr_array_size' in struct pointed by 'ptr_bstr' */
    ptr_bstr->ptr_array_size = malloc(sizeof(int));

    /* declare 'arg_lang' character array */
    char arg_lang[MAX_ARGUMENT_LENGTH];

    /* getopt_long()'s long_options struct */
    static struct option long_options[] = {
        /* verbosity flags */
        {"verbose",     no_argument,    &verbose_flag, 1},
        {"quiet",       no_argument,    &verbose_flag, 0},
        {"interactive", no_argument,    &interactive_flag, 1},
        /* program actions */
        {"hex-escape",  no_argument,        NULL, 'x'},
        {"gen-badchar", no_argument,        NULL, 'b'},
        {"dump-file",   required_argument,  NULL, 'D'},
        /* program options */
        {"file",        required_argument,  NULL, 'f'},
        {"width",       required_argument,  NULL, 'w'},
        {"syntax",      required_argument,  NULL, 's'},
        {"indent",      required_argument,  NULL, 'i'},
        {"var-name",    required_argument,  NULL, 'n'},
        /* version option */
        {"version",     no_argument,    NULL, '@'},
        /* help option */
        {"help",        no_argument,    NULL, 'h'},
        {0, 0, 0, 0}
    };

    /* using getopt_long() from GNU C library to parse command-line options */
    while ((opt = getopt_long(argc, argv, ":D:xbf:w:s:i:n:h",
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
            /* program's commands and options */
            case 'v': verbose_flag = 1; break;
            case 'x': doOutputHexEscapedString = true; break;
            case 'b': doOutputBadCharString = true; break;
            case 'D':   /* dump file content in hex */
                doHexDumpFile = true;
                if (optarg != NULL)
                    snprintf(fread_filename, MAX_FILENAME_LENGTH, "%s",
                             optarg);
                break;
            case 'f':   /* file to read from option */
                doReadFromFile = true;
                if (optarg != NULL)
                    snprintf(fread_filename, MAX_FILENAME_LENGTH, "%s",
                             optarg);
                break;
            case 's':   /* syntax option given */
                doLanguageDecoration = true;
                if (optarg != NULL) {
                    snprintf(arg_lang, MAX_ARGUMENT_LENGTH, "%s", optarg);
                }
                if (strcmp(arg_lang, "c") == 0 || strcmp(arg_lang, "C") == 0) {
                    ptr_bstr->output_lang=1;
                } else if (strcmp(arg_lang, "python") == 0) {
                    ptr_bstr->output_lang=2;
                } else {
                    printf("[-] Unknown language \"%s\" specified.\n",
                           arg_lang);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'i':   /* indentation option given */
                doPerformIndentation = true;
                if (optarg != NULL) {
                    /* read the given indentation width from the command-line
                     * option and convert it to integer using atoi() from the
                     * GNU C library.
                     */
                    ptr_bstr->indent_width = atoi(optarg);
                } else {
                    /* if option is not given, initialize to zero */
                    ptr_bstr->indent_width = 0;
                }
                break;
            case 'n':   /* variable name option given */
                if (optarg != NULL) {
                    ptr_bstr->ptr_var_name =
                        allocate_dynamic_memory(MAX_ARGUMENT_LENGTH);
                    snprintf(ptr_bstr->ptr_var_name, MAX_ARGUMENT_LENGTH,
                             "%s", optarg);
                }
                break;
            case 'w':   /* binary string width option */
                doLimitBinaryStringWidth = true;
                /* make sure 'optarg' isn't null before using it */
                if (optarg != NULL) {
                    /* using atoi() from the GNU C library to read the binary
                     * string width from the command-line -w|--width option
                     * argument.
                     */
                    ptr_bstr->string_width = atoi(optarg);
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
        /* initialize integer pointed by 'ptr_array_size' */
        *(ptr_bstr->ptr_array_size) = 0;
        /* toggle verbosity if flag set */
        if (verbose_flag == true) {
            printf("[*] Convert hexadecimal input to an escaped binary string"
                   ".\n");
            if (doLimitBinaryStringWidth == true) {
                printf("[+] Binary string width is limited to %d bytes.\n",
                       ptr_bstr->string_width);
            }
            if (doLanguageDecoration == true) {
                printf("[+] Output binary string for %s language syntax.\n",
                        arg_lang);
            }
            if (doPerformIndentation == true) {
                printf("[+] Indentation level set to %d character(s).\n",
                       ptr_bstr->indent_width);
            }
        }
        /* if -D|--dump-file option is additionally given */
        if (doHexDumpFile == true) {
            /* call to read_from_file() */
            ptr_bstr->ptr_char_array = read_from_file(fread_filename,
                                                      ptr_bstr->ptr_array_size,
                                                      2);
        }
        /* if -f|--file option is given read from file instead of stdin */
        else if (doReadFromFile == true) {
            /* call to read_from_file() */
            ptr_bstr->ptr_char_array = read_from_file(fread_filename,
                                                      ptr_bstr->ptr_array_size,
                                                      1);
        }
        else {
            /* call to read_and_store_char_input() */
            ptr_bstr->ptr_char_array =
                read_and_store_char_input(ptr_bstr->ptr_array_size);
        }
        /* call to output_hex_escaped_string() */
        output_hex_escaped_string(ptr_bstr);
        /* call to free() for 'ptr_char_array' */
        free(ptr_bstr->ptr_char_array);
        /* call to free() for 'ptr_array_size' */
        free(ptr_bstr->ptr_array_size);
        /* call to free() for 'ptr_bstr' */
        free(ptr_bstr);
        /* exit as we're the last action */
        exit(EXIT_SUCCESS);
    }

    /* if -D|--dump-file option is given */
    if (doHexDumpFile == true) {
        /* call to read_from_file() */
        read_from_file(fread_filename, NULL, 0);
        /* exit as we're the last action */
        exit(EXIT_SUCCESS);
    }

    /* if -b|--gen-badchar option is given */
    if (doOutputBadCharString == true) {
        /* initialize integer 'array_size' to BADCHAR_HEX_SEQLEN bytes */
        *(ptr_bstr->ptr_array_size) = BADCHAR_HEX_SEQLEN;
        /* toggle verbosity if flag set */
        if (verbose_flag == true) {
            printf("[*] Generating bad character binary string.\n");
            if (doLimitBinaryStringWidth == true) {
                printf("[+] Binary string width is limited to %d bytes.\n",
                       ptr_bstr->string_width);
            }
        }
        /* call to generate_badchar_sequence() */
        ptr_bstr->ptr_char_array = generate_badchar_sequence();
        /* call to output_hex_escaped_string() */
        output_hex_escaped_string(ptr_bstr);
        /* calls to free() for the various allocated memory locations */
        free(ptr_bstr->ptr_char_array);
        free(ptr_bstr->ptr_array_size);
        free(ptr_bstr);
        /* exit as we're the last action */
        exit(EXIT_SUCCESS);
    }

    return 0;
}
