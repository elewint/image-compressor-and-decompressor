# COMP 40 hw04: arith

Eli Intriligator and Max Behrendt

Date: Oct 28 2021

TA help acknowledgements: Dylan, Alex, Gustavo, Igor, Tina, Ann-Marie, Miles, Gustavo, Mary-Joy

## Purpose

The main task of this program is to compress a PPM image into the
COMP40 Compressed Image Format and to fully decompress an image in
the COMP40 Compressed Image Format back into a PPM image. The
compression is lossy, however the effects of the compression are not
visible on most images and the compressed format only uses approximately
one third of the space required to store the input PPM image. We use
component video space conversion, quantization of chroma values,
discrete cosine transforms, and bitpacking to accomplish these results.

## Architecture

This program uses a series of classes that break down compression and
decompression processes. The colorspace class is used to convert 2d arrays
of rgb structs into 2d arrays of ypbpr structs, and also do the reverse.
The quantize class is used to quantize the pb and pr values that are
stored in ypbpr structs, and also reverse quantize them. The dctrans
class takes the y values from ypbpr structs and uses dct to convert
them to a, b, c, and d values, and is also used to convert these values
back into y values. The bitpack class is used to pack the quantized
indicies of pb and pr and a, b, c, and d values into single words. It
is also used to unpack these values. compress40.c uses all of these
classes to either compress or decompress the given input.

## Known problems/limitations

We believe we have implemented all features correctly.

---

Time spent analyzing problems posed in the assignment:
    12 hours

Time spent solving the problems after our analysis:
    32 hours
