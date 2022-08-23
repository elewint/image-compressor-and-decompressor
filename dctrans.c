/**************************************************************
 *
 *                           dctrans.c
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     Implementation of the dctrans class.
 *     
 **************************************************************/
#include "dctrans.h"

int32_t map_bcd(float value);
float unmap_bcd(int32_t value);

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
void dct(UArray_T block_array, Codeword cw)
{
    assert(block_array != NULL);
    assert(cw != NULL);
    assert(UArray_length(block_array) == 4);
    
    float y1 = get_y_value(UArray_at(block_array, 0));
    float y2 = get_y_value(UArray_at(block_array, 1));
    float y3 = get_y_value(UArray_at(block_array, 2));
    float y4 = get_y_value(UArray_at(block_array, 3));
    
    uint32_t a = 63 * (y4 + y3 + y2 + y1) / 4.0;
    int32_t b = map_bcd((y4 + y3 - y2 - y1) / 4.0);
    int32_t c = map_bcd((y4 - y3 + y2 - y1) / 4.0);
    int32_t d = map_bcd((y4 - y3 - y2 + y1) / 4.0);

    set_a_value(cw, a);
    set_b_value(cw, b);
    set_c_value(cw, c);
    set_d_value(cw, d);
}

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
void reverse_dct(Codeword cw, UArray_T block_array)
{
    assert(block_array != NULL);
    assert(cw != NULL);
    assert(UArray_length(block_array) == 4);

    float a = get_a_value(cw) / 63.0;
    float b = unmap_bcd(get_b_value(cw));
    float c = unmap_bcd(get_c_value(cw));
    float d = unmap_bcd(get_d_value(cw));

    float y1 = a - b - c + d;
    float y2 = a - b + c - d;
    float y3 = a + b - c - d;
    float y4 = a + b + c + d;

    set_y_value(UArray_at(block_array, 0), y1);
    set_y_value(UArray_at(block_array, 1), y2);
    set_y_value(UArray_at(block_array, 2), y3);
    set_y_value(UArray_at(block_array, 3), y4);
}

/* map_bcd
 * Purpose: Helper function that maps a float value onto the
 *          integer range from -15 to 15
 * Parameters: A float
 * Returns: An int32_t
 *
 * Expected input: A float value that lies between -0.5 and 0.5 (but
 *                  should likely be between -0.3 and 0.3)
 * Success output: An integer between -15 and 15
 * Failure output: none
 */
int32_t map_bcd(float value)
{
    if (value > 0.3) {
        return 30;
    } else if (value < -0.3) {
        return -30;
    }

    double slope = (30.0 + 30.0) / (0.3 + 0.3);
    return (-30 + slope * (value + 0.3));
}

/* unmap_bcd
 * Purpose: Helper function that takes an integer between -15 and 15
 *          and returns its proper float value when mapped to the
 *          range -0.3 to 0.3
 * Parameters: An int32_t
 * Returns: A float
 *
 * Expected input: An int value that lies between -15 and 15
 * Success output: A float value between -0.3 and 0.3
 * Failure output: none
 */
float unmap_bcd(int32_t value)
{
    double slope = (0.3 + 0.3) / (30.0 + 30.0);
    return (-0.3 + slope * (value + 30.0));
}