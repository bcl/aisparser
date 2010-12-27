/* -----------------------------------------------------------------------
   NMEA 0183 Sentence Parsing
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "portable.h"
#include "nmea.h"

/*! \file
    \brief NMEA 0183 Sentence Parser Module
    \author Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>, All Rights Reserved
    \version 1.8

    This module provides utility functions for handling NMEA 0183 data
    streams. The stream data is 8 bit ASCII with comma separated data
    fields. The functions in this module can be used to generate and
    verify the checksum, find the start of the sentence, find the next
    field, convert a field to an integer and copy a field to a destination
    buffer.
    
*/


/* ----------------------------------------------------------------------- */
/** Convert a single ASCII Hex digit (0-9 A-F or a-f) to a binary value

    \param c ASCII character to convert

    Returns:
      - binary 0x00-0x0F
      - -1 if it was not an ASCII hex character
      
    Example:
    \code
    ahextobin( 'D' ) == 0x0D
    \endcode
*/
/* ----------------------------------------------------------------------- */
char __stdcall ahextobin( char *c )
{
    if( (*c >= '0') && (*c <= '9') )
    {
        return *c - '0';
    } else if( (*c >= 'A') && (*c <= 'F') ) {
        return *c - ('A'-10);
    } else if( (*c >= 'a') && (*c <= 'f') ) {
        return *c - ('a'-10);
    }
    
    return -1;
}


/* ----------------------------------------------------------------------- */
/** Return a pointer to the next field in the string

    \param p Pointer to the NMEA 0183 field

    Returns:
      - pointer to next field in string
      - NULL if the end of the string has been reached
      
    Example:
    \code
    char buf[255];
    char *p;
    
    strcpy( buf, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    p = nmea_next_field( p );

    *p == '1'
    \endcode
*/
/* ----------------------------------------------------------------------- */
char * __stdcall nmea_next_field( char *p )
{
    while (   p 
           && (*p != 0) 
           && (*p != ',') 
           && (*p != '*') ) { p++; }
    if (p != 0)
        return p+1;
        
    return NULL;
}


/* ----------------------------------------------------------------------- */
/** Convert the current field to an unsigned integer

    \param p Pointer to the NMEA 0183 field

    Returns:
      - unsigned int

    Example:
    \code
    char buf[255];
    char *p;
    unsigned int i;
    
    strcpy( buf, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    p = nmea_next_field( p );   
    i = nmea_uint( p );
    
    i == 1
    \endcode
*/
/* ----------------------------------------------------------------------- */
unsigned int __stdcall nmea_uint( char *p )
{
    unsigned int i = 0;

    while (   p 
           && (*p != 0) 
           && (*p != ',') 
           && (*p != '*') )
    {
        i *= 10;
        i += *p - '0';
        p++;
    }

    return i;    
}



/* ----------------------------------------------------------------------- */
/** Copy len bytes from a field to a destination buffer

    \param dest pointer to destination buffer
    \param src pointer to 0 terminated source buffer
    \param len maximum number of characters to copy

    Returns:
      - unsigned int

    This routine stops copying when it hits the end of the field, end of
    the string or len.
      
    Example:
    \code
    char buf1[255];
    char buf2[255];
    char *p;
    
    strcpy( buf1, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    buf2[0] = 0;
    
    p = buf1;
    nmea_copy_field( buf2, p, 254 );

    buf2 == buf1
    \endcode
*/
/* ----------------------------------------------------------------------- */
char * __stdcall nmea_copy_field( char *dest, char *src, int len )
{
    while (   len
           && src
           && dest 
           && (*src != 0) 
           && (*src != ',') 
           && (*src != '*') ) { *dest++ = *src++; len--; }

    *dest = 0;
    return dest;
}


/* ----------------------------------------------------------------------- */
/** Find the start of a NMEA sentence '!' or '$' character

    \param buffer pointer to a 0 terminated string buffer

    Returns:
        - pointer to start character or NULL if not found
        
    The NMEA sentence may not always start at the beginning of the buffer,
    use this routine to make sure the start of the sentence has been found.
    
    Example:
    \code
    char buf[255];
    char *p;

    strcpy( (char *) buf, "678,4343,123,585*FF\n$GPGGA,32,121,676,29394,29234*AA\n" );
    p = find_nmea_start( buf ) );

    *p == "$GPGGA,..."
    \endcode
*/
/* ----------------------------------------------------------------------- */
char * __stdcall find_nmea_start( char *buffer )
{
    char  *p;
    
    if( !buffer )
        return NULL;

    p = buffer;
    while( p && (*p != 0) && (*p != '!') && (*p != '$') )
        p++;
        
    if( *p != 0 )
        return p;
        
    return NULL;
}


/* ----------------------------------------------------------------------- */
/** Calculate the checksum of a NMEA 0183 sentence
   
    \param buffer pointer to a 0 terminated buffer
    \param checksum pointer to variable where checksum will be stored

    Returns:
        - 0 if no error
        - 1 if there was an error
        - Fills in *checksum with calculated checksum
        
    This function will calculate the checksum of the string by skipping the
    ! or $ character and stopping at the * character or end of string. 

    Example:
    \code
    char buf[255];
    unsigned char checksum;
    
    strcpy( (char *) buf, "!AIVDM,1,1,,A,15N;<J0P00Jro1<H>bAP0?vL00Rb,0*1B\n" );
    nmea_checksum( buf, &checksum )

    checksum == 0x1B
    \endcode
   
*/
/* ----------------------------------------------------------------------- */
unsigned int __stdcall nmea_checksum( char *buffer, unsigned char *checksum )
{
    char    *p;

    if( !buffer )
        return 1;
    if( !checksum )
        return 1;
  
    /* Find start of sentence, after a '!' or '$' */
    p = find_nmea_start( buffer );
    if( p == NULL )
        return 1;
    p++;
    
    *checksum = 0;
    /* Run checksum until end or illegal character */
    while( (*p != 0) && (*p != '*') && !((*p == '!') || (*p == '$')) )
    {
        *checksum ^= *p;
        p++;
    }
    
    /* Check for illegal character */
    if( *p != '*' )
        return 1;

    return 0;
}


/* ----------------------------------------------------------------------- */
/** Check and return the checksum of a NMEA 0183 sentence

    \param buffer pointer to a 0 terminated buffer
    \param checksum pointer to variable where checksum will be stored
   
    Returns:
        - 0 if it does match
        - 1 if it does not match
        - 2 if there was an error

    This function checks the checksum of a string and returns the result.
    The string is expected to start with a ! or $ and end after the
    2 hex checksum characters. Trailing CR and/or LF are optional. The 
    actual checksum is returned in the variable pointed to by checksum.

    Example:
    \code
    char buf[255];
    unsigned char checksum;
    
    strcpy( (char *) buf, "!AIVDM,1,1,,A,15N;<J0P00Jro1<H>bAP0?vL00Rb,0*1B\n" );
    if( check_nmea_checksum( buf, &checksum ) != 0 )
    {
        fprintf( stderr, "Checksum failed\n" );
    }

    checksum == 0x1B
    \endcode
*/
/* ----------------------------------------------------------------------- */
unsigned int __stdcall check_nmea_checksum( char *buffer, unsigned char *checksum )
{
    unsigned int      rv;
    char       *p;
    unsigned char       ssum;
    
    if( !buffer )
        return 2;
    if( !checksum )
        return 2;
    
    if( !( rv = nmea_checksum( buffer, checksum ) ) )
    {
        /* Find the checksum in the sentence */
        if( ( p = strchr( (char *) buffer, '*') ) == NULL )
            return 2;
        
        /* Make sure it found the end of the sentence */
        if( *p == '*' )
        {
            /* Make sure there is a checksum to check */
            if( isxdigit( *(p+1) ) && isxdigit( *(p+2) ) )
            {
                /* Convert the checksum and check it */
                ssum  = ahextobin( p+1 ) << 4;
                ssum += ahextobin( p+2 );
                
                if( ssum == *checksum )
                    return 0;
            } else {
                return 1;  
            }
        } else {
            return 1;
        }
    }
    return 2;
}


