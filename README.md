"Another Huffman" encoder
=========================

Command line tool "ah": **Another Huffman** encoder to compress and decompress
files, with the cool addition of the verbose mode, that display useful
information like the encoding table and the Huffman binary tree.

Encode the word _"banana"_ from the input stream, show encoding info and store
the encoded result in `banana.ah`:

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

The output can be redirected to the standard output with `-c`, while in
verbose mode (`-v`) the debug information goes to the standard error stream.
Also using `-` as file argument, the data is taken from the standard
input stream.

To decode a file or the input stream, use `-d`:

    $ ah README.md          # Encode and store in README.md.av
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

   To install into another folder, execute the step 1
   overwriting the `CMAKE_INSTALL_PREFIX` variable, eg.
   to install into the `/usr/bin` folder:

       $ cmake -DCMAKE_INSTALL_PREFIX=/usr .

To clean the compiled binaries and compiled modules:

    $ make clean

To clean all the compiled binaries and the "Makefiles" files:

    $ make clean-all


### Tests

Tests are built with CHEAT <https://github.com/Tuplanolla/cheat>,
a unit testing framework for C/C++, and can be executed
with CMake CTest:

    $ make test

To run the tests and see all the output in the console (specially
if the tests fail), first build the project (`make`), and then execute:

    $ out/test_ah && out/test_huff


About
-----

Source: https://github.com/mrsarm/ah

_Source code based on_:

- Huffman coding: based in the source code of the [compres](https://github.com/mrsarm/compres)
  tool written originally by Salvador Pozo Coronado (2000).
- Frequency table generation: [fq](https://github.com/mrsarm/fq) CLI.
- Printing of the Huffman tree: algorithm based in the code from Randy Gaul's Game
  Programming Blog (2015) [Printing Pretty Ascii Trees](https://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/).

Authors: (2021) Mariano Ruiz <mrsarm@gmail.cm>

License: LGPL-3
