/**************************************************************
 *
 *                           dctrans.h
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     This class contains two functions – dct and reverse_dct. The dct
 *     function performs a Discrete Cosine Transform to convert the luma
 *     values of a given 2-by-2 block of pixels into a, b, c, and d
 *     components, while the reverse_dct function performs the reverse
 *     operation, converting a, b, c, and d components into four luma
 *     values corresponding to the luma values of pixels in a 2-by-2 block
 *     
 **************************************************************/
#ifndef DCTRANS_INCLUDED
#define DCTRANS_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <a2methods.h>
#include <a2plain.h>
#include <uarray.h>

#include "codeword.h"
#include "colorspace.h"

/* dct
 * Purpose: Does the math to DCT the y values in a block of 4
 *          YPbPr structs and set them in as a, b, c, and d in a
 *          codeword struct.
 * Parameters: A UArray of YPbPr structs and a codeword struct
 * Returns: nothing
 *
 * Expected input: An array of 4 full ypbpr structs and an empty codeword
 *                  struct.
 * Success output: Will correctly initialize a, b, c, and d in the codeword
 *                  struct.
 * Failure output: Will raise an exception if any of the supplied pointer
 *                  parameters are null.
 */
void dct(UArray_T block_array, Codeword cw);

/* reverse_dct
 * Purpose: Does the math to reverse DCT the a, b, c, and d values in a
 *          codeword struct and sets the corrosponding y values in
 *          4 ypbpr structs in a UArray
 * Parameters: A UArray of YPbPr structs and a codeword struct
 * Returns: nothing
 *
 * Expected input: An array of 4 empty ypbpr structs and a full codeword
 *                  struct.
 * Success output: Will correctly initialize the y values in the ypbpr
 *                  struct.
 * Failure output: Will raise an exception if any of the supplied pointer
 *                  parameters are null.
 */
void reverse_dct(Codeword cw, UArray_T block_array);

#endif