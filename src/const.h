/* const.h

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
#define INVALID_FILE_IN                 7       /* Cannot open output file */
#define ERROR_UNKNOWN                   50      /* Unknown error */

#define OUTPUT_EXT                      ".ah"   /* Default output file name extension. */


#define DEPTH_BUFFER_SIZE               2048    /* 2K buffer used when printing the
                                                   Huffman tree */
#define BUFFER_WINDOW                   8192    /* 8K initial buffer size and window
                                                   size to reallocate when using
                                                   stdin as a source */

#define VERBOSE_TABLE                   "> Frequency table and Huffman coding\n"
#define VERBOSE_TREE                    "> Tree Huffman coding\n"

#endif /* __FQ_CONST_H */
