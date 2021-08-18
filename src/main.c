/* main.c

   Copyright (C) 2021 Mariano Ruiz <mrsarm@gmail.com>
   This file is part of the "Another Huffman" encoder project.

   This project is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the "Frequency Counter" project; if not, see
   <http://www.gnu.org/licenses/>.  */


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <signal.h>
#include "const.h"
#include "freqlist.h"
#include "ah.h"
#include "util.h"


#define USAGE   "Usage: %s [-dcvh] [FILE]\n" \
                "Compress or uncompress FILE using Huffman encoding " \
                "(by default, compress FILE in-place).\n" \
                "\n" \
                "Options:\n" \
                "  -c       write on standard output\n" \
                "  -d       decompress\n" \
                "  -v       verbose mode, print the frequency table (if compressing)\n" \
                "           and the binary tree used in the encryption\n" \
                "  -h       display this help and exit\n" \
                "\n" \
                "With no FILE, or when FILE is -, read standard input.\n" \
                "\"Another Huffman\" encoder project v1.0.0: ah <https://github.com/mrsarm/ah>\n"


/* Initialize the global variables with the command arguments */
ah_data* init_options(int argc, char *argv[]);

/* Ctrl+C handler */
void ctrlc_handler(int sig);

/* Compress input */
void compress();
/* Decompress input */
void decompress();

ah_data* data;

int main(int argc, char *argv[])
{
    signal(SIGINT, ctrlc_handler);                      // Initialize Ctrl+C signal
    data = init_options(argc, argv);                    // Initialize data with the command arguments

    int r = ah_data_init_resources(data);               // Initialize resources (files)
    switch (r) {
        case OK: break;
        case ERROR_FILE_NOT_FOUND:
            error_cannot_open(r, "input", data->filename_in, (void*)ah_data_free_resources, data);
        case ERROR_FILE_OUT:
            error_cannot_open(r, "output", data->filename_out, (void*)ah_data_free_resources, data);
        case INVALID_FILE_IN:
            error_invalid_file_in(r, "input", data->filename_in, (void*)ah_data_free_resources, data);
        case ERROR_MEM:
            error_mem((void*)ah_data_free_resources, data);
        default:
            error_unknown_code(r, "ah_data_init_resources", (void*)ah_data_free_resources, data);
    }
    if (data->decompres) {
        decompress();                                   // Decompress input into output
    } else {
        compress();                                     // Compress input into output
    }

    ah_data_free_resources(data);                       // Close file and free memory
    return 0;
}

/* Compress input */
void compress() {
    int r = ah_count(data);                             // Count the symbols
    switch (r) {
        case OK: break;
        case ERROR_MEM:
            error_mem((void*)ah_data_free_resources, data);
            default:
                error_unknown_code(r, "ah_count", (void*)ah_data_free_resources, data);
    }

    freqlist_build_huff(data->freql);                   // Build Huffman tree
    if (data->verbose) {
        freqlist_fprintf(stderr, VERBOSE_TABLE, data->freql);
        fprintf(stderr, "\n");
        freqlist_fprintf_tree(stderr, VERBOSE_TREE, data->freql);
    }

    ah_encode(data);                                    // Encode and write
}

/* Decompress input */
void decompress() {
    int r = ah_decode(data);
    switch (r) {
        case OK: break;
        case ERROR_MEM:
            error_mem((void*)ah_data_free_resources, data);
        default:
            error_unknown_code(r, "ah_decode", (void*)ah_data_free_resources, data);
    }
    if (data->verbose) {
        freqlist_fprintf_tree(stderr, VERBOSE_TREE, data->freql);
    }
}


/* Initialize the global variables with the command options */
ah_data* init_options(int argc, char *argv[]) {
    ah_data* data = ah_data_init();
    if (!data) error_mem(NULL, NULL);
    opterr = 0;
    int c;
    while ((c = getopt(argc, argv, "dcvh")) != -1) {
        switch (c) {
            case 'h':
                printf(USAGE, argv[0]);
                exit(0);
            case 'd':
                data->decompres = TRUE;
                break;
            case 'v':
                data->verbose = TRUE;
                break;
            case 'c':
                data->fo = stdout;
                break;
            case '?':
                if (isprint (optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                    fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
                } else {
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                }
                exit(ERROR_PARAM);
        }
    }
    for (int index = optind; index < argc; index++) {
        if (!data->filename_in) {
            data->filename_in = argv[index];
        } else {
            fprintf(stderr, "Error: extra operand `%s'\n", argv[index]);
            fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
            exit(ERROR_PARAM);
        }
    }
    return data;
}


/* Ctrl+C handler */
void ctrlc_handler(int sig) {
    printf("\n");
    if (data && data->freql && data->verbose) {
        if (!data->decompres) {
            freqlist_sort(data->freql);
            freqlist_build_huff(data->freql);
            freqlist_fprintf(stdout, VERBOSE_TABLE, data->freql);
            fprintf(stderr, "\n");
        }
        freqlist_fprintf_tree(stderr, VERBOSE_TREE, data->freql);
    }
    exit(0);
}

/*  End program.  */
