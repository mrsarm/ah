/* ah.h

   Copyright (C) 2021-2022 Mariano Ruiz <mrsarm@gmail.com>
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


#ifndef __AH_H
#define __AH_H


#include "freqlist.h"


/*
 * Contain the main information about
 * the encoding process: input file, frequency list, etc.
 */
typedef struct _ah_data {
    char *filename_in,          /* Input file name */
         *filename_out;         /* Output file name with the encoded data */
    FILE *fi,                   /* Input file manager */
         *fo;                   /* Output file manager. */
    unsigned long length_in;    /* File size in bytes */
    unsigned char *buffer_in;   /* Buffer to keep all the content from
                                   fi (only used when fi = stdin) */
    unsigned long length_buff;  /* Buffer size in bytes */
    freqlist *freql;            /* Frequency list of characters */
    int decompres;              /* If TRUE is decompression */
    int verbose;                /* If TRUE the verbose mode is activated */
} ah_data;



/*
 * Return ah_data struct initialized with
 * default values.
 */
ah_data* ah_data_init(void);



/*
 * Initialization of input data structure
 * from the given file name.
 * Open data->filename_in in "rb" mode, if it's
 * NULL, use stdin as data->fi file.
 * Return `0` if no errors, otherwise an error code.
 */
int ah_data_init_resources(ah_data *data);


/*
 * Initialize the freql struct of data.
 */
freqlist *ah_data_init_freql(ah_data *data);


/*
 * Free all resources of the application.
 */
void ah_data_free_resources(ah_data *data);


/*
 * Count the frequencies.
 */
int ah_count(ah_data *data);

/*
 * Encode and write the compressed data.
 */
void ah_encode(ah_data *data);

/*
 * Decode and write the raw data.
 */
int ah_decode(ah_data *data);


#endif /* __AH_H */
