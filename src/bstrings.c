/* vi:set tw=78 ts=8 sw=4 sts=4 et:
 *
 * Binary String Toolkit
 * This program allow to perform conversion operations on binary strings while
 * supporting various input and output formats such as:
 *     - hexadecimal escaped string (\x).
 *
 */

/*
 * bstrings.c: main program source file
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/bool.h"

void output_hex_escaped_string(int *ptr_char_array, int *array_size) {
   int i, c;

   /* initialize the hex escaped character array index to zero */
   int ai = 0;

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
        }
    }
}

int * allocate_dynamic_memory(int alloc_size) {
    /* use malloc() to allocate dynamic memory and then return to the caller
     * function the memory location allocated on the heap.
     */
    int *ptr = malloc(alloc_size);
    return ptr;

    /* error handling: on errors malloc() returns NULL. */
    if (ptr == NULL) {
        printf("%d byte(s) memory allocation error.", alloc_size);
        exit(EXIT_FAILURE);
    }
}

int * change_dynamic_memory(int *ptr, int new_size) {
    /* call to realloc() to change the size of the memory block pointed to by
     * the pointer 'ptr' with the new size value in 'new_size'.
     */
    int *new_ptr = realloc(ptr, new_size);
    return new_ptr;

    /* error handling: on errors realloc() returns NULL. */
    if (new_ptr == NULL) {
        printf("%d byte(s) memory re-allocation error.", new_size);
        exit(EXIT_FAILURE);
    }
}

int * read_and_store_char_input(int *array_size) {
    /* declare integer 'c' which will hold input character. */
    int c;
    /* initialize integer 'i' which will be used as an array index. */
    int i = 0;

    /* initialize character array pointer 'ptr_char_array' by calling
     * allocate_dynamic_memory() function.
     */
    int *ptr_char_array = allocate_dynamic_memory(sizeof(int));

    /* store each input character into the character array 'ptr_char_array'
     * until we reach EOF.
     */
    while ((c = getchar()) != EOF) {
        ptr_char_array[i] = (char)c;
        ptr_char_array = change_dynamic_memory(ptr_char_array, sizeof(int) * \
                         (*array_size+=1));
        i++;
    }

   return ptr_char_array;
}

int main(int argc, char *argv[]) {
    /* initialize all variables needed for command-line options handling. */
    int opt;
    bool doOutputHexEscapedString = false;

    /* initialize integer 'array_size' */
    int array_size = 1;

    /* using getopt() from POSIX C library to parse command-line options. */
    while ((opt = getopt(argc, argv, "x")) != -1) {
        switch (opt) {
            case 'x': doOutputHexEscapedString = true; break;
            default:
                fprintf(stderr, "Usage: %s [-x]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (doOutputHexEscapedString == true) {
        /* call to read_and_store_char_input() */
        int *ptr_char_array = read_and_store_char_input(&array_size);
        /* call to output_hex_escaped_string() */
        output_hex_escaped_string(ptr_char_array, &array_size);
    }

    return 0;
}
