/**************************************************************
 *
 *                     codeword.c
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     Implementation of the codeword class.
 *     
 **************************************************************/
#include "codeword.h"

/* Codeword stores the color data that is encoded in a codeword */
struct Codeword {
    uint32_t a;
    int32_t b;
    int32_t c;
    int32_t d;
    uint32_t pb_index;
    uint32_t pr_index;
};

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
void print_codewords(A2Methods_UArray2 word_array)
{
    assert(word_array != NULL);
    A2Methods_T methods = uarray2_methods_plain; 
    methods->small_map_row_major(word_array, apply_print, NULL);
}

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
void apply_print(void *curr_word, void *cl)
{
    (void)cl;

    uint32_t word = *(int32_t *)curr_word;
    
    for (int i = 24; i >= 0; i -= 8) {
        char c = Bitpack_getu(word, 8, i);
        putchar(c);
    }
}

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
                                     A2Methods_UArray2 word_array)
{
    assert(cw_array != NULL);
    assert(word_array != NULL);
    A2Methods_T methods = uarray2_methods_plain; 
    methods->map_row_major(cw_array, apply_bitpack, word_array);
}

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
void apply_bitpack(int col, int row, A2Methods_UArray2 cw_array,
                                             void *curr_cw, void *word_arr)
{
    (void)cw_array;

    Codeword cw = (Codeword)curr_cw;
    A2Methods_UArray2 word_array = (A2Methods_UArray2)word_arr;
    A2Methods_T methods = uarray2_methods_plain; 

    int32_t word = pack_codeword(cw);
    *(int32_t *)methods->at(word_array, col, row) = word;
}

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
                                 A2Methods_UArray2 cw_array)
{
    assert(cw_array != NULL);
    assert(word_array != NULL);
    A2Methods_T methods = uarray2_methods_plain; 
    methods->map_row_major(word_array, apply_unpack, cw_array);
}

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
                                         void *curr_word, void *cw_arr)
{
    (void)word_array;

    uint32_t word = *(uint32_t *)curr_word;
    A2Methods_UArray2 cw_array = (A2Methods_UArray2)cw_arr;
    A2Methods_T methods = uarray2_methods_plain; 

    Codeword cw = malloc(sizeof(struct Codeword));
    assert(cw);

    unpack_codeword(word, cw);
    *(Codeword)methods->at(cw_array, col, row) = *cw;
    free(cw);
}

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
uint64_t pack_codeword(Codeword cw)
{
    assert(cw != NULL);
    uint64_t word = 0;
    word = Bitpack_newu(word, 4, 0, get_pr_index(cw));
    word = Bitpack_newu(word, 4, 4, get_pb_index(cw));
    word = Bitpack_news(word, 6, 8, get_d_value(cw));
    word = Bitpack_news(word, 6, 14, get_c_value(cw));
    word = Bitpack_news(word, 6, 20, get_b_value(cw));
    word = Bitpack_newu(word, 6, 26, get_a_value(cw));

    return word;
}

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
Codeword unpack_codeword(uint64_t word, Codeword cw)
{
    assert(cw != NULL);
    set_a_value(cw, Bitpack_getu(word, 6, 26));
    set_b_value(cw, Bitpack_gets(word, 6, 20));
    set_c_value(cw, Bitpack_gets(word, 6, 14));
    set_d_value(cw, Bitpack_gets(word, 6, 8));
    set_pb_index(cw, Bitpack_getu(word, 4, 4));
    set_pr_index(cw, Bitpack_getu(word, 4, 0));

    return cw;
}

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
size_t size_of_codeword()
{
    return sizeof(struct Codeword);
}

/* get_a_value
 * Purpose: Returns the a component of a Codeword struct
 * Parameters: a Codeword
 * Returns: the a value of the Codeword struct as a uint32_t
 *
 * Expected input: a valid Codeword
 * Success output: the a value of the Codeword struct
 * Failure output: none
 */
uint32_t get_a_value(Codeword cw)
{
    assert(cw != NULL);
    return cw->a;
}

/* get_b_value
 * Purpose: Returns the b component of a Codeword struct
 * Parameters: a Codeword
 * Returns: the b value of the Codeword struct as an int32_t
 *
 * Expected input: a valid Codeword
 * Success output: the b value of the Codeword struct
 * Failure output: none
 */
int32_t get_b_value(Codeword cw)
{
    assert(cw != NULL);
    return cw->b;
}

/* get_c_value
 * Purpose: Returns the c component of a Codeword struct
 * Parameters: a Codeword
 * Returns: the c value of the Codeword struct as an int32_t
 *
 * Expected input: a valid Codeword
 * Success output: the c value of the Codeword struct
 * Failure output: none
 */
int32_t get_c_value(Codeword cw)
{
    assert(cw != NULL);
    return cw->c;
}

/* get_d_value
 * Purpose: Returns the d component of a Codeword struct
 * Parameters: a Codeword
 * Returns: the d value of the Codeword struct as a int32_t
 *
 * Expected input: a valid Codeword
 * Success output: the d value of the Codeword struct
 * Failure output: none
 */
int32_t get_d_value(Codeword cw)
{
    assert(cw != NULL);    
    return cw->d;
}

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
uint32_t get_pb_index(Codeword cw)
{
    assert(cw != NULL);
    return cw->pb_index;
}

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
uint32_t get_pr_index(Codeword cw)
{
    assert(cw != NULL);
    return cw->pr_index;
}

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
void set_a_value(Codeword cw, uint32_t value)
{
    assert(cw != NULL);
    cw->a = value;
}

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
void set_b_value(Codeword cw, int32_t value)
{
    assert(cw != NULL);
    cw->b = value;
}

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
void set_c_value(Codeword cw, int32_t value)
{
    assert(cw != NULL);
    cw->c = value;
}

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
void set_d_value(Codeword cw, int32_t value)
{
    assert(cw != NULL);
    cw->d = value;
}

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
void set_pb_index(Codeword cw, uint32_t value)
{
    assert(cw != NULL);
    cw->pb_index = value;
}

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
void set_pr_index(Codeword cw, uint32_t value)
{
    assert(cw != NULL);
    cw->pr_index = value;
}

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
                                                        A2Methods_T methods)
{
    assert(cw != NULL);
    assert(methods != NULL);
    assert(cw_array != NULL);
    *(Codeword)methods->at(cw_array, row, col) = *cw;
}