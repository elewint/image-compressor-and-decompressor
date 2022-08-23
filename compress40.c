/**************************************************************
 *
 *                     compress40.c
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *    Summary
 *    The implementation of our compress40 class. Contains the
 *    compress40 and decompress40 functions. The compress40
 *    function takes a ppm file and compresses it into the
 *    COMP40 Compressed Image Format, while the decompress40
 *    function reads in a COMP40 Compressed Image Format
 *    file and decompresses it into a ppm file.
 *     
 **************************************************************/
#include <string.h>
#include <stdlib.h>

#include <assert.h>
#include <compress40.h>
#include <a2methods.h>
#include <a2plain.h>
#include <uarray.h>
#include <pnm.h>
#include <bitpack.h>

#include "colorspace.h"
#include "quantize.h"
#include "codeword.h"
#include "dctrans.h"


Pnm_ppm trim(Pnm_ppm ppm);

void quantizer(Pnm_ppm ppm, A2Methods_UArray2 ypbpr_array,
                    A2Methods_UArray2 cw_array, A2Methods_T methods);
void reverse_quantizer(Pnm_ppm ppm, A2Methods_UArray2 ypbpr_array,
                     A2Methods_UArray2 cw_array, A2Methods_T methods);

void write_compressed_file(Pnm_ppm ppm, A2Methods_UArray2 word_array);
Pnm_ppm read_compressed_header(FILE *input);
A2Methods_UArray2 read_compressed_words(FILE *input, Pnm_ppm image);

/* compress40
 * Purpose: Reads a file and compresses a ppm from within that file
 * Parameters: A file pointer
 * Returns: nothing
 *
 * Expected input: A file containing a valid ppm
 * Success output: Prints the compressed output to stdout
 * Failure output: Will raise an exception through Pnm_ppmread if
 *                  the ppm supplied is not in the proper format
 */
void compress40(FILE *input)
{
    assert(input != NULL);

    A2Methods_T methods = uarray2_methods_plain; 
    assert(methods);

    /* Read PPM */
    Pnm_ppm image = Pnm_ppmread(input, methods);
    
    /* Trim PPM */
    image = trim(image);
    
    /* Convert RGB to YPbPr */
    A2Methods_UArray2 ypbpr_array = 
                                convert_rgb_to_ypbpr(image, methods);
    
    /* Quantize PbPr values */
    int width = image->width;
    int height = image->height;
    A2Methods_UArray2 cw_array = methods->new(width / 2, height / 2,
                                                size_of_codeword());

    quantizer(image, ypbpr_array, cw_array, methods);

    A2Methods_UArray2 word_array = methods->new(width / 2, height / 2,
                                                    sizeof(uint32_t));
    bitpack_codewords(cw_array, word_array);

    /* Write ppm to stdout */
    write_compressed_file(image, word_array);

    /* Free functions */
    methods->free(&word_array);
    methods->free(&ypbpr_array);
    methods->free(&cw_array);
    Pnm_ppmfree(&image);
}

/* compress40
 * Purpose: Reads a file and compresses a ppm from within that file
 * Parameters: A file pointer
 * Returns: nothing
 *
 * Expected input: A file containing a valid ppm
 * Success output: Prints the compressed output to stdout
 * Failure output: Will raise an exception through Pnm_ppmread if
 *                  the ppm supplied is not in the proper format
 */
void decompress40(FILE *input)
{
    assert(input != NULL);

    A2Methods_T methods = uarray2_methods_plain; 
    assert(methods);

    Pnm_ppm image = read_compressed_header(input);
    image->methods = methods;
    A2Methods_UArray2 word_array = read_compressed_words(input, image);

    int width = image->width;
    int height = image->height;

    A2Methods_UArray2 cw_array = methods->new(width, height,
                                             size_of_codeword());
    unpack_codewords(word_array, cw_array);

    A2Methods_UArray2 ypbpr_array = methods->new(width, height,
                                                 size_of_ypbpr());

    reverse_quantizer(image, ypbpr_array, cw_array, methods);

    A2Methods_UArray2 rgb_array = 
                        convert_ypbpr_to_rgb(ypbpr_array, methods);

    image->pixels = rgb_array;
    Pnm_ppmwrite(stdout, image);

    /* Free functions */
    methods->free(&word_array);
    methods->free(&ypbpr_array);
    methods->free(&cw_array);
    Pnm_ppmfree(&image);
}

/* trim
 * Purpose: Trims a ppm such that its width and height become even numbers
 * Parameters: A ppm
 * Returns: A ppm
 *
 * Expected input: A valid ppm
 * Success output: A ppm with even width and height values
 * Failure output: Will raise an exception if the ppm supplied is NULL
 */
Pnm_ppm trim(Pnm_ppm ppm)
{
    assert(ppm != NULL);

    int width = ppm->width;
    int height = ppm->height;

    if (width % 2 != 0) {
        width--;
        ppm->width = width;
    }

    if (height % 2 != 0) {
        height--;
        ppm->height = height;
    } 
        
    A2Methods_T methods = uarray2_methods_plain; 

    A2Methods_UArray2 trimmed_pixels = methods->new(width, height,
                                          sizeof(struct Pnm_rgb));
    A2Methods_UArray2 old_pixels = ppm->pixels;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            *(Pnm_rgb)methods->at(trimmed_pixels, i, j) =
                                   *(Pnm_rgb)methods->at(old_pixels, i, j);
        }
    }

    ppm->pixels = trimmed_pixels;
    methods->free(&old_pixels);
        
    return ppm;
}

/* quantizer
 * Purpose: Quantizes the Pb and Pr values and DCTs the y values in an array
 *          of YPbPr structs
 * Parameters: A ppm, a UArray2 of ypbpr structs, a UArray2 of codeword
 *             structs, and methods
 * Returns: nothing
 *
 * Expected input: A valid ppm, a valid 2d array of ypbpr structs, a valid
 *                 2d array of codeword structs, and methods
 * Success output: Will correctly set the a, b, c, d, pb_index, and pr_index
 *                  values in each codeword struct in the 2d array of
 *                  codeword structs.
 * Failure output: Will raise an exception if any of the supplied pointer
 *                  parameters are null.
 */
void quantizer(Pnm_ppm ppm, A2Methods_UArray2 ypbpr_array,
                 A2Methods_UArray2 cw_array, A2Methods_T methods)
{
    assert(ppm != NULL);
    assert(ypbpr_array != NULL);
    assert(cw_array != NULL);
    assert(methods != NULL);

    int width = ppm->width;
    int height = ppm->height;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            /* Top-left corner */
            if (i % 2 == 0 && j % 2 == 0) {
                UArray_T block_array = make_block_array(ypbpr_array, j, i,
                                                                 methods);

                Codeword cw = malloc(size_of_codeword());
                assert(cw);

                pb_pr_quantize(block_array, cw);
                dct(block_array, cw);

                set_cw_array(cw_array, j / 2, i / 2, cw, methods);

                free(cw);
                UArray_free(&block_array);
            }
        }
    }
}

/* reverse_quantizer
 * Purpose: Reverse quantizes the pb and pr indicies and reverse DCTs the
 *          a, b, c, and d values in an array of codeword structs
 * Parameters: A ppm, a UArray2 of ypbpr structs, a UArray2 of codeword
 *             structs, and methods
 * Returns: nothing
 *
 * Expected input: A valid ppm, a valid 2d array of ypbpr structs, a valid
 *                 2d array of codeword structs, and methods
 * Success output: Will correctly set the y, pb, and pr values in each ypbpr
 *                   struct in the 2d array of ypbpr structs
 * Failure output: Will raise an exception if any of the supplied pointer
 *                  parameters are null.
 */
void reverse_quantizer(Pnm_ppm ppm, A2Methods_UArray2 ypbpr_array,
                         A2Methods_UArray2 cw_array, A2Methods_T methods)
{
    assert(ppm != NULL);
    assert(ypbpr_array != NULL);
    assert(cw_array != NULL);
    assert(methods != NULL);

    int width = ppm->width;
    int height = ppm->height;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            /* Top-left corner */
            if (i % 2 == 0 && j % 2 == 0) {
                UArray_T block_array = make_block_array(ypbpr_array, j, i,
                                                                 methods);

                Codeword cw = (Codeword)methods->at(cw_array, j / 2, i / 2);

                pb_pr_reverse_quantize(cw, block_array);
                reverse_dct(cw, block_array);

                set_ypbpr_block(ypbpr_array, j, i, methods, block_array);

                UArray_free(&block_array);
            }
        }
    }
}

/* write_compressed_file
 * Purpose: Writes a compressed ppm to stdout
 * Parameters: A ppm, a UArray2 of words
 * Returns: nothing
 *
 * Expected input: A valid ppm and a valid 2d array of words
 * Success output: Will print the array of words to stdout in
 *                  comp40 compressed image format
 * Failure output: Will raise an exception if any of the supplied pointer
 *                  parameters are null.
 */
void write_compressed_file(Pnm_ppm ppm, A2Methods_UArray2 word_array)
{
    assert(ppm != NULL);
    assert(word_array != NULL);

    unsigned width = ppm->width;
    unsigned height = ppm->height;
    printf("COMP40 Compressed image format 2\n%u %u\n", width, height);
    
    print_codewords(word_array);
}

/* read_compressed_header
 * Purpose: Reads in the header of a file containing a comp40 compressed
 *          image and returns a ppm with the width and height values from
 *          that header initialized
 * Parameters: A file pointer
 * Returns: A ppm
 *
 * Expected input: A file containing a comp40 compressed image
 * Success output: A ppm with width and height values initialized with the
 *                  width and height of the comp40 compressed image, and
 *                  a denominator of 200
 * Failure output: Will raise an exception if any of the supplied pointer
 *                  parameters are null.
 */
Pnm_ppm read_compressed_header(FILE *input)
{
    assert(input != NULL);

    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                                                     &width, &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    Pnm_ppm image = malloc(sizeof(struct Pnm_ppm));
    assert(image);

    image->width = width;
    image->height = height;
    image->denominator = 200;

    return image;
}

/* read_compressed_words
 * Purpose: Reads the body of a comp40 compressed image into a UArray2
 *          and returns that array
 * Parameters: A file pointer and a ppm
 * Returns: A Uarray2 of words
 *
 * Expected input: A file pointer that points to the body of a comp40
 *                  compressed image and a ppm that has only width, height,
 *                  and denominator initialized
 * Success output: A UArray2 of words identical to the ones read from the
 *                  file
 * Failure output: Will raise an exception if any of the supplied pointer
 *                  parameters are null.
 */
A2Methods_UArray2 read_compressed_words(FILE *input, Pnm_ppm image)
{
    assert(image != NULL);
    assert(input != NULL);

    int width = image->width;
    int height = image->height;

    A2Methods_T methods = uarray2_methods_plain;

    A2Methods_UArray2 word_array = methods->new(width / 2, height / 2,
                                                    sizeof(uint32_t));

    for (int i = 0; i < height / 2; i ++) {
        for (int j = 0; j < width / 2; j++) {
            uint32_t curr_word = 0;
            
            for (int k = 24; k >= 0; k -= 8) {
                uint32_t c = fgetc(input);
                curr_word = Bitpack_newu(curr_word, 8, k, c);
            }

            *(int32_t *)methods->at(word_array, j, i) = curr_word;
        }
    }

    return word_array;
}
