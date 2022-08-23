/**************************************************************
 *
 *                     quantize.c
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     Implementation of the quantize class.
 *     
 **************************************************************/
#include "quantize.h"

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
void pb_pr_quantize(UArray_T block_array, Codeword cw)
{
    assert(block_array != NULL);
    assert(cw != NULL);
    assert(UArray_length(block_array) == 4);
    float pb_add = 0;
    float pr_add = 0;

    /* Calculate Pb and Pr averages */
    for (int i = 0; i < UArray_length(block_array); i++) {
        pb_add += get_pb_value(UArray_at(block_array, i));
        pr_add += get_pr_value(UArray_at(block_array, i));
    }
    
    float pb_avg = pb_add / 4.0; 
    float pr_avg = pr_add / 4.0;

    /* Force the Pb and Pr averages into the range [-0.5, 0.5] */
    if (pb_avg > 0.5) {
        pb_avg = 0.5;
    } else if (pb_avg < -0.5) {
        pb_avg = -0.5;
    }

    if (pr_avg > 0.5) {
        pr_avg = 0.5;
    } else if (pr_avg < -0.5) {
        pr_avg = -0.5;
    }

    uint32_t pb_index = Arith40_index_of_chroma(pb_avg);
    uint32_t pr_index = Arith40_index_of_chroma(pr_avg);

    set_pb_index(cw, pb_index);
    set_pr_index(cw, pr_index);
}

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
void pb_pr_reverse_quantize(Codeword cw, UArray_T block_array)
{
    assert(block_array != NULL);
    assert(cw != NULL);
    assert(UArray_length(block_array) == 4);

    float pb = Arith40_chroma_of_index(get_pb_index(cw));
    float pr = Arith40_chroma_of_index(get_pr_index(cw));

    for (int i = 0; i < UArray_length(block_array); i++) {
        set_pb_value(UArray_at(block_array, i), pb);
        set_pr_value(UArray_at(block_array, i), pr);
    }
}