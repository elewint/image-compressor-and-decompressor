/**************************************************************
 *
 *                     codeword.h
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     This class allows the user to create, access, and use Codeword
 *     structs, data structures that are designed to store the color
 *     and chroma components of a compressed image.
 *     
 **************************************************************/
#ifndef CODEWORD_INCLUDED
#define CODEWORD_INCLUDED
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <a2methods.h>
#include <a2plain.h>
#include <bitpack.h>
#include <assert.h>

/* Codeword is a pointer to a struct that holds the color data of a codeword*/
typedef struct Codeword *Codeword;

/* print_codewords
 * Purpose: Prints the words in a 2D array of int32_t words to stdout
 *          using the apply function apply_print
 * Parameters: a 2D array of int32_t words
 * Returns: void
 *
 * Expected input: a valid word array filled with int32_t words
 * Success output: the words will be printed as characters to stdout
 * Failure output: words are not printed to stdout as expected
 */
void print_codewords(A2Methods_UArray2 cw_array);

/* apply_print
 * Purpose: Apply function to print the current int32_t word to stdout
 * Parameters: a void pointer representing the current word to print and
 *             a closure void pointer that is not used
 * Returns: void
 *
 * Expected input: called on a valid word array
 * Success output: words will be printed as characters to stdout
 * Failure output: words are not printed to stdout as expected
 */
void apply_print(void *curr_cw, void *cl);

/* bitpack_codewords
 * Purpose: Packs the given Codeword array into int32_t words and stores
 *          them in the word_array, all using apply function apply_bitpack
 * Parameters: a 2D array of Codeword structs and a 2D array of int32_t words
 * Returns: void
 *
 * Expected input: called on a valid Codeword array and a valid word array
 * Success output: words will be stored in word_array
 * Failure output: words are not stored in word_array as expected
 */
void bitpack_codewords(A2Methods_UArray2 cw_array,
                    A2Methods_UArray2 word_array);

/* apply_bitpack
 * Purpose: Apply function to pack the current Codeword into an int32_t word 
 *          and stores it in the given element of the word_array
 * Parameters: two ints representing column and row to store the word at,
 *             a Codeword array, a void pointer to store the current Codeword,
 *             and a void pointer closure that holds the word_array
 * Returns: void
 *
 * Expected input: called on a valid Codeword array and passed a valid word
 *                 array as a closure
 * Success output: the current Codeword will be stored in word_array as a word
 * Failure output: the Codeword data is not stored in word_array
 */
void apply_bitpack(int col, int row, A2Methods_UArray2 cw_array, void *curr_cw,
                                                               void *word_arr);

/* unpack_codewords
 * Purpose: Unpacks the given word_array of int32_t words into Codeword
 *          structs and stores them in cw_array using the apply
 *          function apply_bitpack
 * Parameters: a 2D array of int32_t words and a 2D array of Codeword structs
 * Returns: void
 *
 * Expected input: called on a valid word_array and a valid Codeword array
 * Success output: words will be stored in cw_array
 * Failure output: words are not stored in cw_array as expected
 */
void unpack_codewords(A2Methods_UArray2 word_array,
                               A2Methods_UArray2 cw_array);

/* apply_unpack
 * Purpose: Apply function to unpack the current int32_t word into a Codeword
 *          struct and store it in the given element of the cw_array
 * Parameters: two ints representing column and row to store the Codeword at,
 *             a word array, a void pointer to store the current word,
 *             and a void pointer closure that holds the Codeword array
 * Returns: void
 *
 * Expected input: called on a valid word array and passed a valid Codeword
 *                 array as a closure
 * Success output: the current word will be stored in cw_array as a Codeword
 * Failure output: the word is not stored in cw_array
 */
void apply_unpack(int col, int row, A2Methods_UArray2 word_array,
                                  void *curr_word, void *cw_arr);

/* pack_codeword
 * Purpose: Packs the given Codeword struct into the
 *          lowest-order 32 bits of an int64_t word
 * Parameters: a Codeword
 * Returns: the word as an int64_t
 *
 * Expected input: called on a valid Codeword
 * Success output: word storing Codeword data is successfully returned
 * Failure output: word storing Codeword data is not returned
 */
uint64_t pack_codeword(Codeword cw);

/* unpack_codeword
 * Purpose: Unpacks the data stored in the lowest-order 32 bits of
 *          an int64_t word into a Codeword struct
 * Parameters: an int64_t word to unpack and a Codeword where the
 *             word's unpacked data will be stored
 * Returns: the Codeword with data from the word
 *
 * Expected input: passed a word with data in the lowest-order 32 bits
 *                 and a valid Codeword
 * Success output: a Codeword holding the word data is successfully returned
 * Failure output: a Codeword holding the data is not returned
 */
Codeword unpack_codeword(uint64_t word, Codeword cw);

/*****************************************************************
*                        Getter functions
*****************************************************************/

/* size_of_codeword
 * Purpose: Returns the size of a Codeword struct
 * Parameters: none
 * Returns: the size of the struct as a size_t
 *
 * Expected input: none
 * Success output: the struct size
 * Failure output: none
 */
size_t size_of_codeword();

/* get_a_value
 * Purpose: Returns the a component of a Codeword struct
 * Parameters: a Codeword
 * Returns: the a value of the Codeword struct as a uint32_t
 *
 * Expected input: a valid Codeword
 * Success output: the a value of the Codeword struct
 * Failure output: none
 */
uint32_t get_a_value(Codeword cw);

/* get_b_value
 * Purpose: Returns the b component of a Codeword struct
 * Parameters: a Codeword
 * Returns: the b value of the Codeword struct as an int32_t
 *
 * Expected input: a valid Codeword
 * Success output: the b value of the Codeword struct
 * Failure output: none
 */
int32_t get_b_value(Codeword cw);

/* get_c_value
 * Purpose: Returns the c component of a Codeword struct
 * Parameters: a Codeword
 * Returns: the c value of the Codeword struct as an int32_t
 *
 * Expected input: a valid Codeword
 * Success output: the c value of the Codeword struct
 * Failure output: none
 */
int32_t get_c_value(Codeword cw);

/* get_d_value
 * Purpose: Returns the d component of a Codeword struct
 * Parameters: a Codeword
 * Returns: the d value of the Codeword struct as a int32_t
 *
 * Expected input: a valid Codeword
 * Success output: the d value of the Codeword struct
 * Failure output: none
 */
int32_t get_d_value(Codeword cw);

/* get_pb_index
 * Purpose: Returns the index of the Codeword's pb color value
 *          in an internal table
 * Parameters: a Codeword
 * Returns: the pb value of the Codeword struct as a uint32_t
 *
 * Expected input: a valid Codeword
 * Success output: the pb value of the Codeword struct as a uint32_t
 * Failure output: none
 */
uint32_t get_pb_index(Codeword cw);

/* get_pr_index
 * Purpose: Returns the index of the Codeword's pr color value
 *          in an internal table
 * Parameters: a Codeword
 * Returns: the pr value of the Codeword struct as a uint32_t
 *
 * Expected input: a valid Codeword
 * Success output: the pr value of the Codeword struct as a uint32_t
 * Failure output: none
 */
uint32_t get_pr_index(Codeword cw);

/*****************************************************************
*                        Setter functions
*****************************************************************/

/* set_a_value
 * Purpose: Sets the a value of a Codeword struct
 * Parameters: a Codeword and the value to store
 *             as a uint32_t
 * Returns: void
 *
 * Expected input: a valid Codeword
 * Success output: the a value will be set inside
 *                 the Codeword struct
 * Failure output: none
 */
void set_a_value(Codeword cw, uint32_t value);

/* set_b_value
 * Purpose: Sets the b value of a Codeword struct
 * Parameters: a Codeword and the value to store
 *             as an int32_t
 * Returns: void
 *
 * Expected input: a valid Codeword
 * Success output: the b value will be set inside
 *                 the Codeword struct
 * Failure output: none
 */
void set_b_value(Codeword cw, int32_t value);

/* set_c_value
 * Purpose: Sets the c value of a Codeword struct
 * Parameters: a Codeword and the value to store
 *             as an int32_t
 * Returns: void
 *
 * Expected input: a valid Codeword
 * Success output: the c value will be set inside
 *                 the Codeword struct
 * Failure output: none
 */
void set_c_value(Codeword cw, int32_t value);

/* set_d_value
 * Purpose: Sets the d value of a Codeword struct
 * Parameters: a Codeword and the value to store
 *             as an int32_t
 * Returns: void
 *
 * Expected input: a valid Codeword
 * Success output: the d value will be set inside
 *                 the Codeword struct
 * Failure output: none
 */
void set_d_value(Codeword cw, int32_t value);

/* set_pb_index
 * Purpose: Sets the pb index stored in a Codeword struct
 * Parameters: a Codeword and the value to store
 *             as a uint32_t
 * Returns: void
 *
 * Expected input: a valid Codeword
 * Success output: the pb value will be set inside
 *                 the Codeword struct
 * Failure output: none
 */
void set_pb_index(Codeword cw, uint32_t value);

/* set_pr_index
 * Purpose: Sets the pr index stored in a Codeword struct
 * Parameters: a Codeword and the value to store
 *             as a uint32_t
 * Returns: void
 *
 * Expected input: a valid Codeword
 * Success output: the pr value will be set inside
 *                 the Codeword struct
 * Failure output: none
 */
void set_pr_index(Codeword cw, uint32_t value);

/* set_cw_array
 * Purpose: Sets the specified element of an input array of Codewords to
 *          a given Codeword
 * Parameters: a Codeword array, ints for the row and col of the specified
 *             element, a new Codeword to set the given element to, and 
 *             the methods suite methods
 * Returns: void
 *
 * Expected input: a valid Codeword array, an int and row within the
 *                 Codeword array, and the methods suite methods
 * Success output: the Codeword element at (row, col) will be updated
 *                 to the given Codeword
 * Failure output: none
 */
void set_cw_array(A2Methods_UArray2 cw_array, int row, int col, Codeword cw,
                                                       A2Methods_T methods);

#endif