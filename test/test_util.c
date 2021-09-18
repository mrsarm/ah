/* test_util.c

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


#include <string.h>
#include <util.h>
#include "cheat.h"


/***************
 *  Test strings
 ***************/

CHEAT_TEST(rmsub_expected_null_too_small,
    cheat_assert( rmsub("small.ax", "largefilename") == NULL );
)

CHEAT_TEST(rmsub_expected_not_substring,
    cheat_assert( rmsub("file.txt", ".doc") == NULL );
)

CHEAT_TEST(rmsub_expected_string_substring_ok_with_more_ext,
    cheat_assert( strcmp(rmsub("file.txt.av", ".av"), "file.txt") == 0 );
)
