/**************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: Arith
 *     Authors:  Eli Intriligator (eintri01), Max Behrendt (mbehre01)
 *     Date:     Oct 28, 2021
 *
 *     Summary
 *     Implementation of the bitpack class. Contains functions that
 *     check to see if values can be packed into certain amounts
 *     of bits, pack values into words, and retrieve packed values
 *     from words. 
 *     
 **************************************************************/
#include <bitpack.h>
#include <stdio.h>
#include "assert.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

uint64_t shift_left(uint64_t input, unsigned shift);
uint64_t shift_right_u(uint64_t input, unsigned shift);
int64_t shift_right_s(int64_t input, unsigned shift);
uint64_t zero_out_field(uint64_t word, unsigned width, unsigned lsb);

/* Bitpack_fitsu
 * Purpose: Checks to see if a given unsigned 64 bit integer value
 *              can fit in the supplied bit width
 * Parameters: A 64 bit unsigned value and an unsigned width
 * Returns: A boolean
 *
 * Expected input: A valid 64 bit unsigned int and a bit width
 * Success output: True if the given value can fit in the supplied width
 *                  of bits, false if it can't
 * Failure output: Raises an exception if the given width is
 *                  greater than 64
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64);

    if (width == 64) {
        return true;
    } else if (width == 0) {
        return false;
    } else if (n < shift_left((uint64_t)1, width)) {
        return true;
    } else {
        return false;
    }
}

/* Bitpack_fitss
 * Purpose: Checks to see if a given signed 64 bit integer value
 *              can fit in the supplied bit width
 * Parameters: A 64 bit signed value and an unsigned width
 * Returns: A boolean
 *
 * Expected input: A valid 64 bit signed int and a bit width
 * Success output: True if the given value can fit in the supplied width
 *                  of bits, false if it can't
 * Failure output: Raises an exception if the given width is
 *                  greater than 64
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= 64);    

    if (width == 64) {
        return true;
    } else if (width == 0) {
        return false;
    } else if (n < (int64_t)shift_left(1, width - 1)
            && n >= (int64_t)-1 * (int64_t)(shift_left(1, width - 1))) {
        return true;
    } else {
        return false;
    }
}

/* Bitpack_gets
 * Purpose: Gets the value from the supplied field in a given word
 * Parameters: A 64 bit word, a width, and an lsb
 * Returns: A 64 bit unsigned value
 *
 * Expected input: A valid 64 bit word, the width of a field in that
 *                  word, and the lsb of that field
 * Success output: An unsignged 64 bit value retrieved from the given field
 * Failure output: Raises an exception of the given width plus the
 *                  given lsb is greater than 64, or the given width is
 *                  greater than 64
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert(width + lsb <= 64);

    if (width == 0) {
        return 0;
    }

    word = shift_right_u(word, lsb);

    uint64_t mask = 0;
    mask = ~mask;
    mask = shift_right_u(mask, 64 - width);

    word &= mask;

    return word;
}

/* Bitpack_gets
 * Purpose: Gets the value from the supplied field in a given word
 * Parameters: A 64 bit word, a width, and an lsb
 * Returns: A 64 bit signed value
 *
 * Expected input: A valid 64 bit word, the width of a field in that
 *                  word, and the lsb of that field
 * Success output: A signged 64 bit value retrieved from the given field
 * Failure output: Raises an exception of the given width plus the
 *                  given lsb is greater than 64, or the given width is
 *                  greater than 64
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert(width + lsb <= 64);

    if (width == 0) {
        return 0;
    }

    word = shift_right_s(word, lsb);

    uint64_t mask = 0;
    mask = ~mask;
    mask = shift_right_u(mask, 64 - width);

    uint64_t sign_bit_mask = 1;
    sign_bit_mask = shift_left(sign_bit_mask, width - 1);
    uint64_t sign_bit = word & sign_bit_mask;

    if (sign_bit == 0) {
        word &= mask;
        return (int64_t)word;
    } else {
        int64_t mask_of_1s = -1;
        mask_of_1s = shift_left(mask_of_1s, width);
        word |= mask_of_1s;
        return (int64_t)word;
    } 
}

/* Bitpack_newu
 * Purpose: Sets a field in a given word to be the supplied value
 * Parameters: A 64 bit word, a width, and lsb, and a 64 bit unsigned value
 * Returns: A 64 bit word
 *
 * Expected input: A valid 64 bit word, an unsigned 64 bit value, the width
 *                  of that value, and the lsb in the word where that
 *                  value will be inserted
 * Success output: A 64 bit word with the given field filled in with the
 *                  proper value
 * Failure output: Raises an exception of the given width plus the
 *                  given lsb is greater than 64, or the given width is
 *                  greater than 64
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                                                     uint64_t value)
{
    assert(width <= 64);
    assert(width + lsb <= 64);

    if (!Bitpack_fitsu(value, width)) {
        RAISE(Bitpack_Overflow);
    }

    word = zero_out_field(word, width, lsb);
    value = shift_left(value, lsb);
    word |= value;

    return word;
}

/* Bitpack_news
 * Purpose: Sets a field in a given word to be the supplied value
 * Parameters: A 64 bit word, a width, and lsb, and a 64 bit signed value
 * Returns: A 64 bit word
 *
 * Expected input: A valid 64 bit word, a signed 64 bit value, the width
 *                  of that value, and the lsb in the word where that
 *                  value will be inserted
 * Success output: A 64 bit word with the given field filled in with the
 *                  proper value
 * Failure output: Raises an exception of the given width plus the
 *                  given lsb is greater than 64, or the given width is
 *                  greater than 64
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                                                         int64_t value)
{
    assert(width <= 64);
    assert(width + lsb <= 64);

    if (!Bitpack_fitss(value, width)) {
        RAISE(Bitpack_Overflow);
    }

    word = zero_out_field(word, width, lsb);
    value = shift_left(value, lsb);
    word |= value;

    return word;
}

/* shift_left
 * Purpose: Helper function that shifts a 64 bit word to the left
 * Parameters: A 64 bit word and a shift amount
 * Returns: A 64 bit word
 *
 * Expected input: A valid 64 bit word, and an integer denoting how
 *                  far that word will be shifted
 * Success output: A 64 bit word that is properly shifted
 * Failure output: none
 */
uint64_t shift_left(uint64_t input, unsigned shift)
{
    if (shift == 64) {
        return 0;
    } else {
        input <<= shift;
    }
    
    return input;
}

/* shift_right_u
 * Purpose: Helper function that shifts an unsigned integer to the right
 * Parameters: A 64 bit word and a shift amount
 * Returns: A 64 bit word
 *
 * Expected input: A valid 64 bit word, and an integer denoting how
 *                  far that word will be shifted
 * Success output: A 64 bit word that is properly shifted
 * Failure output: none
 */
uint64_t shift_right_u(uint64_t input, unsigned shift)
{
    if (shift == 64) {
        return 0;
    } else {
        input >>= shift;
    }
    
    return input;
}

/* shift_right_s
 * Purpose: Helper function that shifts a signed integer to the right
 * Parameters: A 64 bit word and a shift amount
 * Returns: A 64 bit word
 *
 * Expected input: A valid 64 bit word, and an integer denoting how
 *                  far that word will be shifted
 * Success output: A 64 bit word that is properly shifted
 * Failure output: none
 */
int64_t shift_right_s(int64_t input, unsigned shift)
{
    if (shift == 64) {
        input >>= 63;
        input >>= 1;
    } else {
        input >>= shift;
    }
    
    return input;
}

/* zero_out_field
 * Purpose: Helper function that zeros out the binary digets in the given
 *          field of the supplied word
 * Parameters: A 64 bit word, the width of a field in that word, and the
 *              lsb of that field
 * Returns: A 64 bit word
 *
 * Expected input: A valid 64 bit word,
 *                  the width of a field in that word, and the lsb of 
 *                  that field
 * Success output: A 64 bit word with the proper field zeroed out
 * Failure output: none
 */
uint64_t zero_out_field(uint64_t word, unsigned width, unsigned lsb)
{
    uint64_t mask = ~0;
    mask = shift_right_u(mask, 64 - width);
    mask = shift_left(mask, lsb);
    mask = ~mask;
    word &= mask;
    return word;
}
