/* util.h

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


#ifndef __AH_UTIL_H
#define __AH_UTIL_H


/*
 * Print an error message with error_code and from string in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void error_unknown_code(int error_code, char *from,
                        void(free_resources)(void*), void* data);

/*
 * Print an error message in the stderr that the file cannot be opened, and abort
 * the program after invoking the free_resources function.
 */
void error_cannot_open(int error_code, char *file_type, char *filename,
                       void(free_resources)(void*), void* data);

/*
 * Print an error message in the stderr that the file is invalid, and abort
 * the program after invoking the free_resources function.
 */
void error_invalid_file_in(int error_code, char *file_type, char *filename,
                           void(free_resources)(void*), void* data);

/*
 * Print error_code in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void fatal(int error_code,
           char *error_msg,
           void(free_resources)(void*), void* data);


/*
 * Print an insufficient memory error in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void error_mem(void(free_resources)(void*), void* data);


/*
 * Print an invalid number of bits used by a symbol error in the stderr,
 * and abort the program after invoking the free_resources function.
 */
void error_invalid_nbits(void(free_resources)(void*), void* data);


/*
 * Concatenate s1 and s2 in a new string.
 */
char *cat(char *s1, char *s2);


/*
 * Return a new string like s1, removing last
 * appearance of s2 in s1.
 * Return NULL if s2 is not ending s1.
 */
char *rmsub(char *s1, char *s2);


#endif /* __AH_UTIL_H */
