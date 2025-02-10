/* util_t.c

   Copyright (C) 2021-2025 Mariano Ruiz <mrsarm@gmail.com>
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
   License along with the "Another Huffman" encoder project; if not, see
   <http://www.gnu.org/licenses/>.  */


#include <stdlib.h>
#include "util_t.h"
#include "const.h"
#include "util.h"


/*
 * Count the buffer passed and return the ``ah_data`` with the output data.
 */
ah_data* count_buff(unsigned char* buff_in, unsigned int buff_in_length, int verbose) {

    ah_data* data = ah_data_init();
    if (!data) {
        error_mem(NULL, NULL);
    }
    data->verbose = verbose;
    // Initialize with a memory stream
    data->fi = fmemopen(buff_in, buff_in_length, "rb");
    data->length_in = 0l;

    int r = ah_count(data);
    switch (r) {
        case OK: break;
        case ERROR_MEM:
            error_mem((void*)ah_data_free_resources, data);
        default:
            error_unknown_code(r, "fq_count", (void*)ah_data_free_resources, data);
    }

    freqlist_fprintf(stdout, "> Final frequency table\n", data->freql);
    if (data->verbose) printf("\n=====================================\n\n");
    return data;
}


/*
 * Check if plist is equals to expected_freqlist (second list is in
 * array format).
 */
int freqlist_check(const freqlist* plist, const unsigned int expected_freqlist[][2],
                   unsigned int expected_length) {

    if (plist->length !=  expected_length) {
        fprintf(stderr, "Different lengths. plist: %d - expected: %d\n", plist->length, expected_length);
        return FALSE;
    }
    node_freqlist *pnode = plist->list;
    int i = 0;
    while (pnode) {
        if (pnode->symb != expected_freqlist[i][0]
                || pnode->freq != expected_freqlist[i][1]) {
            fprintf(stderr, "Different values in plist. Pos.: %d - Symbols: %d, %d. "
                            "Frequencies: %lu, %u\n",
                    i, (int)pnode->symb, expected_freqlist[i][0], pnode->freq, expected_freqlist[i][1]);
            return FALSE;
        }
        if (pnode->pos != i) {
            fprintf(stderr, "Error: different pos value in node. Pos. expected %d - Pos. %d\n",
                    i, pnode->pos);
            return FALSE;
        }
        i++;
        pnode = pnode->next;
    }
    return TRUE;
}

/*
 * Check if plist tree data is equals to expected_freqlist (second list is in
 * array format).
 */
int freqlist_check_tree(const freqlist* plist, const unsigned int expected_freqlist[][3],
                   unsigned int expected_length) {

    if (plist->length !=  expected_length) {
        fprintf(stderr, "Different lengths. plist: %d - expected: %d\n", plist->length, expected_length);
        return FALSE;
    }
    node_freqlist *pnode = plist->list;
    int i = 0;
    while (pnode) {
        if (!pnode->zero && !pnode->one) {
            if (pnode->symb != expected_freqlist[i][0]
                    || pnode->nbits != expected_freqlist[i][1]
                    || pnode->bits != expected_freqlist[i][2]) {
                fprintf(stderr, "Different values in plist. Pos.: %d - Symbols: '%c', '%c'. "
                                "nbits: %d, expected nbits: %d "
                                "bits: 0x%08lX, expected bits: 0x%08X \n",
                                i, (int)pnode->symb, expected_freqlist[i][0],
                                pnode->nbits, expected_freqlist[i][1],
                                pnode->bits, expected_freqlist[i][2]);
                return FALSE;
            }
            i++;
        }
        pnode = pnode->next;
    }
    return TRUE;
}
