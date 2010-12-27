/* -----------------------------------------------------------------------
   AIS Parser SDK St. Lawrence Seaway Test functions
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
   All rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "portable.h"
#include "nmea.h"
#include "sixbit.h"
#include "vdm_parse.h"
#include "seaway.h"
#include "test_seaway.h"

/*! \file
    \brief St. Lawrence Seaway Test functions
    \author Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>, All Rights Reserved
    \version 1.0

    This module contains functions to test the St. Lawrence Seaway
    message functions.
    Each function returns a 0 if the test fails and a 1 if it passes.

	These are example messages from the documentation.
	If you have real live messages I can use, please email them to me!
*/
char test_msg1_1[] = "0 1 0 0 1 1 1 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 1 0 1 1 1 1 0 1 0 0 0 0 0 1 0 0 0 1 0 0 1 0 1 0 0 0 0 0 1 0 1 0 1 0 0 1 1 0 1 0 1 0 0 1 1 0 0 0 1 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 1 0 0 0 1 1 0 0 1 0 1 1 1 0 1 0 1 0 0 0 0 0 0 0 0 1 0 0 1 0 1 0 0 1 1 1 0 1 1 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 1 0 0 1 0 0 1 0 1 1 0 0 1 1 1 0 0 0 1 0 0 0 0 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 1 0 1 0 0 0 0 0 0 1 0 0 1 0 0 0 0 1 1 1 1 0 0";
char test_msg1_2[] = "0 1 0 0 1 1 1 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 1 0 1 1 1 1 0 1 0 0 0 0 0 1 0 0 0 1 0 0 1 0 1 0 0 0 0 0 1 0 1 0 1 0 0 1 1 0 1 0 1 0 0 1 1 0 0 0 1 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 1 0 0 0 1 1 0 0 1 0 1 1 1 0 1 0 1 0 0 0 0 0 0 0 0 1 0 0 1 0 1 0 0 1 1 1 0 1 1 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 1 0 0 1 0 0 1 0 1 0 0 0 0";
char test_msg1_3[] = "0 1 0 0 1 1 1 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 1 1 0 1 0 1 1 1 1 0 1 0 0 0 0 0 1 0 0 0 1 0 0 1 0 1 0 0 0 0 0 1 0 1 0 1 0 0 1 1 0 1 0 1 0 0 1 1 0 0 0 1 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 1 0 0 0 1 1 0 0 1 0 1 1 1 0 1 0 1 0 0 0 0 0 0 0 0 1 0 0 1 0 1 0 0 1 1 1 0 1 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
char test_msg1_6[] = "0 1 0 0 1 1 1 1 0 0 0 0 0 0 0 1 0 0 0 0 0 1 1 0 0 1 0 1 1 1 1 0 1 0 0 0 0 0 1 0 0 0 1 0 0 1 0 1 0 0 0 0 0 1 0 1 0 1 0 0 1 1 0 1 0 1 0 0 1 1 0 0 0 1 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 1 0 0 0 1 1 0 0 1 0 1 1 1 0 1 0 1 0 0 0 0 0 0 0 0 1 0 0 1 0 1 0 0 1 1 1 0 1 1 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
char test_msg2_1[] = "0 1 0 0 1 1 1 1 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1 0 1 0 1 1 1 1 0 1 0 0 0 0 0 1 0 0 0 1 0 0 1 0 1 0 0 0 0 0 1 0 1 0 1 0 0 1 1 0 1 0 1 0 0 1 1 0 0 0 1 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 1 1 0 0 0 1 1 0 0 1 0 1 1 1 0 1 0 1 0 0 0 0 0 0 0 0 1 0 0 1 0 1 0 0 1 1 1 0 1 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 1 0 0 1 0 0 1 0 0 1 1 0 0 0 0 1 0 1 1 0 1 1 0 0 1 1 0 0 0 0 0 0 1 0 0 1 1 0 1 0 1 0 0 0 0 0 0 0 1 0 1 0 0 1 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 1 0 1 1 1 1 0 1 0 0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0";
char test_msg2_2[] = "0 1 0 0 1 1 1 1 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 1 0 1 1 1 1 0 1 0 1 1 0 0 0 1 1 1 1 0 0 0 1 1 0 1 0 0 1 0 0 1 0 0 1 1 0 0 0 0 1 0 1 1 0 1 1 0 0 1 1 0 0 0 0 0 0 1 0 0 1 1 0 1 0 1 0 0 0 0 0 0 0 1 0 1 0 0 1 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 1 1 0 0 1 1 0 0 0 1 0 0 1 1 0 1 1 1 1 1 0 0 1 1 0 0 1 1 0 0 0 0 1 1 0 0 0 1 0 1 0 1 1 1 1 0 1 0 1 1 0 1 0 0 0 0 0 0 0 1 0 0 1 1 0 0 1 1 0 0 0 1 0 0 1 1 0 1 1 1 1 1 0 0 1 1 0 0 1 1 0 0 0 0 1 1 0 0 1 0 0 1 0 1 1 1 1 0 1 0 1 1 0 1 0 1 1 1 1 0 0 1 0 0 1 1 0 0 1 1 0 0 0 1 0 0 1 1 0 1 1 1 1 1 0 0 1 1 0 0 1 1 0 0 0 0 1 1 0 0 1 1 0 1 0 1 1 1 1 0 1 0 1 1 1 0 0 0 0 0 0 0 0 1 0 0 1 1 0 0 1 1 0 0 0 1 0 0 1 1 0 1 1 1 1 1 0 0 1 1 0 0 1 1 0 0 0 0 1 1 0 0 0 1 0 0 0 0";
char test_msg32_1[]= "0 1 0 0 1 1 1 1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";


/** Return the next 6-bit ASCII value

    \param str pointer to ASCII binary string

    This function fetches 6-bits at a time from a ASCII binary string and returns
	the binary value.
*/
unsigned char bin2sixbit( char **str )
{
	unsigned int i=0;
	unsigned char value = 0;

	if (!str)
		return 0;

   	while( *str && **str && (i < 6) )
	{
		if ( **str == '1')
		{
			value = value << 1;
			value |= 1;
			i++;
		} else if ( **str == '0') {
			value = value << 1;
			i++;
		}
        (*str)++;
	}
	return binto6bit(value);
}



int test_seaway1_1( void )
{
	char *p;
	char *t;
	char msg[100];
	sixbit seaway;
	seaway1_1 msg1_1;
	
	p = test_msg1_1;
	t = msg;
	while (*p)
	{
		*t = bin2sixbit( &p );
		t++;
		*t = 0;
	}
	// printf("msg: %s\n", msg );

	// Setup the state of the 6-bit parser
	memset( &seaway, 0, sizeof( seaway ) );
	strcpy( seaway.bits, msg);
	seaway.p = seaway.bits;
	seaway.remainder = 0;
	seaway.remainder_bits = 0;

	// Grab the DAC and FI from the 6-bit string
	if ( ((int) get_6bit( &seaway, 10)) != 316)
	{
		fprintf( stderr, "test_seaway1_1() failed: dac != 316\n");
        return 0;
	}

	if ( ((char) get_6bit( &seaway, 6)) != 1)
	{
	    fprintf( stderr, "test_seaway1_1() failed: f1 != 1\n");
        return 0;
    }

	// Get the spare bits
	get_6bit( &seaway, 2);

	// Get the msgid
	if( ((char) get_6bit( &seaway, 6)) != 1)
	{
		fprintf( stderr, "test_seaway1_1() failed: msgid != 1\n");
        return 0;        
	} 

	parse_seaway1_1( &seaway, &msg1_1);

	// Check what the parser returned
	if (msg1_1.report[0].utc_time.month != 5)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].utc_time.month != 5\n");
        return 0;
	}
	if (msg1_1.report[0].utc_time.day != 29)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].utc_time.day != 29\n");
        return 0;
	}
	if (msg1_1.report[0].utc_time.hours != 0)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].utc_time.hours != 0\n");
        return 0;
	}
	if (msg1_1.report[0].utc_time.minutes != 34)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].utc_time.month != 34\n");
        return 0;
	}
	
	if ( strcmp( "TEST1  ", msg1_1.report[0].station_id) != 0)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].station_id != 'TEST1  '\n");
        return 0;
	}

	short lat_dd, long_ddd;
	double lat_min, long_min;
	
	pos2dmm( msg1_1.report[0].latitude, msg1_1.report[0].longitude,
	         &lat_dd, &lat_min,
             &long_ddd, &long_min );
	
	if ( long_ddd != 27)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].longitude (deg)\n");
        return 0;
	}
	if ( long_min != 5)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].longitude (min)\n");
        return 0;
	}
	if ( lat_dd != 5)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].latitude (deg)\n");
        return 0;
	}
	if ( lat_min != 5)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0],latitude (min)\n");
        return 0;
	}

	if ( msg1_1.report[0].speed != 32)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].speed\n");
        return 0;
	}
	
	if ( msg1_1.report[0].gust != 40)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].gust\n");
        return 0;
	}
	
	if ( msg1_1.report[0].direction != 293)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].direction\n");
        return 0;
	}

	if ( msg1_1.report[0].pressure != 10000)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].pressure\n");
        return 0;
	}

	if ( msg1_1.report[0].air_temp != -2)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].air_temp\n");
        return 0;
	}

	if ( msg1_1.report[0].dew_point != -12)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].dew_point\n");
        return 0;
	}

	if ( msg1_1.report[0].visibility != 9)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].visibility\n");
        return 0;
	}
	
	if ( msg1_1.report[0].water_temp != 60)
	{
		fprintf( stderr, "test_seaway1_1() failed: report[0].water_temp\n");
        return 0;
	}

	return 1;
}


int test_seaway1_2( void )
{
	char *p;
	char *t;
	char msg[100];
	sixbit seaway;
	seaway1_2 msg1_2;
	
	p = test_msg1_2;
	t = msg;
	while (*p)
	{
		*t = bin2sixbit( &p );
		t++;
		*t = 0;
	}

	// Setup the state of the 6-bit parser
	memset( &seaway, 0, sizeof( seaway ) );
	strcpy( seaway.bits, msg);
	seaway.p = seaway.bits;
	seaway.remainder = 0;
	seaway.remainder_bits = 0;

	// Grab the DAC and FI from the 6-bit string
	if ( ((int) get_6bit( &seaway, 10)) != 316)
	{
		fprintf( stderr, "test_seaway1_2() failed: dac != 316\n");
        return 0;
	}

	if ( ((char) get_6bit( &seaway, 6)) != 1)
	{
	    fprintf( stderr, "test_seaway1_2() failed: f1 != 1\n");
        return 0;
    }

	// Get the spare bits
	get_6bit( &seaway, 2);

	// Get the msgid
	if( ((char) get_6bit( &seaway, 6)) != 2)
	{
		fprintf( stderr, "test_seaway1_2() failed: msgid != 2\n");
        return 0;        
	} 

	parse_seaway1_2( &seaway, &msg1_2);

	// Check what the parser returned
	if (msg1_2.report[0].utc_time.month != 5)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].utc_time.month != 5\n");
        return 0;
	}
	if (msg1_2.report[0].utc_time.day != 29)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].utc_time.day != 29\n");
        return 0;
	}
	if (msg1_2.report[0].utc_time.hours != 0)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].utc_time.hours != 0\n");
        return 0;
	}
	if (msg1_2.report[0].utc_time.minutes != 34)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].utc_time.month != 34\n");
        return 0;
	}

	if ( strcmp( "TEST1  ", msg1_2.report[0].station_id) != 0)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].station_id != 'TEST1  '\n");
        return 0;
	}

	short lat_dd, long_ddd;
	double lat_min, long_min;
	
	pos2dmm( msg1_2.report[0].latitude, msg1_2.report[0].longitude,
	         &lat_dd, &lat_min,
             &long_ddd, &long_min );
	
	if ( long_ddd != 27)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].longitude (deg)\n");
        return 0;
	}
	if ( long_min != 5)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].longitude (min)\n");
        return 0;
	}
	if ( lat_dd != 5)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].latitude (deg)\n");
        return 0;
	}
	if ( lat_min != 5)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0],latitude (min)\n");
        return 0;
	}

	if ( msg1_2.report[0].speed != 32)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].speed\n");
        return 0;
	}
	
	if ( msg1_2.report[0].gust != 40)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].gust\n");
        return 0;
	}
	
	if ( msg1_2.report[0].direction != 293)
	{
		fprintf( stderr, "test_seaway1_2() failed: report[0].direction\n");
        return 0;
	}

	return 1;
}


int test_seaway1_3( void )
{
	char *p;
	char *t;
	char msg[100];
	sixbit seaway;
	seaway1_3 msg1_3;
	
	p = test_msg1_3;
	t = msg;
	while (*p)
	{
		*t = bin2sixbit( &p );
		t++;
		*t = 0;
	}

	// Setup the state of the 6-bit parser
	memset( &seaway, 0, sizeof( seaway ) );
	strcpy( seaway.bits, msg);
	seaway.p = seaway.bits;
	seaway.remainder = 0;
	seaway.remainder_bits = 0;

	// Grab the DAC and FI from the 6-bit string
	if ( ((int) get_6bit( &seaway, 10)) != 316)
	{
		fprintf( stderr, "test_seaway1_3() failed: dac != 316\n");
        return 0;
	}

	if ( ((char) get_6bit( &seaway, 6)) != 1)
	{
	    fprintf( stderr, "test_seaway1_3() failed: f1 != 1\n");
        return 0;
    }

	// Get the spare bits
	get_6bit( &seaway, 2);

	// Get the msgid
	if( ((char) get_6bit( &seaway, 6)) != 3)
	{
		fprintf( stderr, "test_seaway1_3() failed: msgid != 3\n");
        return 0;        
	} 

	parse_seaway1_3( &seaway, &msg1_3);

	// Check what the parser returned
	if (msg1_3.report[0].utc_time.month != 5)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].utc_time.month != 5\n");
        return 0;
	}
	if (msg1_3.report[0].utc_time.day != 29)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].utc_time.day != 29\n");
        return 0;
	}
	if (msg1_3.report[0].utc_time.hours != 0)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].utc_time.hours != 0\n");
        return 0;
	}
	if (msg1_3.report[0].utc_time.minutes != 34)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].utc_time.month != 34\n");
        return 0;
	}

	if ( strcmp( "TEST1  ", msg1_3.report[0].station_id) != 0)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].station_id != 'TEST1  '\n");
        return 0;
	}

	short lat_dd, long_ddd;
	double lat_min, long_min;
	
	pos2dmm( msg1_3.report[0].latitude, msg1_3.report[0].longitude,
	         &lat_dd, &lat_min,
             &long_ddd, &long_min );
	
	if ( long_ddd != 27)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].longitude (deg)\n");
        return 0;
	}
	if ( long_min != 5)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].longitude (min)\n");
        return 0;
	}
	if ( lat_dd != 5)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].latitude (deg)\n");
        return 0;
	}
	if ( lat_min != 5)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0],latitude (min)\n");
        return 0;
	}

	if ( msg1_3.report[0].type != 0)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].type\n");
        return 0;
	}

	if ( msg1_3.report[0].level != 32)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].level\n");
        return 0;
	}

	if ( msg1_3.report[0].datum != 1)
	{
		fprintf( stderr, "test_seaway1_3() failed: report[0].datum\n");
        return 0;
	}

	return 1;
}


int test_seaway1_6( void )
{
	char *p;
	char *t;
	char msg[100];
	sixbit seaway;
	seaway1_6 msg1_6;
	
	p = test_msg1_6;
	t = msg;
	while (*p)
	{
		*t = bin2sixbit( &p );
		t++;
		*t = 0;
	}

	// Setup the state of the 6-bit parser
	memset( &seaway, 0, sizeof( seaway ) );
	strcpy( seaway.bits, msg);
	seaway.p = seaway.bits;
	seaway.remainder = 0;
	seaway.remainder_bits = 0;

	// Grab the DAC and FI from the 6-bit string
	if ( ((int) get_6bit( &seaway, 10)) != 316)
	{
		fprintf( stderr, "test_seaway1_6() failed: dac != 316\n");
        return 0;
	}

	if ( ((char) get_6bit( &seaway, 6)) != 1)
	{
	    fprintf( stderr, "test_seaway1_6() failed: f1 != 1\n");
        return 0;
    }

	// Get the spare bits
	get_6bit( &seaway, 2);

	// Get the msgid
	if( ((char) get_6bit( &seaway, 6)) != 6)
	{
		fprintf( stderr, "test_seaway1_6() failed: msgid != 6\n");
        return 0;        
	} 

	parse_seaway1_6( &seaway, &msg1_6);

	// Check what the parser returned
	if (msg1_6.report[0].utc_time.month != 5)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].utc_time.month != 5\n");
        return 0;
	}
	if (msg1_6.report[0].utc_time.day != 29)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].utc_time.day != 29\n");
        return 0;
	}
	if (msg1_6.report[0].utc_time.hours != 0)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].utc_time.hours != 0\n");
        return 0;
	}
	if (msg1_6.report[0].utc_time.minutes != 34)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].utc_time.month != 34\n");
        return 0;
	}

	if ( strcmp( "TEST1  ", msg1_6.report[0].station_id) != 0)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].station_id != 'TEST1  '\n");
        return 0;
	}

	short lat_dd, long_ddd;
	double lat_min, long_min;
	
	pos2dmm( msg1_6.report[0].latitude, msg1_6.report[0].longitude,
	         &lat_dd, &lat_min,
             &long_ddd, &long_min );
	
	if ( long_ddd != 27)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].longitude (deg)\n");
        return 0;
	}
	if ( long_min != 5)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].longitude (min)\n");
        return 0;
	}
	if ( lat_dd != 5)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].latitude (deg)\n");
        return 0;
	}
	if ( lat_min != 5)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0],latitude (min)\n");
        return 0;
	}

	if ( msg1_6.report[0].flow != 8192)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].flow\n");
        return 0;
	}

	if ( msg1_6.report[0].spare != 0)
	{
		fprintf( stderr, "test_seaway1_6() failed: report[0].spare\n");
        return 0;
	}


	return 1;
}


int test_seaway2_1( void )
{
	char *p;
	char *t;
	char msg[100];
	sixbit seaway;
	seaway2_1 msg2_1;
	
	p = test_msg2_1;
	t = msg;
	while (*p)
	{
		*t = bin2sixbit( &p );
		t++;
		*t = 0;
	}

	// Setup the state of the 6-bit parser
	memset( &seaway, 0, sizeof( seaway ) );
	strcpy( seaway.bits, msg);
	seaway.p = seaway.bits;
	seaway.remainder = 0;
	seaway.remainder_bits = 0;

	// Grab the DAC and FI from the 6-bit string
	if ( ((int) get_6bit( &seaway, 10)) != 316)
	{
		fprintf( stderr, "test_seaway2_1() failed: dac != 316\n");
        return 0;
	}

	if ( ((char) get_6bit( &seaway, 6)) != 2)
	{
	    fprintf( stderr, "test_seaway2_1() failed: f1 != 2\n");
        return 0;
    }

	// Get the spare bits
	get_6bit( &seaway, 2);

	// Get the msgid
	if( ((char) get_6bit( &seaway, 6)) != 1)
	{
		fprintf( stderr, "test_seaway2_1() failed: msgid != 1\n");
        return 0;        
	} 

	parse_seaway2_1( &seaway, &msg2_1);

	// Check what the parser returned
	if (msg2_1.utc_time.month != 5)
	{
		fprintf( stderr, "test_seaway2_1() failed: utc_time.month != 5\n");
        return 0;
	}
	if (msg2_1.utc_time.day != 29)
	{
		fprintf( stderr, "test_seaway2_1() failed: utc_time.day != 29\n");
        return 0;
	}
	if (msg2_1.utc_time.hours != 0)
	{
		fprintf( stderr, "test_seaway2_1() failed: utc_time.hours != 0\n");
        return 0;
	}
	if (msg2_1.utc_time.minutes != 34)
	{
		fprintf( stderr, "test_seaway2_1() failed: utc_time.month != 34\n");
        return 0;
	}

	if ( strcmp( "TEST1  ", msg2_1.lock_id) != 0)
	{
		fprintf( stderr, "test_seaway2_1() failed: lock_id != 'TEST1  '\n");
        return 0;
	}

	short lat_dd, long_ddd;
	double lat_min, long_min;
	
	pos2dmm( msg2_1.latitude, msg2_1.longitude,
	         &lat_dd, &lat_min,
             &long_ddd, &long_min );
	
	if ( long_ddd != 27)
	{
		fprintf( stderr, "test_seaway2_1() failed: longitude (deg)\n");
        return 0;
	}
	if ( long_min != 5)
	{
		fprintf( stderr, "test_seaway2_1() failed: longitude (min)\n");
        return 0;
	}
	if ( lat_dd != 5)
	{
		fprintf( stderr, "test_seaway2_1() failed: latitude (deg)\n");
        return 0;
	}
	if ( lat_min != 5)
	{
		fprintf( stderr, "test_seaway2_1() failed: latitude (min)\n");
        return 0;
	}

	if ( strcmp( "MILKY STAR     ", msg2_1.schedule[0].name) != 0 )
	{
		fprintf( stderr, "test_seaway2_1() failed: schedule[0].name [%s]\n", msg2_1.schedule[0].name);
        return 0;
	}

	if ( msg2_1.schedule[0].direction != 1)
	{
		fprintf( stderr, "test_seaway2_1() failed: schedule[0].direction\n");
        return 0;
	}

	if (msg2_1.schedule[0].eta.month != 5)
	{
		fprintf( stderr, "test_seaway2_1() failed: utc_time.month != 5\n");
        return 0;
	}
	if (msg2_1.schedule[0].eta.day != 29)
	{
		fprintf( stderr, "test_seaway2_1() failed: utc_time.day != 29\n");
        return 0;
	}
	if (msg2_1.schedule[0].eta.hours != 0)
	{
		fprintf( stderr, "test_seaway2_1() failed: utc_time.hours != 0\n");
        return 0;
	}
	if (msg2_1.schedule[0].eta.minutes != 34)
	{
		fprintf( stderr, "test_seaway2_1() failed: utc_time.month != 34\n");
        return 0;
	}

	return 1;
}


int test_seaway2_2( void )
{
	char *p;
	char *t;
	char msg[100];
	sixbit seaway;
	seaway2_2 msg2_2;
	
	p = test_msg2_2;
	t = msg;
	while (*p)
	{
		*t = bin2sixbit( &p );
		t++;
		*t = 0;
	}

	// Setup the state of the 6-bit parser
	memset( &seaway, 0, sizeof( seaway ) );
	strcpy( seaway.bits, msg);
	seaway.p = seaway.bits;
	seaway.remainder = 0;
	seaway.remainder_bits = 0;

	// Grab the DAC and FI from the 6-bit string
	if ( ((int) get_6bit( &seaway, 10)) != 316)
	{
		fprintf( stderr, "test_seaway2_2() failed: dac != 316\n");
        return 0;
	}

	if ( ((char) get_6bit( &seaway, 6)) != 2)
	{
	    fprintf( stderr, "test_seaway2_2() failed: f1 != 2\n");
        return 0;
    }

	// Get the spare bits
	get_6bit( &seaway, 2);

	// Get the msgid
	if( ((char) get_6bit( &seaway, 6)) != 2)
	{
		fprintf( stderr, "test_seaway2_2() failed: msgid != 2\n");
        return 0;        
	} 

	parse_seaway2_2( &seaway, &msg2_2);

	// Check what the parser returned
	if (msg2_2.utc_time.month != 5)
	{
		fprintf( stderr, "test_seaway2_2() failed: utc_time.month != 5\n");
        return 0;
	}
	if (msg2_2.utc_time.day != 29)
	{
		fprintf( stderr, "test_seaway2_2() failed: utc_time.day != 29\n");
        return 0;
	}
	if (msg2_2.utc_time.hours != 12)
	{
		fprintf( stderr, "test_seaway2_2() failed: utc_time.hours != 12\n");
        return 0;
	}
	if (msg2_2.utc_time.minutes != 30)
	{
		fprintf( stderr, "test_seaway2_2() failed: utc_time.month != 30\n");
        return 0;
	}
	
	if ( strcmp( "MILKY STAR     ", msg2_2.name) != 0 )
	{
		fprintf( stderr, "test_seaway2_2() failed: name\n");
        return 0;
	}

	if ( strcmp( "SLS_L01", msg2_2.last_location) != 0 )
	{
		fprintf( stderr, "test_seaway2_2() failed: last_location\n");
        return 0;
	}

	if (msg2_2.last_ata.month != 5)
	{
		fprintf( stderr, "test_seaway2_2() failed: last_ata.month != 5\n");
        return 0;
	}
	if (msg2_2.last_ata.day != 29)
	{
		fprintf( stderr, "test_seaway2_2() failed: last_ata.day != 29\n");
        return 0;
	}
	if (msg2_2.last_ata.hours != 13)
	{
		fprintf( stderr, "test_seaway2_2() failed: last_ata.hours != 13\n");
        return 0;
	}
	if (msg2_2.last_ata.minutes != 0)
	{
		fprintf( stderr, "test_seaway2_2() failed: last_ata.month != 0\n");
        return 0;
	}

	if ( strcmp( "SLS_L02", msg2_2.first_lock) != 0 )
	{
		fprintf( stderr, "test_seaway2_2() failed: first_lock\n");
        return 0;
	}

	if (msg2_2.first_eta.month != 5)
	{
		fprintf( stderr, "test_seaway2_2() failed: first_ata.month != 5\n");
        return 0;
	}
	if (msg2_2.first_eta.day != 29)
	{
		fprintf( stderr, "test_seaway2_2() failed: first_ata.day != 29\n");
        return 0;
	}
	if (msg2_2.first_eta.hours != 13)
	{
		fprintf( stderr, "test_seaway2_2() failed: first_ata.hours != 13\n");
        return 0;
	}
	if (msg2_2.first_eta.minutes != 30)
	{
		fprintf( stderr, "test_seaway2_2() failed: first_ata.month != 30\n");
        return 0;
	}
	
	if ( strcmp( "SLS_L03", msg2_2.second_lock) != 0 )
	{
		fprintf( stderr, "test_seaway2_2() failed: second_lock\n");
        return 0;
	}

	if (msg2_2.second_eta.month != 5)
	{
		fprintf( stderr, "test_seaway2_2() failed: second_ata.month != 5\n");
        return 0;
	}
	if (msg2_2.second_eta.day != 29)
	{
		fprintf( stderr, "test_seaway2_2() failed: second_ata.day != 29\n");
        return 0;
	}
	if (msg2_2.second_eta.hours != 14)
	{
		fprintf( stderr, "test_seaway2_2() failed: second_ata.hours != 14\n");
        return 0;
	}
	if (msg2_2.second_eta.minutes != 00)
	{
		fprintf( stderr, "test_seaway2_2() failed: second_ata.month != 00\n");
        return 0;
	}

	if ( strcmp( "SLS_L01", msg2_2.delay) != 0 )
	{
		fprintf( stderr, "test_seaway2_2() failed: delay\n");
        return 0;
	}

	return 1;
}


int test_seaway32_1( void )
{
	char *p;
	char *t;
	char msg[100];
	sixbit seaway;
	int dac, fi, msgid;
	seaway32_1 msg32_1;
	
	p = test_msg32_1;
	t = msg;
	while (*p)
	{
		*t = bin2sixbit( &p );
		t++;
		*t = 0;
	}

	// Setup the state of the 6-bit parser
	memset( &seaway, 0, sizeof( seaway ) );
	strcpy( seaway.bits, msg);
	seaway.p = seaway.bits;
	seaway.remainder = 0;
	seaway.remainder_bits = 0;

	// Grab the DAC and FI from the 6-bit string
	if ( (dac = (int) get_6bit( &seaway, 10)) != 316)
	{
		fprintf( stderr, "test_seaway32_1() failed: dac != 316\n");
        return 0;
	}

	if ( (fi = (char) get_6bit( &seaway, 6)) != 32)
	{
	    fprintf( stderr, "test_seaway32_1() failed: f1 != 32 [%d]\n", fi);
        return 0;
    }

	// Get the spare bits
	get_6bit( &seaway, 2);

	// Get the msgid
	if( (msgid = (char) get_6bit( &seaway, 6)) != 1)
	{
		fprintf( stderr, "test_seaway32_1() failed: msgid != 1 [%d]\n", msgid );
        return 0;        
	} 

	parse_seaway32_1( &seaway, &msg32_1);

	// Check what the parser returned
	if (msg32_1.major != 4)
	{
		fprintf( stderr, "test_seaway32_1() failed: major != 4\n");
        return 0;
	}

	// Check what the parser returned
	if (msg32_1.minor != 0)
	{
		fprintf( stderr, "test_seaway32_1() failed: minor != 0\n");
        return 0;
	}
	
	return 1;
}



/** 
*/
void test_seaway()
{
	test_seaway1_1();	
	test_seaway1_2();
	test_seaway1_3();
	test_seaway2_1();
	test_seaway2_2();
	test_seaway32_1();
	
	// Need test functions for the pawss messages
}


	
void old()
{
	char *p;
	char *t;
	char msg[100];
	sixbit seaway;
	int dac, fi, spare, msgid;
	seaway1_1 msg1_1;
	
	p = test_msg1_1;
	t = msg;
	while (*p)
	{
		*t = bin2sixbit( &p );
		t++;
		*t = 0;
	}
	printf("msg: %s\n", msg );

	memset( &seaway, 0, sizeof( seaway ) );
	strcpy( seaway.bits, msg);
	seaway.p = seaway.bits;
	seaway.remainder = 0;
	seaway.remainder_bits = 0;
	
	dac = (int) get_6bit( &seaway, 10);
	fi = (char) get_6bit( &seaway, 6); 
	
	spare = (char) get_6bit( &seaway, 2);
	msgid = (char) get_6bit( &seaway, 6);

	printf( "DAC  : 0x%03X (%d)\n", dac, dac );
	printf( "FI   : 0x%02X (%d)\n", fi, fi);
	printf( "Spare: 0x%02X (%d)\n", spare, spare );
	printf( "msgid: 0x%02X (%d)\n", msgid, msgid );	

	parse_seaway1_1( &seaway, &msg1_1);
	
	short lat_dd, long_ddd;
	double lat_min, long_min;
	
	pos2dmm( msg1_1.report[0].latitude, msg1_1.report[0].longitude,
	         &lat_dd, &lat_min,
             &long_ddd, &long_min );
	
	
	
}


/** Test function to show the info about a msg 8 examples
*/
void test_seaway_msgs()
{
	int i;
	ais_state state;
    aismsg_8  message;
    unsigned int  result;
	char *test_messages[] = {
		"!AIVDM,2,1,2,B,8030ojA?0@=DE3@?BDPA3onQiUFttP1Wh01DE3<1EJ?>0onlkUG0e01I,0*3D",
		"!AIVDM,2,2,2,B,h00,2*7D",
		"!AIVDM,2,1,9,B,8030ojA?0@=DE3C?B5<00o`O1UA@V01vh01DE63>DB3?5oW@PU?d4P1i,0*55",
		"!AIVDM,2,2,9,B,h00,2*76",
		"!AIVDM,2,1,3,B,8030ojA?0@=DE9CD:?B9Fot`9UKQW03Gh01DE9CD6B19?oqHd5H=WP11,0*51",
		"!AIVDM,2,2,3,B,h00,2*7C"
	};


    memset( &state, 0, sizeof(state) );
	for (i=0; i<6; i++)
	{ 
		if( assemble_vdm( &state, test_messages[i] ) == 0)
		{
		    state.msgid = (char) get_6bit( &state.six_state, 6 );
			if (state.msgid != 8)
				continue;
				
		    if( (result = parse_ais_8( &state, &message )) != 0 )
		    {
		        fprintf( stderr, "test_ais_1() failed: error %d\n", result);
		        return;
		    }

			// Get the seaway info from the payload of the message 8
			sixbit seaway;
			int dac, fi, spare, msgid;
			memset( &seaway, 0, sizeof( seaway ) );

			dac = message.app_id >> 6;
			fi = message.app_id & 0x3F; 
			seaway = message.data;

			spare = (char) get_6bit( &seaway, 2);
			msgid = (char) get_6bit( &seaway, 6);

			printf( "AppID: 0x%04X\n", message.app_id );
			printf( "DAC  : 0x%03X (%d)\n", dac, dac );
			printf( "FI   : 0x%02X (%d)\n", fi, fi);
			printf( "Spare: 0x%02X (%d)\n", spare, spare );
			printf( "msgid: 0x%02X (%d)\n", msgid, msgid );	

			seaway1_3 msg1_3;
			if ((fi == 1) && (msgid == 3))
			{
				if ( (result = parse_seaway1_3( &seaway, &msg1_3)) != 0 )
				{
					fprintf( stderr, "parse_seaway1_3 failed: %d\n", result);
				}
				
				int r;
				for (r=0; r<6; r++)
				{
					water_level_report *report = &msg1_3.report[r];

					printf("month     : %d\n", report->utc_time.month);
					printf("day       : %d\n", report->utc_time.day);
					printf("hours     : %d\n", report->utc_time.hours);
					printf("minutes   : %d\n", report->utc_time.minutes);
					printf("station   : %s\n", report->station_id);
					printf("longitude : %ld\n", report->longitude);
					printf("latitude  : %ld\n", report->latitude);
					printf("type      : %d\n", report->type);
					printf("level     : %d\n", report->level);
					printf("datum     : %d\n", report->datum);
					printf("spare     : %d\n", report->spare);
					printf("\n\n");
					
				}
			}
		    memset( &state, 0, sizeof(state) );
		}
	}
}
