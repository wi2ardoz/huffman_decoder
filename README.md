# Huffman-Decoder
#
Title:			Huffman Decoder
Date:		    09.06.18 
Author:		    Liad Oz
Description:
                FUNCTIONS
                ---------
                This program creates huffman tree by given database of chars and
                it capable of doing few acts on it:
                option 1 => encode text using the Huffman Tree
                option 2 => bit-encode text using the Huffman Tree
                option 3 => decode using the Huffman Tree
                option 4 => bit-decode using the Huffman Tree
                option 5 => serialize the Huffman Tree / printing the leaves

                NOTES
                -----
                1) Tree memory must contain '@' char.
                2) At the end of each encoded string must be '@' char.
                3) Bit encode won't encode if one or more chars will be encoded to null.
