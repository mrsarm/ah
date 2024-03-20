/* test_huff.c

   Copyright (C) 2021-2024 Mariano Ruiz <mrsarm@gmail.com>
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
#include <cheat.h>
#include <ah.h>
#include "const.h"
#include "util_t.h"


CHEAT_DECLARE(
    ah_data* data;
)

CHEAT_TEAR_DOWN(
    if (data) {
        ah_data_free_resources(data);
    }
)

/****************************
 *  DATA SET 3: word "banana"
 ****************************/
CHEAT_DECLARE(
    unsigned char buff_3[] =            { 'b', 'a', 'n', 'a', 'n', 'a' };
    unsigned int expected_ah_3[][3] =   { {'b',2,3} ,{'n',2,2}, {'a',1,0} };
)
CHEAT_TEST(expected_build_huff_3_ok,
    data=count_buff(buff_3, ARRAY_SIZE(buff_3), FALSE);
    freqlist_build_huff(data->freql);
    fprintf(stderr, "\n");
    freqlist_fprintf_tree(stderr, VERBOSE_TREE, data->freql);
    cheat_assert(  freqlist_check_tree(data->freql, expected_ah_3, ARRAY_SIZE(expected_ah_3))  );
)

/****************************
 *  DATA SET 4: phrase "ata la jaca a la estaca"
 ****************************/
CHEAT_DECLARE(
    char *buff_4 =                      "ata la jaca a la estaca";
    unsigned int expected_ah_4[][3] =   { {'s',6,1}, {'j',6,0}, {'e',5,1}, {'t',4,3},
                                          {'l',4,2}, {'c',4,1}, {' ',2,1}, {'a',1,1} };
)
CHEAT_TEST(expected_build_huff_4_ok,
    data=count_buff((unsigned char*)buff_4, strlen(buff_4), FALSE);
    freqlist_build_huff(data->freql);
    fprintf(stderr, "\n");
    freqlist_fprintf_tree(stderr, VERBOSE_TREE, data->freql);
    cheat_assert(  freqlist_check_tree(data->freql, expected_ah_4, ARRAY_SIZE(expected_ah_4))  );
)
