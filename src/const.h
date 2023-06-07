/* const.h

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


#ifndef __FQ_CONST_H
#define __FQ_CONST_H


#ifndef TRUE

#define TRUE                            1
#define FALSE                           0

#endif /* TRUE */

#define OK                              0       /* Functions return 0 as success code */
#define ERROR_MEM                       2       /* Insufficient memory error. */
#define ERROR_PARAM                     3       /* Command line parametrization error. */
#define ERROR_FILE_NOT_FOUND            5       /* The input file is not found or can not
                                                   be opened. */
#define ERROR_FILE_OUT                  6       /* Cannot open output file */
#define INVALID_FILE_IN                 7       /* Cannot open input file */
#define INVALID_BITS_SIZE               8       /* Invalid number of bits to encode a symbol */
#define ERROR_UNKNOWN                   50      /* Unknown error */

#define OUTPUT_EXT                      ".ah"   /* Default output file name extension. */

#define MAGIC_NUMBER                    "\x0f\xa1"  /* 2 bytes identifier of the file format */
#define HEADER_COO_VERSION              1       /* Version of the format used */
#define HEADER_COO_VERSION_BITS         3       /* Bits used in the header to store the version
                                                   of the format used */
#define MAGIC_NUMBER_SIZE               2
#define NUMBER_SIZE                     8       /* Bytes used to store big numbers in output
                                                   (same than bytes used by the long int type
                                                   in most platforms) */
#define SMALL_COUNT_SIZE                2       /* Bytes used to store short numbers in output
                                                   to count (same than bytes used by the short type
                                                   in most platforms) */
#define COUNT_SIZE                      4       /* Bytes used to store numbers in output to count
                                                   (same than bytes used by the int type
                                                   in most platforms) */
#define SYMBOL_SIZE                     1       /* Bytes used by one symbol (one char) */


#define DEPTH_BUFFER_SIZE               2048    /* 2K buffer used when printing the
                                                   Huffman tree */
#define BUFFER_WINDOW                   8192    /* 8K initial buffer size and window
                                                   size to reallocate when using
                                                   stdin as a source */

#define VERBOSE_TABLE                   "> Frequency table and Huffman coding\n"
#define VERBOSE_TREE                    "> Tree Huffman coding\n"

#endif /* __FQ_CONST_H */
