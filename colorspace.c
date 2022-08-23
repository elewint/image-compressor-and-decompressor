/**************************************************************
 *
 *                     40image.c
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     This file is the implementation of our colorspace class.
 *     It contains the implementations of the listed functions
 *     in colorspace.h that concern the conversions from rgb to
 *     ypbpr and ypbpr to rgb.
 *     
 *     
 **************************************************************/
#include "colorspace.h"

struct YPbPr {
    float y, pb, pr;
};

struct closure_data {
    A2Methods_UArray2 array;
    A2Methods_T methods;
    unsigned denominator;
};

unsigned force_values_into_range(float value, unsigned denominator);

/* convert_rgb_to_ypbpr
 * Purpose: Converts an array of rgb structs into an array of ypbpr
 *          structs
 * Parameters: A ppm and methods
 * Returns: A UArray2 of ypbpr structs
 *
 * Expected input: A valid ppm and methods
 * Success output: A UArray2 of ypbpr structs
 * Failure output: none
 */
A2Methods_UArray2 convert_rgb_to_ypbpr(Pnm_ppm ppm, A2Methods_T methods)
{ 
    assert(methods != NULL);
    assert(ppm != NULL);
    A2Methods_UArray2 rgb_array = ppm->pixels;
    int width = ppm->width;
    int height = ppm->height;
 
    A2Methods_UArray2 ypbpr_array = methods->new(width, height,
                                             sizeof(struct YPbPr));

    closure_data ypbpr_data = malloc(sizeof(struct closure_data));
    assert(ypbpr_data);

    ypbpr_data->array = ypbpr_array;
    ypbpr_data->methods = methods;
    ypbpr_data->denominator = ppm->denominator;
                                             
    methods->map_row_major(rgb_array, apply_rgb_to_ypbpr, ypbpr_data);

    ypbpr_array = ypbpr_data->array;
    free(ypbpr_data);

    return ypbpr_array;
}

/* convert_ypbpr_to_rgb
 * Purpose: Converts an array of ypbpr structs into an array of rgb
 *          structs
 * Parameters: A UArray2 of ypbpr structs and methods
 * Returns: A UArray2 of rgb structs
 *
 * Expected input: A UArray2 of ypbpr structs and methods
 * Success output: A UArray2 of rgb structs
 * Failure output: none
 */
A2Methods_UArray2 convert_ypbpr_to_rgb(A2Methods_UArray2 array,
                                             A2Methods_T methods)
{
    assert(array != NULL);
    assert(methods != NULL);

    int width = methods->width(array);
    int height = methods->height(array);

    A2Methods_UArray2 rgb_array = methods->new(width, height,
                                             sizeof(struct Pnm_rgb));

    closure_data rgb_data = malloc(sizeof(struct closure_data));
    assert(rgb_data);

    rgb_data->array = rgb_array;
    rgb_data->methods = methods;
                                             
    methods->map_row_major(array, apply_ypbpr_to_rgb, rgb_data);

    rgb_array = rgb_data->array;
    free(rgb_data);

    return rgb_array;
}

/* apply_rgb_to_ypbpr
 * Purpose: Apply function to the mapping function that iterates over
 *          the array of rgb structs. Converts rgb values to ypbpr
 *          values and sets the correct ypbpr struct values in the
 *           array of ypbpr structs
 * Parameters: Two integers denoting a location, an array of rgb
 *              structs, the element at (i, j), and a closure, which
 *              holds a struct that contains methods, the correct
 *              denominator of the ppm, and the array of ypbpr structs
 * Returns: Nothing
 *
 * Expected input: A valid location in the rgb array, an rgb array,
 *                  the element at that location, and a closure struct
 *                   that contains a ypbpr array, a denominator, and
 *                   methods.
 * Success output: none
 * Failure output: none
 */
void apply_rgb_to_ypbpr(int i, int j, A2Methods_UArray2 rgb_array,
                                                         void *elem, void *cl)
{
    (void) rgb_array;

    struct closure_data data = *(closure_data)cl;
    A2Methods_UArray2 ypbpr_array = data.array;
    A2Methods_T methods = data.methods;
    unsigned denominator = data.denominator;

    Pnm_rgb curr_rgb = (Pnm_rgb)elem;
    float r = curr_rgb->red / (float)denominator;
    float g = curr_rgb->green / (float)denominator;
    float b = curr_rgb->blue / (float)denominator;

    float y = 0.299 * r + 0.587 * g + 0.114 * b;
    float pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
    float pr = 0.5 * r - 0.418688 * g - 0.081312 * b;

    YPbPr ypbpr = malloc(sizeof(struct YPbPr));
    assert(ypbpr);

    ypbpr->y = y;
    ypbpr->pb = pb;
    ypbpr->pr = pr;

    *(YPbPr)methods->at(ypbpr_array, i, j) = *ypbpr;
    free(ypbpr);
}

/* apply_ypbpr_to_rgb
 * Purpose: Apply function to the mapping function that iterates over
 *          the array of ypbpr structs. Converts ypbpr values to rgb
 *          values and sets the correct rgb struct values in the
 *           array of rgb structs
 * Parameters: Two integers denoting a location, an array of ypbpr
 *              structs, the element at (i, j), and a closure, which
 *              holds a struct that contains methods and the array 
 *              of rgb structs
 * Returns: Nothing
 *
 * Expected input: A valid location in the ypbpr array, a ypbpr array,
 *                  the element at that location, and a closure struct
 *                   that contains an rgb array, and methods.
 * Success output: none
 * Failure output: none
 */
void apply_ypbpr_to_rgb(int i, int j, A2Methods_UArray2 ypbpr_array,
                                                         void *elem, void *cl)
{
    (void) ypbpr_array;

    Pnm_rgb curr_rgb = malloc(sizeof(struct Pnm_rgb));
    assert(curr_rgb);
    
    struct closure_data rgb_data = *(closure_data)cl;
    A2Methods_UArray2 rgb_array = rgb_data.array;
    A2Methods_T methods = rgb_data.methods;

    unsigned denominator = 200;

    YPbPr curr_ypbpr = (YPbPr)elem;
    float y = curr_ypbpr->y;
    float pr = curr_ypbpr->pr;
    float pb = curr_ypbpr->pb;

    curr_rgb->red = 
        force_values_into_range((1.0 * y + 0.0 * pb + 1.402 * pr)
                                        * denominator, denominator);
    curr_rgb->green = 
        force_values_into_range((1.0 * y - 0.344136 * pb - 0.714136 * pr)
                                             * denominator, denominator);
    curr_rgb->blue = 
        force_values_into_range((1.0 * y + 1.772 * pb + 0.0 * pr)
                                     * denominator, denominator);


    *(Pnm_rgb)methods->at(rgb_array, i, j) = *curr_rgb;
    free(curr_rgb);
}

/* size_of_ypbpr
 * Purpose: returns the size of a ypbpr struct
 * Parameters: none
 * Returns: A size_t denoting the size of a ypbpr struct
 *
 * Expected input: none
 * Success output: The correct size of a ypbpr struct
 * Failure output: none
 */
size_t size_of_ypbpr()
{
    return sizeof(struct YPbPr);
}

/* get_pb_value
 * Purpose: returns the value of pb in a given ypbpr struct
 * Parameters: a ypbpr struct
 * Returns: The float value pb in the ypbpr struct
 *
 * Expected input: A ypbpr struct
 * Success output: The correct pb value stored 
 * Failure output: none
 */
float get_pb_value(YPbPr ypbpr)
{
    assert(ypbpr != NULL);
    return ypbpr->pb;
}

/* get_pr_value
 * Purpose: returns the value of pr in a given ypbpr struct
 * Parameters: a ypbpr struct
 * Returns: The float value pr in the ypbpr struct
 *
 * Expected input: A ypbpr struct
 * Success output: The correct pr value stored 
 * Failure output: none
 */
float get_pr_value(YPbPr ypbpr)
{
    assert(ypbpr != NULL);
    return ypbpr->pr;
}

/* get_y_value
 * Purpose: returns the value of y in a given ypbpr struct
 * Parameters: a ypbpr struct
 * Returns: The float value y in the ypbpr struct
 *
 * Expected input: A ypbpr struct
 * Success output: The correct y value stored 
 * Failure output: none
 */
float get_y_value(YPbPr ypbpr)
{
    assert(ypbpr != NULL);
    return ypbpr->y;
}

/* set_pb_value
 * Purpose: sets the value of pb in a given ypbpr struct
 * Parameters: a ypbpr struct and a float
 * Returns: nothing
 *
 * Expected input: A ypbpr struct and a float value
 * Success output: none
 * Failure output: none
 */
void set_pb_value(YPbPr ypbpr, float value)
{
    assert(ypbpr != NULL);
    ypbpr->pb = value;
}

/* set_pr_value
 * Purpose: sets the value of pr in a given ypbpr struct
 * Parameters: a ypbpr struct and a float
 * Returns: nothing
 *
 * Expected input: A ypbpr struct and a float value
 * Success output: none
 * Failure output: none
 */
void set_pr_value(YPbPr ypbpr, float value)
{
    assert(ypbpr != NULL);
    ypbpr->pr = value;
}

/* set_y_value
 * Purpose: sets the value of y in a given ypbpr struct
 * Parameters: a ypbpr struct and a float value
 * Returns: nothing
 *
 * Expected input: A ypbpr struct and a value
 * Success output: none
 * Failure output: none
 */
void set_y_value(YPbPr ypbpr, float value)
{
    assert(ypbpr != NULL);
    ypbpr->y = value;
}

/* make_block_array
 * Purpose: makes a UArray_T of size 4 of a block in the ypbpr array
 * Parameters: an UArray2 of ypbpr structs, 2 ints denotinga a location
 *              in that array, and methods
 * Returns: A UArray of size 4
 *
 * Expected input: A valid ypbpr array, a location in that array,
 *                  methods
 * Success output: A UArray_T of size 4 of a block in the given
 *                  ypbpr array
 * Failure output: none
 */
UArray_T make_block_array(A2Methods_UArray2 ypbpr_array, int col, int row,
                                                         A2Methods_T methods)
{
    assert(ypbpr_array != NULL);
    assert(methods != NULL);

    UArray_T block_array = UArray_new(4, sizeof(struct YPbPr));

    *(YPbPr)UArray_at(block_array, 0) = *(YPbPr)methods->at(ypbpr_array, col,
                                                                         row);
    *(YPbPr)UArray_at(block_array, 1) = *(YPbPr)methods->at(ypbpr_array,
                                                                 col + 1, row);
    *(YPbPr)UArray_at(block_array, 2) = *(YPbPr)methods->at(ypbpr_array, col,
                                                                     row + 1);
    *(YPbPr)UArray_at(block_array, 3) = *(YPbPr)methods->at(ypbpr_array,
                                                             col + 1, row + 1);
    return block_array;
}

/* set_ypbpr_block
 * Purpose: sets the values in a UArray_T of size 4 with the proper
 *          values that they will store from the ypbpr array
 * Parameters: an UArray2 of ypbpr structs, 2 ints denotinga a location
 *              in that array, methods, and a UArray
 * Returns: nothing
 *
 * Expected input: A valid ypbpr array, a location in that array,
 *                  methods, and a valid block_array of size 4
 * Success output: none
 * Failure output: none
 */
void set_ypbpr_block(A2Methods_UArray2 ypbpr_array, int col, int row,
                          A2Methods_T methods, UArray_T block_array)
{
    assert(ypbpr_array != NULL);
    assert(methods != NULL);
    assert(block_array != NULL);

    *(YPbPr)methods->at(ypbpr_array, col, row) =
                                            *(YPbPr)UArray_at(block_array, 0);
    *(YPbPr)methods->at(ypbpr_array, col + 1, row) =
                                            *(YPbPr)UArray_at(block_array, 1);
    *(YPbPr)methods->at(ypbpr_array, col, row + 1) =
                                            *(YPbPr)UArray_at(block_array, 2);
    *(YPbPr)methods->at(ypbpr_array, col + 1, row + 1) =
                                            *(YPbPr)UArray_at(block_array, 3);
}

/* force_values_into_range
 * Purpose: helper function that forces a float value into a given
 *          range between 0 and a denominator
 * Parameters: a float value and a denominator
 * Returns: an unsigned value
 *
 * Expected input: A float value and a denominator
 * Success output: An unsigned value that lies between 0 and
 *                  denominator
 * Failure output: none
 */
unsigned force_values_into_range(float value, unsigned denominator)
{
    if (value > denominator) {
        value = denominator;
    } else if (value < 0) {
        value = 0;
    }
    
    return value;
}