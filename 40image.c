/**************************************************************
 *
 *                     40image.c
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     This file takes in a file and a command from standard input.
 *     The file will contain a ppm or a COMP40 compressed image.
 *     If no file is given it will read from standard input. 
 *     Depending on the command given, it will then call a function
 *     to either compress or decompress the input. 
 *     
 *     Note
 *     If the given file is null, an unknown command is supplied,
 *     or more than one file is supplied, the program will
 *     exit with a checked runtime error.
 *     
 **************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <assert.h>
#include <compress40.h>
#include <a2methods.h>
#include <a2plain.h>
#include <uarray.h>
#include <pnm.h>
#include <bitpack.h>

#include "colorspace.h"
#include "quantize.h"
#include "codeword.h"
#include "dctrans.h"

static void (*compress_or_decompress)(FILE *input) = compress40;

int main(int argc, char *argv[])
{
    int i;

    for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-c") == 0) {
                    compress_or_decompress = compress40;
            } else if (strcmp(argv[i], "-d") == 0) {
                    compress_or_decompress = decompress40;
            } else if (*argv[i] == '-') {
                    fprintf(stderr, "%s: unknown option '%s'\n",
                            argv[0], argv[i]);
                    exit(1);
            } else if (argc - i > 2) {
                fprintf(stderr, "Usage: %s -d [filename]\n"
                        "       %s -c [filename]\n",
                        argv[0], argv[0]);
                exit(1);
            } else {
                break;
            }
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                compress_or_decompress(fp);
                fclose(fp);
        } else {
                compress_or_decompress(stdin);
        }

        return EXIT_SUCCESS;
}
