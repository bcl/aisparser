/* -----------------------------------------------------------------------
   6-bit packed ASCII Test functions
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com
   All Rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portable.h"
#include "sixbit.h"

/*! \file
    \brief 6-bit packed ASCII Test functions

*/

int test_binfrom6bit( void )
{
    if( binfrom6bit( 0x35 ) != 0x05 )
    {
        fprintf( stderr, "binfrom6bit() 1: Failed\n" );
        return 0;
    }

    if( binfrom6bit( 0x6D ) != 0x35 )
    {
        fprintf( stderr, "binfrom6bit() 2: Failed\n" );
        return 0;
    }

    if( binfrom6bit( 0x05 ) != -1 )
    {
        fprintf( stderr, "binfrom6bit() 3: Failed\n" );
        return 0;
    }

    if( binfrom6bit( 0x5B ) != -1 )
    {
        fprintf( stderr, "binfrom6bit() 4: Failed\n" );
        return 0;
    }

    fprintf( stderr, "binfrom6bit(): Passed\n" );
    return 1;
}


int test_init_6bit( void )
{
    sixbit state;

    if( init_6bit( &state ) == 1 )
    {
        fprintf( stderr, "init_6bit() 1: Failed\n" );
        return 0;
    }
    strcpy( state.bits, "1234567890" );
    if( strcmp( state.p, "1234567890" ) != 0 )
    {
        fprintf( stderr, "init_6bit() 1a: Failed\n" );
        return 0;
    }

    fprintf( stderr, "init_6bit(): Passed\n" );
    return 1;
}


int test_get_6bit( void )
{
    sixbit  state;
    unsigned char i;

    if( init_6bit( &state ) == 1 )
    {
        fprintf( stderr, "get_sixbit() 1: Failed\n" );
        return 0;
    }
    strcpy( state.bits, "0123456789" );
    for( i = 0; i < 10; i++ )
    {
        if( get_6bit( &state, 6 ) != i )
        {
            fprintf( stderr, "get_6bit() 2 - %d: Failed\n", i );
            return 0;
        }
    }

    fprintf( stderr, "get_sixbit(): Passed\n" );
    return 1;
}
