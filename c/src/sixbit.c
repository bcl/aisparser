/* -----------------------------------------------------------------------
   6-bit packed ASCII functions
   Copyright 2006-2020 by Brian C. Lane <bcl@brianlane.com
   All Rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portable.h"
#include "sixbit.h"

/*! \file
    \brief 6-bit packed ASCII functions
    \author Copyright 2006-2020 by Brian C. Lane <bcl@brianlane.com>, All Rights Reserved
    \version 1.8

    This module's functions are used to extract data from the 6-bit packed
    ASCII string used by AIVDM/AIVDO AIS messages.

    The state of the sixbit machine is held in the sixbit structure so that
    multiple streams can be processed by maintaining separate instances
    of sixbit. init_6bit() should be called with a pointer to the sixbit
    instance, it will setup the structure for parsing. The 6-bit data
    should then be copied into the sixbit.bits buffer. A maximum of 255
    characters are allowed (this is changed by #SIXBIT_LEN in sixbit.h)

    Up to 32 bits of data are fetched from the string by calling get_6bit()

    The size of the packet can be calculated with strlen(state->bits) * 6 but
    note that due to padding at the end of the string the data may be
    1-6 bits longer than the the expected length for the message id.

    It is up to the calling function to keep track of how many bits have
    been fetched. When it reaches the end of the 6-bit string get_6bit()
    will return 0's.

*/



const unsigned char pow2_mask[] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F };


/* ----------------------------------------------------------------------- */
/** Calculate the number of bits remaining in the six_state

    \param state

    returns:
      - Number of bits remaining
*/
/* ----------------------------------------------------------------------- */
unsigned int __stdcall sixbit_length( sixbit *state )
{
    return state->remainder_bits + (strlen(state->bits) * 6);
}


/* ----------------------------------------------------------------------- */
/** Convert an ascii value to a 6-bit binary value

    This function checks the ASCII value to make sure it can be converted.
    If not, it returns a -1.
    Otherwise it returns the 6-bit binary value.

    \param ascii character to convert

    returns:
      - -1 if it fails
      - 6-bit value (0x00-0x3F)

    This is used to convert the packed 6-bit value to a binary value. It
    is not used to convert data from fields such as the name and
    destination -- Use ais2ascii() instead.
*/
/* ----------------------------------------------------------------------- */
char __stdcall binfrom6bit( char ascii )
{
    if( (ascii < 0x30) || (ascii > 0x77) || ((ascii > 0x57) && (ascii < 0x60)) )
        return -1;
    if( ascii < 0x60 )
        return (ascii - 0x30) & 0x3F;
    else
        return (ascii - 0x38) & 0x3F;
}


/* ----------------------------------------------------------------------- */
/** Convert a binary value to a 6-bit ASCII value

    This function checks the binary value to make sure it can be converted.
    If not, it returns a -1.
    Otherwise it returns the 6-bit ASCII value.

    \param value to convert

    returns:
      - -1 if it fails
      - 6-bit ASCII

    This is used to convert a 6 bit binary value to the packed 6-bit value
    It is not used to convert data from fields such as the name and
    destination -- Use ais2ascii() instead.
*/
/* ----------------------------------------------------------------------- */
char __stdcall binto6bit( char value )
{
	if (value > 0x3F )
		return -1;
	if (value < 0x28)
		return value + 0x30;
	else
		return value + 0x38;
}


/* ----------------------------------------------------------------------- */
/** Initialize a 6-bit datastream structure

    \param state state of 6-bit parser

    This function initializes the state of the sixbit parser variables
    and 0 terminates the 6-bit string.

    Example:
    \code
    sixbit state;

    init_6bit( &state );
    \endcode
*/
/* ----------------------------------------------------------------------- */
int __stdcall init_6bit( sixbit *state )
{
    state->remainder = 0;
    state->remainder_bits = 0;
    state->p = state->bits;
    *state->p = 0;

    return 0;
}


/* ----------------------------------------------------------------------- */
/** Return 0-32 bits from a 6-bit ASCII stream

    \param state pointer to a sixbit state structure
    \param numbits number of bits to return

    This function returns the requested number of bits to the calling
    function. It pulls the bits from the raw 6-bit ASCII as they are
    needed.

    The full string can be addressed by pointing to state->bits and the
    length can be calculated by strlen(state->bits) * 6, but note that the
    string also includes any final padding, so when checking lengths take
    into account that it will be a multiples of 6 bits.

    Example:
    \code
    sixbit  state;
    unsigned char i;

    init_6bit( &state );
    strcpy( state.bits, "5678901234" );
    i = get_6bit( &state, 6 );

    i == 5
    \endcode
*/
/* ----------------------------------------------------------------------- */
unsigned long __stdcall get_6bit( sixbit *state, short numbits )
{
    unsigned long result;
    short         fetch_bits;

    result = 0;
    fetch_bits = numbits;

    while( fetch_bits > 0 )
    {
        /*  Is there anything left over from the last call? */
        if( state->remainder_bits > 0 )
        {
            if( state->remainder_bits <= fetch_bits )
            {
                /* reminder is less than or equal to what is needed */
                result = (result << 6) + state->remainder;
                fetch_bits -= state->remainder_bits;
                state->remainder = 0;
                state->remainder_bits = 0;
            } else {
                /* remainder is larger than what is needed
                   Take the bits from the top of remainder
                */
                result = result << fetch_bits;
                result += state->remainder >> (state->remainder_bits - fetch_bits);

                /* Fixup remainder */
                state->remainder_bits -= fetch_bits;
                state->remainder &= pow2_mask[(int) state->remainder_bits];

                return result;
            }
        }

        /* Get the next block of 6 bits from the ASCII string */
        if( *state->p != 0 )
        {
            state->remainder = binfrom6bit( *state->p );
            state->remainder_bits = 6;
            state->p++;
        } else {
            /* Nothing more to fetch, return what we have */
            return result;
        }
    }
    return result;
}
