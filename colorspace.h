/**************************************************************
 *
 *                     colorspace.h
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *    This file is the interface of our colorspace class.
 *     It contains the functions that concern the conversions
 *     from rgb to ypbpr and ypbpr to rgb.
 *     
 **************************************************************/
#ifndef COLORSPACE_INCLUDED
#define COLORSPACE_INCLUDED
#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <a2methods.h>
#include <a2plain.h>
#include <pnm.h>
#include <uarray.h>

typedef struct YPbPr *YPbPr;

typedef struct closure_data *closure_data;

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
A2Methods_UArray2 convert_rgb_to_ypbpr(Pnm_ppm ppm, A2Methods_T methods);

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
                                             A2Methods_T methods);

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
                                                         void *elem, void *cl);

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
                                                         void *elem, void *cl);

/* size_of_ypbpr
 * Purpose: returns the size of a ypbpr struct
 * Parameters: none
 * Returns: A size_t denoting the size of a ypbpr struct
 *
 * Expected input: none
 * Success output: The correct size of a ypbpr struct
 * Failure output: none
 */
size_t size_of_ypbpr();

/* get_pb_value
 * Purpose: returns the value of pb in a given ypbpr struct
 * Parameters: a ypbpr struct
 * Returns: The float value pb in the ypbpr struct
 *
 * Expected input: A ypbpr struct
 * Success output: The correct pb value stored 
 * Failure output: none
 */
float get_pb_value(YPbPr ypbpr);

/* get_pr_value
 * Purpose: returns the value of pr in a given ypbpr struct
 * Parameters: a ypbpr struct
 * Returns: The float value pr in the ypbpr struct
 *
 * Expected input: A ypbpr struct
 * Success output: The correct pr value stored 
 * Failure output: none
 */
float get_pr_value(YPbPr ypbpr);

/* get_y_value
 * Purpose: returns the value of y in a given ypbpr struct
 * Parameters: a ypbpr struct
 * Returns: The float value y in the ypbpr struct
 *
 * Expected input: A ypbpr struct
 * Success output: The correct y value stored 
 * Failure output: none
 */
float get_y_value(YPbPr ypbpr);

/* set_pb_value
 * Purpose: sets the value of pb in a given ypbpr struct
 * Parameters: a ypbpr struct and a float
 * Returns: nothing
 *
 * Expected input: A ypbpr struct and a float value
 * Success output: none
 * Failure output: none
 */
void set_pb_value(YPbPr ypbpr, float value);

/* set_pr_value
 * Purpose: sets the value of pr in a given ypbpr struct
 * Parameters: a ypbpr struct and a float
 * Returns: nothing
 *
 * Expected input: A ypbpr struct and a float value
 * Success output: none
 * Failure output: none
 */
void set_pr_value(YPbPr ypbpr, float value);

/* set_y_value
 * Purpose: sets the value of y in a given ypbpr struct
 * Parameters: a ypbpr struct and a float value
 * Returns: nothing
 *
 * Expected input: A ypbpr struct and a value
 * Success output: none
 * Failure output: none
 */
void set_y_value(YPbPr ypbpr, float value);

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
                                                     A2Methods_T methods);

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
                          A2Methods_T methods, UArray_T block_array);

#endif