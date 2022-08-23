/**************************************************************
 *
 *                     quantize.h
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     This class contains two quantization functions – pb_pr_quantize and
 *     pb_pr_reverse_quantize. The pb_pr_quantize function allows the user
 *     to quantize the chroma components of a block of pixels in the
 *     component video colorspace into indices in an internal table, while
 *     the pb_pr_reverse_quantize function effectively performs the reverse
 *     operation, converting quantized Pb and Pr values into actual values.
 *     
 **************************************************************/
#ifndef QUANTIZE_INCLUDED
#define QUANTIZE_INCLUDED
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <a2methods.h>
#include <a2plain.h>
#include <uarray.h>
#include <pnm.h>
#include <arith40.h>

#include "codeword.h"
#include "colorspace.h"

/* pb_pr_quantize
 * Purpose: Quantize the Pb and Pr component video colorspace components
 *          in a 2-by-2 block of pixels in an image, storing the
 *          quantized values (which are indices in an internal table)
 *          in a Codeword provided by the user
 * Parameters: a 1D array that holds the block of pixels and a Codeword
 *             where the quantized Pb and Pr components will be stored
 * Returns: void
 *    Note: the 1D block_array stores a block of pixels in the following
 *          order: (top-left, top-right, bottom-left, bottom-right)
 *
 * Expected input: a valid 1D block array (with initialized Pb and Pr
 *                 values) and a valid Codeword to store quantized values
 * Success output: the quantized values will be stored in the Codeword
 * Failure output: if any of the parameters are invalid, a Checked Runtime
 *                 Error is thrown
 */
void pb_pr_quantize(UArray_T block_array, Codeword cw);

/* pb_pr_reverse_quantize
 * Purpose: Convert the quantized Pb and Pr values held in a
 *          user-provided Codeword from indices in an internal table
 *          to actual Pb and Pr color components, storing their values
 *          in a 2-by-2 block of pixels
 * Parameters: a Codeword where the quantized Pb and Pr components are
 *             held and a 1D array to store the converted block of pixels
 * Returns: void
 *    Note: the 1D block_array stores a block of pixels in the following
 *          order: (top-left, top-right, bottom-left, bottom-right)
 *
 * Expected input: a valid Codeword holding quantized Pb and Pr values
 *                 and a valid 1D block array to store converted values
 * Success output: the converted values will be stored in the block array
 * Failure output: if any of the parameters are invalid, a Checked Runtime
 *                 Error is thrown
 */
void pb_pr_reverse_quantize(Codeword cw, UArray_T block_array);

#endif