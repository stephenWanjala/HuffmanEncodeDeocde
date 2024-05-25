# Huffman Coding Project

This repository contains a C implementation of Huffman Coding for encoding and decoding text files. Huffman coding is a lossless data compression algorithm that uses variable-length codes to represent characters based on their frequencies in the input text.

## Overview

This project includes the following features:

- **Encoding**: Compress a text file using Huffman coding and generate a code table.
- **Decoding**: Decompress an encoded file using the code table and reconstruct the original text.

## Files

- `main.c`: The main C file containing the implementation of Huffman coding.


## Compilation

To compile the project, use the following command:

```sh
gcc -o huffman main.c
```

Usage

The program can be run in two modes: encode and decode.
Encoding

To encode a text file:

```sh

./huffman encode <input_text_file> <output_code_table_file> <output_encoded_file>
```

   ` <input_text_file>`: Path to the input text file to be encoded.
    `<output_code_table_file>`: Path to the output file where the code table will be stored.
    `<output_encoded_file>`: Path to the output file where the encoded text will be stored.

Example:

```sh

./huffman encode input.txt codetable.txt encoded.txt
```

Decoding

To decode an encoded file:

```sh

./huffman decode <input_code_table_file> <input_encoded_file> <output_decoded_file>
```

    <input_code_table_file>: Path to the input file containing the code table.
    <input_encoded_file>: Path to the input file containing the encoded text.
    <output_decoded_file>: Path to the output file where the decoded text will be stored.

Example:

```sh

./huffman decode codetable.txt encoded.txt decoded.txt
```

Detailed Explanation
Encoding Process

    Frequency Calculation: The program reads the input text file and calculates the frequency of each character.
    Huffman Tree Construction: A min-heap is used to construct the Huffman tree based on character frequencies.
    Code Generation: The program generates variable-length binary codes for each character based on the Huffman tree.
    Encoding: The input text is encoded using the generated codes and written to the output encoded file. The code table is also saved to a file.

Decoding Process

    Code Table Reading: The program reads the code table from the file.
    Huffman Tree Reconstruction: The Huffman tree is reconstructed using the code table.
    Decoding: The encoded text is read and decoded using the Huffman tree, then the original text is written to the output decoded file.

Compression Statistics

After encoding, the program prints compression statistics including the original size, compressed size, and compression ratio.
