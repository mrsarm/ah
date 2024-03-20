/* ah.c

   Copyright (C) 2021-2023 Mariano Ruiz <mrsarm@gmail.com>
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


#define VERSION_BYTE    HEADER_COO_VERSION << (8 - HEADER_COO_VERSION_BITS)
#define FLAGS_1_BYTE    0


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
        data->length_out = 0l;
        data->header_flags[0] = 0;
        data->header_flags[1] = 0;
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
    // For now only the version of the format is stored in the flags byte
    data->header_flags[0] = VERSION_BYTE;
    data->header_flags[1] = FLAGS_1_BYTE;
    return 0;
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
 * Write header information in output file, including
 * the Huffman table.
 */
int _ah_write_header(ah_data *data) {
    // Write "magic" number that identifies the format
    fwrite(MAGIC_NUMBER, MAGIC_NUMBER_SIZE, 1, data->fo);
    // Write basic header info
    fputc(data->header_flags[0], data->fo);
    fputc(data->header_flags[1], data->fo);
    // Write original input size in bytes
    fwrite(&data->length_in, NUMBER_SIZE, 1, data->fo);
    // Write number of source symbols
    unsigned short int length = data->freql->length;
    fwrite(&length, SMALL_COUNT_SIZE, 1, data->fo);
    // Write each node from the freqlist
    node_freqlist *pnode = data->freql->list;
    while(pnode) {
        fwrite(&pnode->symb, SYMBOL_SIZE, 1, data->fo);
        fwrite(&pnode->nbits, SYMBOL_SIZE, 1, data->fo);
        // Write each symbol "bits" with as fewer bits as possible
        int bytes_size = ah_bits_bytes_size(pnode->nbits);
        if (bytes_size == 1) {
            unsigned char bits = (unsigned char) pnode->bits;
            fwrite(&bits, bytes_size, 1, data->fo);
        } else if (bytes_size == 2) {
            unsigned short bits = (unsigned short) pnode->bits;
            fwrite(&bits, bytes_size, 1, data->fo);
        } else if (bytes_size == 4) {
            unsigned int bits = (unsigned int) pnode->bits;
            fwrite(&bits, bytes_size, 1, data->fo);
        } else if (bytes_size == 8) {
            fwrite(&pnode->bits, bytes_size, 1, data->fo);
        } else {
            return INVALID_BITS_SIZE;
        }
        pnode = pnode->next;
    }
    return OK;
}


/*
 * Encode and write the compressed data.
 */
int ah_encode(ah_data *data) {
    int r = _ah_write_header(data);
    if (r) return r;

    if (data->buffer_in) {
        data->fi = fmemopen(data->buffer_in, data->length_in, "rb");
    } else {
        rewind(data->fi);
    }
    unsigned long int dword = 0l;   // Word used during encoding
    int nbits = 0;                  // Number of bits used in dword
    unsigned char c;                // Read here input file byte by byte
    node_freqlist *pnode;           // Current node to be written
    do {
        c = fgetc(data->fi);
        if(feof(data->fi)) break;
        pnode = freqlist_find(data->freql, c);
        // If nbits + pnode->nbits > 32, pull off a byte
        while(nbits + pnode->nbits > 32) {
            c = dword >> (nbits - 8);                   // Extract the 8 bits with higher
            fwrite(&c, SYMBOL_SIZE, 1, data->fo);       // order and write down into the file.
            nbits -= 8;                                 // Now we have those 8 bits available
            data->length_out += SYMBOL_SIZE;
        }
        dword <<= pnode->nbits;                         // Make room for the new byte
        dword |= pnode->bits;                           // Insert the new byte
        nbits += pnode->nbits;                          // Update the number of bits
    } while(TRUE);
    while(nbits > 0) {                                  // Extract the 4 bytes remaining in dword
        if(nbits>=8) c = dword >> (nbits - 8);
        else c = dword << (8 - nbits);
        fwrite(&c, SYMBOL_SIZE, 1, data->fo);
        nbits -= 8;
        data->length_out += SYMBOL_SIZE;
    }
    return OK;
}

int _ah_read_header(ah_data *data) {
    data->freql = freqlist_create();
    if (!data->freql) {
        return ERROR_MEM;
    }
    data->freql->tree = freqlist_create_node((unsigned char)0, (unsigned char)0, 0l);
    if (!data->freql->tree) {
        return ERROR_MEM;
    }
    char magic_number[MAGIC_NUMBER_SIZE];
    fread(&magic_number, MAGIC_NUMBER_SIZE, 1, data->fi);
    if (strcmp(magic_number, MAGIC_NUMBER) != 0) {
        return INVALID_FILE_IN;
    }
    data->header_flags[0] = fgetc(data->fi);
    if (data->header_flags[0] != VERSION_BYTE) {
        return INVALID_FILE_IN;     // Different version not supported?
    }
    data->header_flags[1] = fgetc(data->fi);
    if (data->header_flags[1] != FLAGS_1_BYTE) {
        return INVALID_FILE_IN;     // New flags not supported?
    }
    // Original input size in bytes
    fread(&data->length_in, NUMBER_SIZE, 1, data->fi);
    if (data->length_in == 0) return 0; // Empty file

    // Number of source symbols
    unsigned short int length;
    fread(&length, SMALL_COUNT_SIZE, 1, data->fi);
    data->freql->length = length;

    for(unsigned int i = 0; i < data->freql->length; i++) {         // Read all elements
        node_freqlist* p = freqlist_create_node(0, 0, 0l);
        if (!p) return ERROR_MEM;
        fread(&p->symb, SYMBOL_SIZE, 1, data->fi);                  // Read node values
        fread(&p->nbits, SYMBOL_SIZE, 1, data->fi);
        int bytes_size = ah_bits_bytes_size(p->nbits);
        if (bytes_size == 1) {
            unsigned char bits;
            fread(&bits, bytes_size, 1, data->fi);
            p->bits = bits;
        } else if (bytes_size == 2) {
            unsigned short bits;
            fread(&bits, bytes_size, 1, data->fi);
            p->bits = bits;
        } else if (bytes_size == 4) {
            unsigned int bits;
            fread(&bits, bytes_size, 1, data->fi);
            p->bits = bits;
        } else if (bytes_size == 8) {
            fread(&p->bits, bytes_size, 1, data->fi);
        } else {
            return INVALID_BITS_SIZE;
        }
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
    return OK;
}

/*
 * Decode and write the raw data.
 */
int ah_decode(ah_data *data) {
    int r = _ah_read_header(data);
    if (r) return r;

    // Read compressed data and extract to the output stream
    unsigned int bits = 0;
    unsigned char a = 0;
    // Read the first 4 bytes in the double word bits
    if (!feof(data->fi)) {
        fread(&a, SYMBOL_SIZE, 1, data->fi);
        bits |= a;
    }
    bits <<= 8;
    if (!feof(data->fi)) {
        fread(&a, SYMBOL_SIZE, 1, data->fi);
        bits |= a;
    }
    bits <<= 8;
    if (!feof(data->fi)) {
        fread(&a, SYMBOL_SIZE, 1, data->fi);
        bits |= a;
    }
    bits <<= 8;
    if (!feof(data->fi)) {
        fread(&a, SYMBOL_SIZE, 1, data->fi);
        bits |= a;
    }
    int j = 0;      /* Each 8 bits another byte is read */
    node_freqlist* q = data->freql->tree;

    do {
        if (bits & 0x80000000) q = q->one; else q = q->zero;        // Right branch
        bits <<= 1;                                                 // Next bit
        j++;
        if (8 == j) {                                               // Each 8 bits
            fread(&a, SYMBOL_SIZE, 1, data->fi);                    // Read 1 byte from file
            bits |= a;                                              // and insert in bits
            j = 0;                                                  // No holes
        }
        if (!q->one && !q->zero) {                                  // If node is a symbol
            putc(q->symb, data->fo);                                // write down to the file
            data->length_in--;                                      // Update remaining length
            q=data->freql->tree;                                    // Back to the tree's root
        }
    } while (data->length_in);                                      // Until file is over

    return 0;
}

/*
 * Return the number of bytes to use to
 * record a code of nbits.
 */
int ah_bits_bytes_size(unsigned char nbits) {
    if (nbits <= 8) return 1;
    if (nbits <= 16) return 2;
    if (nbits <= 32) return 4;
    if (nbits <= 64) return 8;
    return -1;  // Overflow, should never reach this
}

/*
 * Print a summary.
 *
 * @f: the output stream, e.g. the stdout
 * @data: the coo_data struct with the data
 */
void ah_fprintf_summary(FILE *f, const ah_data *data) {
    fprintf(f, "[ Summary ] ===================================\n");
    fprintf(f, "Uncompressed size: %lu\n", data->length_in);
    fprintf(f, "Compressed size (without headers): %lu\n", data->length_out);
    if (data->length_in > 0) {
        fprintf(f, "Ratio (without headers): %f\n",
                ((double) data->length_out) / (double) data->length_in);
    } else {
        fprintf(f, "Ratio (without headers): -\n");
    }
    fprintf(f, "===============================================\n");
}
