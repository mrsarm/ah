/* ah.c

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
#include <string.h>
#include <unistd.h>
#include "ah.h"
#include "const.h"
#include "freqlist.h"
#include "util.h"


/*
 * Return ah_data struct initialized with
 * default values.
 */
ah_data *ah_data_init(void) {
    ah_data* data = (ah_data*) malloc(sizeof(ah_data));
    if (data) {
        data->verbose = FALSE;
        data->decompres = FALSE;
        data->filename_in = NULL;
        data->fi = NULL;
        data->buffer_in = NULL;
        data->length_buff = 0;
        data->freql = NULL;
        data->length_in = 0l;
    }
    return data;
}


/*
 * Initialization of input/output data structures
 * from the given file name.
 * Open data->filename_in in "rb" mode, if it's
 * NULL, use stdin as data->fi file and stdout as data->fo.
 * Return `0` if no errors, otherwise an error code.
 */
int ah_data_init_resources(ah_data *data) {
    if (data->filename_in && strcmp(data->filename_in, "-")) {
        /* Open the file in binary mode / read only */
        data->fi = fopen(data->filename_in, "rb");
        if (!data->fi) {
            return ERROR_FILE_NOT_FOUND;
        }
        if (!data->fo) {    // if fo was not assigned yet
            if (data->decompres) {
                data->filename_out = rmsub(data->filename_in, OUTPUT_EXT);
                if (!data->filename_out) {  // Invalid extension
                    return INVALID_FILE_IN;
                }
            } else {
                data->filename_out = cat(data->filename_in, OUTPUT_EXT);
            }
            data->fo = fopen(data->filename_out, "wb");
            if (!data->fo) {
                return ERROR_FILE_OUT;
            }
        }
    } else {
        data->fi = fdopen(dup(fileno(stdin)), "rb");
        data->buffer_in = (unsigned char *)malloc(BUFFER_WINDOW);
        if (!data->buffer_in) {
            return ERROR_MEM;
        }
        data->length_buff = BUFFER_WINDOW;
        data->fo = fdopen(dup(fileno(stdout)), "wb");
    }
    return 0;
}


/*
 * Initialization of input/output data structures
 * from the given file.
 */
void ah_data_init_resources_fi(ah_data *data, FILE *fi) {
    data->fi = fi;
    data->length_in = 0l;
}


/*
 * Initialize the freql struct of data.
 */
freqlist *ah_data_init_freql(ah_data *data) {
    data->freql = freqlist_create();
    return data->freql;
}


/*
 * Free all input/output resources of the application.
 */
void ah_data_free_resources(ah_data *data) {
    if (data->fi) {
        fflush(data->fi);
        fclose(data->fi);
    }
    if (data->fo) {
        fflush(data->fo);
        fclose(data->fo);
        if (data->filename_out) {
            free(data->filename_out);
        }
    }
    if (data->buffer_in) {
        free(data->buffer_in);
    }
    if (data->freql) freqlist_free(data->freql);
    free(data);
}


/*
 * Count the frequencies.
 */
int ah_count(ah_data *data) {
    if (!data->freql) {
        if (!ah_data_init_freql(data)) {
            error_mem((void*)ah_data_free_resources, data);
        }
    }
    do {
        unsigned char symbol = fgetc(data->fi);   // A buffer is not used due
        if( feof(data->fi) ) {                    // that any modern OS
            break ;                               // has one provided by
        }                                         // the libc library when I/O
        if (data->buffer_in) {
            data->buffer_in[data->length_in] = symbol;
            if (data->length_in+1 == data->length_buff) {
                data->length_buff += BUFFER_WINDOW;
                data->buffer_in = (unsigned char *)realloc(data->buffer_in, data->length_buff);
                if (!data->buffer_in) {
                    return ERROR_MEM;
                }
            }
        }
        node_freqlist *pnode = freqlist_add(data->freql, symbol);
        if (!pnode) {
            return ERROR_MEM;
        }
        data->length_in++;
    } while (TRUE);
    freqlist_sort(data->freql);
    return 0;
}

/*
 * Encode and write the compressed data.
 */
void ah_encode(ah_data *data) {
    // Write original input size in bytes
    fwrite(&data->length_in, sizeof(data->length_in), 1, data->fo);
    // Number of source symbols
    fwrite(&data->freql->length, sizeof(data->freql->length), 1, data->fo);
    node_freqlist *pnode = data->freql->list;
    while(pnode) {
        fwrite(&pnode->symb, sizeof(pnode->symb), 1, data->fo);
        fwrite(&pnode->bits, sizeof(pnode->bits), 1, data->fo);
        fwrite(&pnode->nbits, sizeof(pnode->nbits), 1, data->fo);
        pnode = pnode->next;
    }
    if (data->buffer_in) {
        data->fi = fmemopen(data->buffer_in, data->length_in, "rb");
    } else {
        rewind(data->fi);
    }
    unsigned long int dword = 0l;   // Word used during encoding
    int nbits = 0;                  // Number of bits used in dword
    unsigned char c;                // Read here input file byte by byte
    do {
        c = fgetc(data->fi);
        if(feof(data->fi)) break;
        pnode = freqlist_find(data->freql, c);
        // If nbits + pnode->nbits > 32, pull off a byte
        while(nbits + pnode->nbits > 32) {
            c = dword >> (nbits - 8);                   // Extract the 8 bits with higher order
            fwrite(&c, sizeof(c), 1, data->fo);      // and write down into the file.
            nbits -= 8;                                 // Now we have those 8 bits available
        }
        dword <<= pnode->nbits;                         // Make room for the new byte
        dword |= pnode->bits;                           // Insert the new byte
        nbits += pnode->nbits;                          // Update the number of bits
    } while(TRUE);
    while(nbits > 0) {                                  // Extract the 4 bytes remaining in dword
        if(nbits>=8) c = dword >> (nbits - 8);
        else c = dword << (8 - nbits);
        fwrite(&c, sizeof(c), 1, data->fo);
        nbits -= 8;
    }
}

/*
 * Decode and write the raw data.
 */
int ah_decode(ah_data *data) {
    data->freql = freqlist_create();
    if (!data->freql) {
        return ERROR_MEM;
    }
    data->freql->tree = freqlist_create_node((unsigned char)0, (unsigned char)0, 0l);
    if (!data->freql->tree) {
        return ERROR_MEM;
    }
    // Original input size in bytes
    fread(&data->length_in, sizeof(data->length_in), 1, data->fi);
    // Number of source symbols
    fread(&data->freql->length, sizeof(data->freql->length), 1, data->fi);

    for(unsigned int i = 0; i < data->freql->length; i++) {         // Read all elements
        node_freqlist* p = freqlist_create_node((unsigned char)0, (unsigned char)0, 0l);
        if (!p) return ERROR_MEM;
        fread(&p->symb, sizeof(p->symb), 1, data->fi);           // Read node values
        fread(&p->bits, sizeof(p->bits), 1, data->fi);
        fread(&p->nbits, sizeof(p->nbits), 1, data->fi);
        int j = 1 << (p->nbits-1);                                  // Insert node in place
        node_freqlist* q = data->freql->tree;
        while(j > 1) {
            if(p->bits & j) {                                       // It's a one
                if (q->one) {                                       // If node exist,
                    q = q->one;                                     // move to it
                } else {                                            // else it's created
                    q->one = freqlist_create_node((unsigned char)0, (unsigned char)0, 0l);
                    if (!q->one) return ERROR_MEM;
                    q = q->one;
                    q->symb = 0;
                    q->one = q->zero = NULL;
                }
            } else {                                                // It's a zero
                if(q->zero) {
                    q = q->zero;
                } else {
                    q->zero = freqlist_create_node((unsigned char)0, (unsigned char)0, 0l);
                    if (!q->zero) return ERROR_MEM;
                    q = q->zero;
                    q->symb = 0;
                    q->one = q->zero = NULL;
                }
            }
            j >>= 1;                                                // Next bit
        }
        // Last bit
        if(p->bits & 1) {                                           // It's a one
            q->one = p;
        } else {                                                    // It's a zero
            q->zero = p;
        }
    }

    // Read compressed data and extract to the output stream
    unsigned long int bits = 0;
    unsigned char a;
    // Read the first 4 bytes in the double word bits
    fread(&a, sizeof(a), 1, data->fi);
    bits |= a;
    bits <<= 8;
    fread(&a, sizeof(a), 1, data->fi);
    bits |= a;
    bits <<= 8;
    fread(&a, sizeof(a), 1, data->fi);
    bits |= a;
    bits <<= 8;
    fread(&a, sizeof(a), 1, data->fi);
    bits |= a;
    int j = 0;      /* Cada 8 bits leemos otro byte */
    node_freqlist* q = data->freql->tree;

    do {
        if(bits & 0x80000000) q = q->one; else q = q->zero;         // Right branch
        bits <<= 1;                                                 // Next bit
        j++;
        if(8 == j) {                                                // Each 8 bits
            fread(&a, sizeof(a), 1, data->fi);                      // Read 1 byte from file
            bits |= a;                                              // and insert in bits
            j = 0;                                                  // No holes
        }
        if(!q->one && !q->zero) {                                   // If node is a symbol
            putc(q->symb, data->fo);                                // write down to the file
            data->length_in--;                                      // Update remaining length
            q=data->freql->tree;                                    // Back to the tree's root
        }
    } while(data->length_in);                                       // Until file is over

    return 0;
}
