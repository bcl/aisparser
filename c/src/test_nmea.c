/* -----------------------------------------------------------------------
   NMEA 0183 Sentence Parsing Test Module
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "portable.h"
#include "nmea.h"
#include "test_nmea.h"

/*! \file
    \brief NMEA 0183 Sentence Parser Test Module
    \author Brian C. Lane <bcl@brianlane.com>

*/

int test_ahextobin( void )
{
    char c;
    char i;
    
    for( c = '0', i=0; c <= '9'; c++, i++ )
    {
        if( ahextobin( &c ) != i )
        {
            fprintf( stderr, "ahextobin(): failed %d != %c\n", i, c );
            return 0;
        }
    }
    
    for( c = 'A', i=10; c <= 'F'; c++, i++ )
    {
        if( ahextobin( &c ) != i )
        {
            fprintf( stderr, "ahextobin(): failed %d != %c\n", i, c );
            return 0;
        }
    }

    for( c = 'a', i=10; c <= 'f'; c++, i++ )
    {
        if( ahextobin( &c ) != i )
        {
            fprintf( stderr, "ahextobin(): failed %d != %c\n", i, c );
            return 0;
        }
    }
    
    fprintf( stderr, "ahextobin(): Passed\n" );
    return 1;
}



int test_nmea_next_field( void )
{
    char buf[255];
    char *p;
    
    strcpy( buf, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    p = buf;
    if (strncmp(p, "!AIVDM", 6) != 0)
    {
        fprintf( stderr, "nmea_next_field() 1: Failed\n" );
        return 0;
    }
    p = nmea_next_field( p );
    if (strncmp(p, "1,", 2 ) != 0 )
    {
        fprintf( stderr, "nmea_next_field() 2: Failed\n" );
        return 0;
    }
    p = nmea_next_field( p );
    if (strncmp(p, "1,", 2 ) != 0 )
    {
        fprintf( stderr, "nmea_next_field() 3: Failed\n" );
        return 0;
    }
    p = nmea_next_field( p );
    if (strncmp(p, ",", 1 ) != 0 )
    {
        fprintf( stderr, "nmea_next_field() 4: Failed\n" );
        return 0;
    }
    p = nmea_next_field( p );
    if (strncmp(p, "A,", 2 ) != 0 )
    {
        fprintf( stderr, "nmea_next_field() 5: Failed\n" );
        return 0;
    }
    p = nmea_next_field( p );
    if (strncmp(p, "19NWoq000Wrt<RJHEuuqiWlN061d", 27 ) != 0 )
    {
        fprintf( stderr, "nmea_next_field() 6: Failed\n" );
        return 0;
    }
    p = nmea_next_field( p );
    if (strncmp(p, "0*", 2 ) != 0 )
    {
        fprintf( stderr, "nmea_next_field() 7: Failed\n" );
        return 0;
    }
    p = nmea_next_field( p );
    if (strncmp(p, "5F", 2 ) != 0 )
    {
        fprintf( stderr, "nmea_next_field() 8: Failed\n" );
        return 0;
    }

    fprintf( stderr, "nmea_next_field(): Passed\n");    
    return 1;
}


int test_nmea_uint( void )
{
    char buf[255];
    char *p;
    
    strcpy( buf, "42,1122,0531,0703,0707,65535,0");
    p = buf;
    if (nmea_uint(p) != 42)
    {
        fprintf( stderr, "nmea_uint() 1: Failed\n");
        return 0;
    }
    p = nmea_next_field(p);
    if (nmea_uint(p) != 1122)
    {
        fprintf( stderr, "nmea_uint() 2: Failed\n");
        return 0;
    }
    p = nmea_next_field(p);
    if (nmea_uint(p) != 531)
    {
        fprintf( stderr, "nmea_uint() 3: Failed\n");
        return 0;
    }
    p = nmea_next_field(p);
    if (nmea_uint(p) != 703)
    {
        fprintf( stderr, "nmea_uint() 4: Failed\n");
        return 0;
    }
    p = nmea_next_field(p);
    if (nmea_uint(p) != 707)
    {
        fprintf( stderr, "nmea_uint() 5: Failed\n");
        return 0;
    }
    p = nmea_next_field(p);
    if (nmea_uint(p) != 65535)
    {
        fprintf( stderr, "nmea_uint() 6: Failed\n");
        return 0;
    }
    p = nmea_next_field(p);
    if (nmea_uint(p) != 0)
    {
        fprintf( stderr, "nmea_uint() 7: Failed\n");
        return 0;
    }

    fprintf( stderr, "nmea_uint(): Passed\n" );
    return 1;
}


int test_nmea_copy_field( void )
{
    char buf1[255];
    char buf2[255];
    char *p;
    
    strcpy( buf1, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    buf2[0] = 0;
    
    p = buf1;
    nmea_copy_field( buf2, p, 254 );
    if (strcmp(buf2, "!AIVDM") != 0)
    {
        fprintf( stderr, "nmea_copy_field() 1: Failed\n" );
        return 0;
    }
    p = nmea_next_field(p);
    nmea_copy_field( buf2, p, 254 );
    if (strcmp(buf2, "1") != 0)
    {
        fprintf( stderr, "nmea_copy_field() 2: Failed\n" );
        return 0;
    }
    p = nmea_next_field(p);
    nmea_copy_field( buf2, p, 254 );
    if (strcmp(buf2, "1") != 0)
    {
        fprintf( stderr, "nmea_copy_field() 3: Failed\n" );
        return 0;
    }
    p = nmea_next_field(p);
    p = nmea_next_field(p);
    nmea_copy_field( buf2, p, 254 );
    if (strcmp(buf2, "A") != 0)
    {
        fprintf( stderr, "nmea_copy_field() 4: Failed\n" );
        return 0;
    }
    p = nmea_next_field(p);
    nmea_copy_field( buf2, p, 254 );
    if (strcmp(buf2, "19NWoq000Wrt<RJHEuuqiWlN061d") != 0)
    {
        fprintf( stderr, "nmea_copy_field() 5: Failed\n" );
        return 0;
    }
    p = nmea_next_field(p);
    nmea_copy_field( buf2, p, 254 );
    if (strcmp(buf2, "0") != 0)
    {
        fprintf( stderr, "nmea_copy_field() 6: Failed\n" );
        return 0;
    }
    p = nmea_next_field(p);
    nmea_copy_field( buf2, p, 254 );
    if (strcmp(buf2, "5F") != 0)
    {
        fprintf( stderr, "nmea_copy_field() 7: Failed\n" );
        return 0;
    }
    
    fprintf( stderr, "nmea_copy_field(): Passed\n" );
    return 1;    
}


int test_find_nmea_start( void )
{
    char buf[255];
    char *p;

    strcpy( buf, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    if( ( p = find_nmea_start( buf ) ) == NULL )
    {
        fprintf( stderr, "find_nmea_start() 1: failed\n" );
        return 0;
    }
    
    strcpy( (char *) buf, "$PSHI,123,456,789*CC" );
    if( ( p = find_nmea_start( buf ) ) == NULL )
    {
        fprintf( stderr, "find_nmea_start() 2: failed\n" );
        return 0;
    }

    strcpy( (char *) buf, "678,4343,123,585*FF\n$GPGGA,32,121,676,29394,29234*AA\n" );
    if( ( p = find_nmea_start( buf ) ) == NULL )
    {
        fprintf( stderr, "find_nmea_start() 3: failed\n" );
        return 0;
    }
    if (strncmp( p, "$GPGGA,", 7 ) != 0)
    {
        fprintf( stderr, "find_nmea_start() 3.1: failed\n" );
        return 0;
    }

    strcpy( (char *) buf, "678,4343,123,585*FF\n!AIVDM,5,2,a,123,32,584,49494,DDA,*BB\n" );
    if( ( p = find_nmea_start( buf ) ) == NULL )
    {
        fprintf( stderr, "find_nmea_start() 4: failed\n" );
        return 0;
    }
    if (strncmp( p, "!AIVDM,", 7 ) != 0)
    {
        fprintf( stderr, "find_nmea_start() 4.1: failed\n" );
        return 0;
    }

    strcpy( (char *) buf, "678,4343,123,585*FF\n#AIVDM,5,2,a,123,32,584,49494,DDA,*BB\n" );
    if( ( p = find_nmea_start( buf ) ) != NULL )
    {
        fprintf( stderr, "find_nmea_start() 5: failed\n" );
        return 0;
    }

    fprintf( stderr, "find_nmea_start(): Passed\n" );
    return 1;
}


int test_nmea_checksum( void )
{
    char buf[255];
    unsigned char checksum;
    
    strcpy( (char *) buf, "!AIVDM,1,1,,A,15N;<J0P00Jro1<H>bAP0?vL00Rb,0*1B\n" );
    if( (nmea_checksum( buf, &checksum ) != 0) || (checksum != 0x1B) )
    {
        fprintf( stderr, "nmea_checksum() 1: Failed\n" );
        return 0;
    }
    
    strcpy( (char *) buf, "!AIVDM,1,1,,A,403OwpiuFt3Sdo=sbvK=CG7008J3,0*41" );
    if( (nmea_checksum( buf, &checksum ) != 0) || (checksum == 0x41) )
    {
        fprintf( stderr, "nmea_checksum() 2: Failed\n" );
        return 0;
    }
    
    strcpy( (char *) buf, "!AIVDM,1,1,,A,403OwpiuFt3Sdo=sbvK=CG7008J3,0*" );
    if( (nmea_checksum( buf, &checksum ) != 0) || (checksum == 0x41) )
    {
        fprintf( stderr, "nmea_checksum() 4: Failed\n" );
        return 0;
    }

    
    fprintf( stderr, "nmea_checksum(): Passed\n" );
    return 1;
}


int test_check_nmea_checksum( void )
{
    char buf[255];
    unsigned char checksum;
    
    strcpy( (char *) buf, "!AIVDM,1,1,,A,15N;<J0P00Jro1<H>bAP0?vL00Rb,0*1B\n" );
    if( check_nmea_checksum( buf, &checksum ) != 0 )
    {
        fprintf( stderr, "check_nmea_checksum() 1: Failed\n" );
        return 0;
    }
    
    strcpy( (char *) buf, "!AIVDM,1,1,,A,403OwpiuFt3Sdo=sbvK=CG7008J3,0*41" );
    if( check_nmea_checksum( buf, &checksum ) == 0 )
    {
        fprintf( stderr, "check_nmea_checksum() 2: Failed\n" );
        return 0;
    }
    
    strcpy( (char *) buf, "/s:114,r:4242/!AIVDM,1,1,,A,403OwpiuFt3Sdo=sbvK=CG7008J3,0*41" );
    if( check_nmea_checksum( buf, &checksum ) == 0 )
    {
        fprintf( stderr, "check_nmea_checksum() 3: Failed\n" );
        return 0;
    }
    
    fprintf( stderr, "check_nmea_checksum(): Passed\n" );
    return 1;
}
