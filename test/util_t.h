/* test_util.h

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


#ifndef _AH_UTIL_T_H
#define _AH_UTIL_T_H

#include <ah.h>

#define ARRAY_SIZE(arr)             sizeof(arr) / sizeof((arr)[0])


/*
 * Count the buffer passed and return the ``ah_data`` with the output data.
 */
ah_data* count_buff(unsigned char* buff_in, unsigned int buff_in_length, int verbose);


/*
 * Free only the resources used in test mode.
 */
void free_resources(ah_data *data);


/*
 * Check if plist is equals to expected_freqlist (second list is in
 * array format).
 */
int freqlist_check(const freqlist* plist, const unsigned int expected_freqlist[][2],
				   unsigned int expected_length);


/*
 * Check if plist tree data is equals to expected_freqlist (second list is in
 * array format).
 */
int freqlist_check_tree(const freqlist* plist, const unsigned int expected_freqlist[][3],
                        unsigned int expected_length);


#endif  /* _AH_UTIL_T_H */
