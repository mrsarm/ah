"Another Huffman" encoder
=========================

Command line tool "ah": **Another Huffman** encoder to compress and decompress
files, with the cool addition of the verbose mode, that display useful
information like the encoding table, the Huffman binary tree and
a compression summary.

Let's encode the word _"banana"_ from the input stream, show encoding
info and store the encoded result in the `banana.ah` file:

    $ echo -n banana | ah -v - > banana.ah
    > Frequency table and Huffman coding
    Symbol    Frequency   Pos   Bits    Binary code
    -----------------------------------------------
    'b' 62            1     0      2    11
    'n' 6E            2     1      2    10
    'a' 61            3     2      1    0
    -----------------------------------------------
    Size: 6 - Number of symbols: 3
    
    > Tree Huffman coding
    + (6)
     `--0 (3) 'a' [61]
     `--1 (3)
         `--0 (2) 'n' [6E]
         `--1 (1) 'b' [62]

    [ Summary ] ===================================
    Uncompressed size: 6
    Compressed size (without headers): 2
    Ratio (without headers): 0.333333
    ===============================================


The output can be redirected to the standard output with `-c`, while in
verbose mode (`-v`) the debug information goes to the standard error stream.
Also using `-` as file argument, the data is taken from the standard
input stream like the example above.

To decode a file or the input stream, use `-d`:

    $ ah README.md          # Encode and store in README.md.ah
    $ ah -d README.md.av    # Decode and store in README.md

Check all the options available with `ah -h`.


Build and execute
-----------------

The project uses *CMakes* to build the executable.

1. From the source directory, generate the "Makefile":

       $ cmake .

2. *Make* the project and generate the executable "ah".

       $ make

3. Then execute with (use with `-h` to see available options):

       $ out/ah

4. To install the executable `ah` into `/usr/local/bin`:

       $ sudo make install

   To install into another folder, when executing the step 1
   overwrite the `CMAKE_INSTALL_PREFIX` variable, eg.
   to install into the `/usr/bin` folder:

       $ cmake -DCMAKE_INSTALL_PREFIX=/usr .

To clean the compiled binaries and compiled modules:

    $ make clean

To clean all the compiled binaries and the "Makefiles" files:

    $ make clean-all


### Tests

Tests are built with CHEAT <https://github.com/Tuplanolla/cheat>,
a unit testing framework for C/C++. There are also tests built
with Bash scripts. All of them can be executed
with CMake CTest:

    $ make test

To run the tests and see all the output in the console (specially
if the tests fail), first build the project (`make`), and then execute:

    $ out/test_ah && out/test_huff && out/test_util

To execute just one of the bash scripts, simply run any of them
after build the project, e.g.:

    $ ./test/scripts/test_enc_stream.sh

About
-----

Source: https://github.com/mrsarm/ah

The intention of this project was to learn more about how to build
compression algorithms, and specifically the Huffman algorithm, and
how to be more proficient coding in C and in Unix systems. It was not the
intention to build a _production ready_ CLI, although it does work, but
other compression tools that use more algorithms not just Huffman are
out there that are also more _battle tested_.

**Source code based on**:

- Huffman coding: based in the source code of the [compres](https://github.com/mrsarm/compres)
  tool written originally by Salvador Pozo Coronado (2000).
- Frequency table generation: [fq](https://github.com/mrsarm/fq) CLI.
- Printing of the Huffman tree: algorithm based in the code from Randy Gaul's Game
  Programming Blog (2015) [Printing Pretty Ascii Trees](https://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/).

Authors: (2021-2024) Mariano Ruiz <mrsarm@gmail.cm>

License: LGPL-3
