/* freqlist.c

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


#include "const.h"
#include "freqlist.h"
#include <stdlib.h>


/* Print the binary number with '0's and 1's for debugging */
void _fputbinary(FILE *f, unsigned long int bits, unsigned char nbits) {
    while(nbits--) {
        char c = '0' + ((bits >> nbits) & 1);
        fputc(c, f);
    }
}

/*
 * Print the list of frequencies and binary codes from Huffman coding.
 * @f: the output stream, eg. the stdout
 * @title: print this message before the list (optional)
 * @freql: the frequency list
 */
void freqlist_fprintf(FILE *f, const char *title, const freqlist *freql) {
    if (title)
        fprintf(f, "%s", title);
    fprintf(f, "Symbol    Frequency   Pos   Bits    Binary code\n");
    fprintf(f, "-----------------------------------------------\n");
    for (node_freqlist *pnode_i=freql->list; pnode_i; pnode_i=pnode_i->next) {
        char symb[4];
        char *psymb;
        if (pnode_i->symb < 0x7F && pnode_i->symb >= 0x20) {
            sprintf(symb, "'%c'", pnode_i->symb);
            psymb = symb;
        } else {
            // non-printable symbol
            psymb = " . ";
        }
        fprintf(f, "%s %02X    %9lu    %2X   %4i    ",
                psymb,
                pnode_i->symb,
                pnode_i->freq,
                pnode_i->pos,
                pnode_i->nbits);
        _fputbinary(f, pnode_i->bits, pnode_i->nbits);
        fprintf(f, "\n");
    }
    fprintf(f, "-----------------------------------------------\n");
    fprintf(f, "Size: %lu - Number of symbols: %d\n",
            freql->size, freql->length);
}

void _push(char depth[], char c, int *di) {
    depth[(*di)++] = ' ';
    depth[(*di)++] = c;
    depth[(*di)++] = ' ';
    depth[(*di)++] = ' ';
    depth[*di] = 0;
}
void _pop(char depth[], int *di) {
    depth[(*di)-=4] = 0;
}

void _freqlist_fprintf_tree(FILE *f, node_freqlist *tree, char *depth, int *di, char *start) {
    if (start)
        fprintf(f, "%s", start);
    if (!tree)
        return;
    if (tree->freq)
        fprintf(f, " (%li)", tree->freq);
    if (!tree->zero && !tree->one && *di) {     // No subtrees and not root --> leaf
        fprintf(f, " '%c' [%02X]",
                (tree->symb < 0x7F && tree->symb >= 0x20) ? tree->symb : '.',
                tree->symb);
    }
    fprintf(f, "\n");
    if (tree->zero) {
        fprintf(f, "%s `--", depth);
        _push(depth, '|', di);
        _freqlist_fprintf_tree(f, tree->zero, depth, di, "0");
        _pop(depth, di);
    }
    if (tree->one) {
        fprintf(f, "%s `--", depth);
        _push(depth, ' ', di);
        _freqlist_fprintf_tree(f, tree->one, depth, di, "1");
        _pop(depth, di);
    }
}

/*
 * Print the tree with the frequencies and binary codes from Huffman coding.
 * @f: the output stream, eg. the stdout
 * @title: print this message before the tree (optional)
 * @freql: the frequency list
 */
void freqlist_fprintf_tree(FILE *f, const char *title, const freqlist *freql) {
    if (title)
        fprintf(f, "%s", title);
    int di = 0;
    char depth[DEPTH_BUFFER_SIZE];
    for (int i=0; i < DEPTH_BUFFER_SIZE; i++) depth[i] = '\0';
    _freqlist_fprintf_tree(f, freql->tree, depth, &di, "+");
}

/*
 * Create a new freqlist.
 */
freqlist* freqlist_create() {
    freqlist *l = (freqlist *)malloc(sizeof(freqlist));
    if (l) {
        l->list=l->tree=NULL;
        l->length=0;
        l->size=0L;
    }
    return l;
}


/*
 * Create and return a new node.
 */
node_freqlist* freqlist_create_node(unsigned char c,
                                    unsigned char pos,
                                    unsigned long freq) {
    node_freqlist *pnode = (node_freqlist *)malloc(sizeof(node_freqlist));
    if (!pnode) {
        return NULL;
    }
    pnode->symb=c;
    pnode->pos=pos;
    pnode->freq=freq;
    pnode->next=pnode->prev=pnode->tnext=pnode->zero=pnode->one=NULL;
    return pnode;
}

/* free the memory of all the nodes of the list */
void _freqlist_free_list(node_freqlist *list) {
    node_freqlist *pnode, *pnode_prev;
    pnode = list;
    while (pnode) {
        pnode_prev = pnode;
        pnode = pnode->next;
        free(pnode_prev);
    }
}

/* free the memory of all the "intermediate" nodes of the tree.
   To free the nodes that hold symbols (leaves) use _freqlist_free_list */
void _freqlist_free_tree(node_freqlist *tree) {
    if(tree->zero) _freqlist_free_tree(tree->zero);
    if(tree->one)  _freqlist_free_tree(tree->one);
    if (tree->zero || tree->one) {
        free(tree);
    }
}

/*
 * Free the memory of the list.
 */
void freqlist_free(freqlist* l) {
    if (l->tree) {
        _freqlist_free_tree(l->tree);
    }
    _freqlist_free_list(l->list);
    free(l);
}


/*
 * Return the node with the symbol 'c', or return NULL if not
 * in the list.
 */
node_freqlist *freqlist_find(const freqlist *l, unsigned char c) {
    node_freqlist *pnode = l->list;
    while (pnode && c!=pnode->symb) pnode=pnode->next;
    return pnode;
}



/*
 * Increase the frequency of the symbol 'c' in +1,
 * and rearrange if necessary. If the symbol is not present
 * in the list, add it, and set the frequency of the node in 1.
 * Return the node with the symbol.
 */
node_freqlist *freqlist_add(freqlist *l, unsigned char c) {
    node_freqlist *pnode1, *pnode_prev = NULL;

    pnode1=l->list;
    while (pnode1 && c!=pnode1->symb) {
        pnode_prev=pnode1;
        pnode1=pnode1->next;
    }

    /* After found the symbol in the list, increase its frequency. */
    if (pnode1 && c==pnode1->symb) {
        pnode1->freq++;
        l->size++;
        return pnode1;
    }

    /* If the element doesn't exist, it's added to the end of the list,
       with frequency=1. */
    pnode1 = freqlist_create_node(c, 0, 1l);
    if (pnode1) {
        l->size++;
        l->length++;
        if (pnode_prev) {
            pnode1->pos=pnode_prev->pos + 1;
            pnode1->prev=pnode_prev;
            pnode_prev->next=pnode1;
        } else {
            l->list=pnode1; // First element in the list
        }
    }
    return pnode1;
}


/* Promote the position of the symbol in the list */
void _freqlist_promote(freqlist *l, node_freqlist *pnode) {
    unsigned char i;

    node_freqlist *pnode_prev=pnode->prev,
                  *pnode_next;

    /* While exist a prev node with less frequency than pnode
       or have the same frequency but a lower ASCII code ... */
    while (pnode_prev && node_cmp(pnode, pnode_prev) > 0 ) {
        pnode_prev=pnode_prev->prev;
    }

    if (pnode_prev && pnode_prev != pnode->prev) {
        /* Previous and next of pnode point together */
        (pnode->prev)->next=pnode->next;
        if (pnode->next)
            (pnode->next)->prev=pnode->prev;
        /* pnode points at its new prev and next */
        pnode->prev=pnode_prev;
        pnode_next=pnode->next;
        pnode->next=pnode_prev->next;
        /* The pnode new and prev point to pnode */
        pnode_prev->next=pnode;
        (pnode->next)->prev=pnode;

        /* pnode is assigned to the position it has its next */
        pnode->pos=(pnode->next)->pos;
        /* Increase in +1 the pos variable of all nodes after pnode */
        pnode_prev=pnode;
        while (pnode_prev->next && pnode_prev->next != pnode_next) {
            pnode_prev=pnode_prev->next;
            pnode_prev->pos++;
        }
        return;
    }

    if (!pnode_prev && l->list != pnode) {
        /* The symbol will be the first in the list */
        if (pnode->next)
            (pnode->next)->prev=pnode->prev;

        (pnode->prev)->next=pnode->next;
        pnode->prev=NULL;
        pnode->next=l->list;
        pnode->pos=0;
        l->list->prev=pnode;
        /* Increase in +1 the pos variable of all nodes after pnode */
        l->list=pnode;
        i=1;
        while (pnode->next) {
            pnode=pnode->next;
            pnode->pos=i++;
        }
    }
}


/* This function dis-promotes the position of the symbol in the list */
void _freqlist_dispromote(freqlist *l, node_freqlist *pnode) {
    node_freqlist *pnode_next;

    /* If the node has freq=0 is removed from the list */
    if (pnode->freq == 0) {
        if (pnode->next)
            (pnode->next)->prev=pnode->prev;
        if (pnode->prev)
            (pnode->prev)->next=pnode->next;
        pnode_next=pnode->next;
        if (pnode==l->list) {
            l->list=pnode_next;
        }
        free(pnode);
        while (pnode_next) {
            pnode_next->pos--;
            pnode_next=pnode_next->next;
        }
        l->length--;
    }

    node_freqlist *pnode3=NULL;
    pnode_next=pnode->next;

    /* While exist a next node with more frequency than pnode
       or have same frequency and its ASCII code greater ... */
    while (pnode_next && node_cmp(pnode, pnode_next) < 0 ) {
        pnode3=pnode_next;
        pnode_next=pnode_next->next;
    }

    if (pnode_next && pnode_next != pnode->next) {
        /* Previous and next of pnode1 point together */
        (pnode->next)->prev=pnode->prev;
        if (pnode->prev)
            (pnode->prev)->next=pnode->next;
        /* pnode1 points at its news prev and next */
        pnode->next=pnode_next;
        pnode3=pnode->prev;
        pnode->prev=pnode_next->prev;
        /* The new prev and next point to pnode1 */
        pnode_next->prev=pnode;
        (pnode->prev)->next=pnode;

        /* pnode1 is assigned to the position it has its prev */
        pnode->pos=(pnode->prev)->pos;
        /* Decrease in 1 the pos variable of all nodes before pnode1 */
        pnode_next=pnode;
        while (pnode_next->prev && pnode_next->prev != pnode3) {
            pnode_next=pnode_next->prev;
            pnode_next->pos--;
        }
    }

    /* If pnode_next doesn't exist, means that will be the last in the list,
       but if pnode3 is null, means that was already before this iteration */
    if (!pnode_next && pnode3) {
        /* Decrease in 1 the pos variable of all nodes before pnode1 */
        pnode_next=pnode;
        while (pnode_next->next) {
            pnode_next=pnode_next->next;
            pnode_next->pos--;
        }
        /* pnode3 points at the last of the list */
        pnode3->next=pnode;
        /* Previous and next of pnode1 point together */
        (pnode->next)->prev=pnode->prev;
        if (pnode->prev)
            (pnode->prev)->next=pnode->next;
        /* pnode1 points at its new prev */
        pnode->next=NULL;
        pnode->prev=pnode3;
        pnode->pos=pnode3->pos+1;
    }

    /* The symbol leaves the first position in the table, if it was */
    if (pnode->prev && l->list==pnode) {
        pnode_next=pnode->prev;
        while (pnode_next->prev) {
            pnode_next=pnode_next->prev;
        }
        l->list=pnode_next;
    }
}

/*
 * Swap position with the next element in the list.
 *
 *     pnode0          pnode0    (could be NULL)
 *     pnode1    |-->  *pnode
 *     *pnode  ---     pnode1
 *     pnode2          pnode2    (could be NULL)
 *
 * If pnode0 is NULL, then l->list = pnode (first element
 * in the list).
 *
 * Return 1 if the element were swapped, else 0.
 */
int _freqlist_swap_with_prev(freqlist *l, node_freqlist * pnode) {
    if (pnode->prev) {
        node_freqlist * pnode1 = pnode->prev;
        node_freqlist * pnode0 = pnode1->prev;
        node_freqlist * pnode2 = pnode->next;
        if (pnode0) {
            pnode0->next = pnode;
        } else {
            l->list = pnode;
        }
        pnode->prev = pnode0;
        pnode->next = pnode1;
        pnode1->prev = pnode;
        pnode1->next = pnode2;
        if (pnode2) {
            pnode2->prev = pnode1;
        }
        unsigned char pnode_pos = pnode->pos;
        pnode->pos = pnode1->pos;
        pnode1->pos = pnode_pos;
        return 1;
    }
    return 0;
}


/*
 * Sort the frequencies in case aren't sort.
 * Return the number of swaps made.
 */
int freqlist_sort(freqlist *l) {
    int swaps = 0;
    // Gnome sort implementation
    node_freqlist * pnode = l->list;
    while (pnode) {
        if (pnode == l->list || node_cmp(pnode, pnode->prev) < 0) {
            pnode = pnode->next;
        } else {
            //node_freqlist * pnode_prev = pnode->prev;
            /* swap pnode and pnode->prev */
            _freqlist_swap_with_prev(l, pnode);
            swaps++;
        }
    }
    return swaps;
}

/* Insert the element in the list, sorted by frequency from lower to higher */
void _insert_order(node_freqlist** head, node_freqlist *e) {
    node_freqlist *p = *head, *a = NULL;
    // Find the symbol in the list (sorted by freq)
    while(p && p->freq < e->freq) {
        a = p;                  // Keep the current element to insert after
        p = p->tnext;
    }
    // Insert the element
    e->tnext = p;
    if(a) a->tnext = e;         // Insert between a and p
    else *head = e;             // the new is the first
}

void _build_binary_code(node_freqlist *n, int len, int v)
{
//   if(n->one)  _build_binary_code(n->one, len+1, (v<<1)|1);
//   if(n->zero) _build_binary_code(n->zero, len+1, v<<1);
    if(n->zero) _build_binary_code(n->zero, len + 1, v << 1);
    if(n->one)  _build_binary_code(n->one, len + 1, (v << 1)|1);
    if(!n->zero && !n->one) {
        n->bits = v;
        n->nbits = len;
    }
}

/*
 * Build the Huffman tree and binary
 * codes for encoding.
 * The freqlist has to be sorted first.
 */
int freqlist_build_huff(freqlist *l) {
    if (!l->list) return 0;
    l->tree = l->list;
    node_freqlist *p = l->list;
    while (p) {
        p->tnext = p->next;
        p = p->next;
    }
    while (l->tree && l->tree->tnext) {              // While exist at least 2 elements in the list
        p = (node_freqlist *)malloc(sizeof(node_freqlist));     // A new tree node (that is a sub-tree)
        if (!p) return ERROR_MEM;
        p->symb = 0;                                            // Does not correspond to any symbol
        p->one = l->tree;                                       // Branch one
        l->tree = l->tree->tnext;                               // Next node
        p->zero = l->tree;                                      // branch zero
        l->tree = l->tree->tnext;                               // Next node
        p->freq = p->zero->freq + p->one->freq;                 // Sum of frequencies
        _insert_order(&(l->tree), p);                           // Insert in new node
    }
    _build_binary_code(l->tree, 0, 0);
    return OK;
}


/*
 * Compare by frequency of the node,
 * or symbol if the frequencies are equal.
 */
int node_cmp(const node_freqlist *pnode1, const node_freqlist *pnode2) {
    if ( pnode1->freq < pnode2->freq
         || (pnode1->freq == pnode2->freq && pnode1->symb > pnode2->symb) ) {
        return 1;
    }
    return -1;
}
