/* test_ah.c

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


#include <string.h>
#include "cheat.h"
#include "const.h"
#include "ah.h"
#include "util_t.h"


CHEAT_DECLARE(
    ah_data* data;
)

CHEAT_TEAR_DOWN(
    if (data) {
        ah_data_free_resources(data);
    }
)


/***************
 *  DATA SET 1
 ***************/
CHEAT_DECLARE(
    unsigned char buff_1[] =            { 4, 2, 3, 3, 6, 3, 4, 2, 6, 8 };
    unsigned int expected_ah_1[][2] =   { {8,1}, {6,2}, {4,2}, {2,2}, {3,3} };
)
CHEAT_TEST(expected_buff_out_ok,
    data=count_buff(buff_1, ARRAY_SIZE(buff_1), FALSE);
    cheat_assert(  freqlist_check(data->freql, expected_ah_1, ARRAY_SIZE(expected_ah_1))  );
)


/***************
 *  DATA SET 2
 ***************/
CHEAT_DECLARE(
    unsigned char buff_2[] =            { 0, 5, 3, 2, 2, 8, 5, 0, 6, 18, 10, 0 };
    unsigned int expected_ah_2[][2] =   { {18,1}, {10,1}, {8,1}, {6,1}, {3,1}, {5,2}, {2,2}, {0,3} };
)
CHEAT_TEST(expected_buff_out_2_ok,
   data=count_buff(buff_2, ARRAY_SIZE(buff_2), FALSE);
   cheat_assert(  freqlist_check(data->freql, expected_ah_2, ARRAY_SIZE(expected_ah_2))  );
)


/****************************
 *  DATA SET 3: word "banana"
 ****************************/
CHEAT_DECLARE(
    unsigned char buff_3[] =            { 'b', 'a', 'n', 'a', 'n', 'a' };
    unsigned int expected_ah_3[][2] =   { {'b',1} ,{'n',2}, {'a',3} };
)
CHEAT_TEST(expected_buff_out_3_ok,
    data=count_buff(buff_3, ARRAY_SIZE(buff_3), FALSE);
    cheat_assert(  freqlist_check(data->freql, expected_ah_3, ARRAY_SIZE(expected_ah_3))  );
)


/****************************
 *  DATA SET 4: empty set
 ****************************/
CHEAT_DECLARE(
    unsigned char buff_4[] =            { };
    unsigned int expected_ah_4[][2] =   { };
)
CHEAT_TEST(expected_buff_out_4_ok,
    data=count_buff(buff_4, ARRAY_SIZE(buff_4), FALSE);
    cheat_assert(  freqlist_check(data->freql, expected_ah_4, ARRAY_SIZE(expected_ah_4))  );
)


/****************************
 *  DATA SET 5: only 1 symbol
 ****************************/
CHEAT_DECLARE(
    unsigned char buff_5[] =            { 0 };
    unsigned int expected_ah_5[][2] =   { {0, 1} };
)
CHEAT_TEST(expected_buff_out_5_ok,
    data=count_buff(buff_5, ARRAY_SIZE(buff_5), FALSE);
    cheat_assert(  freqlist_check(data->freql, expected_ah_5, ARRAY_SIZE(expected_ah_5))  );
)
