# Huffman Compression

## Introduction

Huffman code is a particular type of optimal prefix code that is commonly used for lossless data compression.

## Functionality

This program creates huffman tree by given database of chars and
it capable of the following operations:

1. Encode text using the Huffman Tree
2. Bit-encode text using the Huffman Tree
3. Decode using the Huffman Tree
4. Bit-decode using the Huffman Tree
5. Serialize the Huffman Tree / printing the leaves

## NOTES

* Tree memory must contain '@' char.
* At the end of each encoded string must be '@' char.
* Bit encode won't encode if one or more chars will be encoded to null.
