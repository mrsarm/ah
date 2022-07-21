/* freqlist.h

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


#ifndef _FREQLIST_H
#define _FREQLIST_H


#include <stdio.h>


/*
 * Node element of the frequency list.
 *
 *   prev * node ( <= freq )
 *        ▲
 *        |
 *       node
 *        |
 *        ▼
 *   next * node ( >= freq )
 *
 */
typedef struct _node_freqlist
{
    unsigned char symb;             /* Referenced symbol. */
    unsigned char pos;              /* Position in the list starting in 0
                                       the first element in the list
                                       (higher frequency) */
    unsigned long freq;             /* Occurrences of the character
                                       in the stream (frequency). */

    /* Table data, used to have the table sorted by freq */

    struct _node_freqlist *prev;    /* Pointer to the previous node in
                                       the list, with frequency less
                                       or equal than this. */
    struct _node_freqlist *next;    /* Pointer to the next node in the list,
                                       with frequency greater or equal
                                       than this. */

    /* Tree data, used to build the binary tree with the Huffman codes */

    struct _node_freqlist *tnext;   /* Next node in the tree list */
    struct _node_freqlist *zero;    /* Pointer to the previous node in
                                       the list, with frequency greater
                                       or equal than this. */
    struct _node_freqlist *one;     /* Pointer to the next node in the list,
                                       with frequency less or equal
                                       than this. */
    unsigned long int bits;         /* Huffman binary code */
    unsigned char nbits;            /* Number of bits used by bits */
} node_freqlist;


/*
 * Struct with the node list of frequencies,
 * and an array with the frequency of each symbol.
 */
typedef struct _freqlist
{
    node_freqlist *list;            /* Pointer to the first node in the table
                                       (with the highest frequency). */
    node_freqlist *tree;            /* Pointer to the first node in the Huffman
                                       tree (not a char unless the tree only has
                                       one symbol). */
    unsigned int length;            /* Numbers of different symbols
                                       in the list. */
    unsigned long size;             /* Numbers of symbols in the list. */
} freqlist;


/*
 * Create a new freqlist.
 */
freqlist* freqlist_create();


/*
 * Create and return a new node.
 */
node_freqlist* freqlist_create_node(unsigned char c,
                                    unsigned char pos,
                                    unsigned long freq);

/*
 * Free the memory of the list.
 */
void freqlist_free(freqlist* l);

/*
 * Return the node with the symbol 'c', or return NULL if not
 * in the list.
 */
node_freqlist *freqlist_find(const freqlist* l, unsigned char c);

/*
 * Increase the frequency of the symbol 'c' in +1,
 * and rearrange if necessary. If the symbol is not present
 * in the list, add it, and set the frequency of the node in 1.
 * Return the node with the symbol.
 */
node_freqlist *freqlist_add(freqlist *l, unsigned char c);

/*
 * Print the list of frequencies and binary codes from Huffman coding.
 * @f: the output stream, eg. the stdout
 * @title: print this message before the list (optional)
 * @freql: the frequency list
 */
void freqlist_fprintf(FILE *f, const char *title, const freqlist *freql);

/*
 * Print the tree with the frequencies and binary codes from Huffman coding.
 * @f: the output stream, eg. the stdout
 * @title: print this message before the tree (optional)
 * @freql: the frequency list
 */
void freqlist_fprintf_tree(FILE *f, const char *title, const freqlist *freql);


/*
 * Sort the frequencies in case aren't sort.
 * Return the number of swaps made.
 */
int freqlist_sort(freqlist *l);

/*
 * Build the Huffman tree and binary
 * codes for encoding.
 * The freqlist has to be sorted first.
 */
int freqlist_build_huff(freqlist *l);


/*
 * Compare by frequency of the node,
 * or symbol if the frequencies are equal.
 */
int node_cmp(const node_freqlist *pnode1, const node_freqlist *pnode2);


#endif /* _FREQLIST_H */
