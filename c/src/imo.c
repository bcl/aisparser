/* -----------------------------------------------------------------------
   AIS Parser SDK IMO Binary Data Parsing
   Copyright 2006-2020 by Brian C. Lane <bcl@brianlane.com>
   All rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portable.h"
#include "nmea.h"
#include "sixbit.h"
#include "vdm_parse.h"
#include "imo.h"

/*! \file
    \brief IMO Binary Message Parsing
    \author Copyright 2006-2020 by Brian C. Lane <bcl@brianlane.com>, All Rights Reserved
    \version 1.0

    For exact definitions of each field you should see the IMO SN/Circ. 236
    28 May 2004

    The format of the !AIVDM strings are covered in IEC 61993-2

    AIS Messages 6 and 8 are used to pass the data.

    Includes information like Meteorological and Hydrological data, Dangerous Cargo Indication,
    etc.

	Example Code:
	\code
	ais_state state;
    aismsg_8  message;
	sixbit imo;
	int dac, fi;
	imo1_11 msg1_11;
    unsigned int  result;
    char buf_1[] = "!AIVDM,2,1,2,B,8030ojA?0@=DE3@?BDPA3onQiUFttP1Wh01DE3<1EJ?>0onlkUG0e01I,0*3D";
	char buf_2[] = "!AIVDM,2,2,2,B,h00,2*7D";

    memset( &state, 0, sizeof(state) );
    assemble_vdm( &state, buf_1 );
    assemble_vdm( &state, buf_2 );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
	if (state.msgid == 8 )
	{
	    result = parse_ais_8( &state, &message );

		// Get the seaway info from the payload of the message 8
		memset( &imo, 0, sizeof( imo ) );

		// Copy the DAC and FI from the message 8 over to seaway
		dac = message.app_id >> 6;
		fi = message.app_id & 0x3F;
		imo = message.data;

		// Is it a ...
		if ((dac == 1) && (fi == 11))
		{
			result = parse_imo1_11( &imo, &msg1_11);

			// msg1_11 now holds the water level information
		}
    }
	\endcode

 */

/* ----------------------------------------------------------------------- */
/** Meteorological and Hydrological data

    \param state pointer to parser state
    \param result pointer to result structure

    return
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Meteorological and Hydrological message into
	a imo1_11 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_imo1_11( sixbit *state, imo1_11 *result )
{
    int length;

    if( !state )
        return 1;
    if( !result )
        return 1;

	length = sixbit_length(state);
    if( (length < 0) || (length > 1008) )
        return 2;

    /* Clear out the structure first */
    memset( result, 0, sizeof( imo1_11 ) );

    result->latitude      = (long) get_6bit( state, 24 );
    result->longitude     = (long) get_6bit( state, 25 );
    result->timedate      = (unsigned int) get_6bit( state, 16 );
	result->wind_avg      = (char) get_6bit( state, 7 );
	result->wind_gust     = (char) get_6bit( state, 7 );
	result->wind_dir      = (int) get_6bit( state, 9 );
	result->gust_dir      = (int) get_6bit( state, 9 );
	result->air_temp      = (int) get_6bit( state, 11 );
	result->humidity      = (char) get_6bit( state, 7 );
	result->dew_point     = (int) get_6bit( state, 10 );
	result->pressure      = (int) get_6bit( state, 9 );
	result->tendency      = (char) get_6bit( state, 2 );
	result->visibility    = (unsigned char) get_6bit( state, 8 );
	result->water_level   = (int) get_6bit( state, 9 );
	result->water_trend   = (char) get_6bit( state, 2 );
	result->surface_speed = (unsigned char) get_6bit( state, 8 );
	result->surface_dir   = (int) get_6bit( state, 9 );
	result->speed_2       = (unsigned char) get_6bit( state, 8 );
	result->dir_2         = (int) get_6bit( state, 9 );
	result->level_2       = (char) get_6bit( state, 5 );
	result->speed_3       = (unsigned char) get_6bit( state, 8 );
	result->dir_3         = (int) get_6bit( state, 9 );
	result->level_3       = (char) get_6bit( state, 5 );
	result->wave_height   = (unsigned char) get_6bit( state, 8 );
	result->wave_period   = (char) get_6bit( state, 6 );
	result->wave_dir     = (int) get_6bit( state, 9 );
	result->swell_height  = (unsigned char) get_6bit( state, 8 );
	result->swell_period  = (char) get_6bit( state, 6 );
	result->swell_dir     = (int) get_6bit( state, 9 );
	result->sea_state     = (char) get_6bit( state, 4 );
	result->water_temp    = (int) get_6bit( state, 10 );
	result->precip_type   = (char) get_6bit( state, 3 );
	result->salanity      = (int) get_6bit( state, 9 );
	result->ice           = (char) get_6bit( state, 2 );
	result->spare         = (char) get_6bit( state, 6 );

   	/* Convert the position to signed value */
	result->longitude *= 10;
	result->latitude *= 10;
   	conv_pos( &result->latitude, &result->longitude);

	conv_sign( 0x0400, &result->air_temp );
	conv_sign( 0x0200, &result->dew_point );
	conv_sign( 0x0100, &result->water_level );
	conv_sign( 0x0200, &result->water_temp );

	return 0;
}


/* ----------------------------------------------------------------------- */
/** Dangerous Cargo Indication

    \param state pointer to parser state
    \param result pointer to result structure

    return
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Meteorological and Hydrological message into
	a imo1_12 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_imo1_12( sixbit *state, imo1_12 *result )
{
    int length;
    int j;

    if( !state )
        return 1;
    if( !result )
        return 1;

	length = sixbit_length(state);
    if( (length < 0) || (length > 1008) )
        return 2;

    /* Clear out the structure first */
    memset( result, 0, sizeof( imo1_12 ) );

    j = 0;
    while( j != 6 )
    {
        result->last_port[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->last_port[j] = 0;

	if( get_timetag( state, &result->atd ) )
		return 3;

    j = 0;
    while( j != 6 )
    {
        result->next_port[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->next_port[j] = 0;

	if( get_timetag( state, &result->eta ) )
		return 3;

    j = 0;
    while( j != 20 )
    {
        result->good[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->good[j] = 0;

    j = 0;
    while( j != 5 )
    {
        result->imd[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->imd[j] = 0;

    result->un_number = (int)  get_6bit( state, 13 );
    result->quantity  = (int)  get_6bit( state, 10 );
    result->units     = (char) get_6bit( state, 2 );
    result->spare     = (char) get_6bit( state, 3 );

	return 0;
}


/* ----------------------------------------------------------------------- */
/** Fairway Closed

    \param state pointer to parser state
    \param result pointer to result structure

    return
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Meteorological and Hydrological message into
	a imo1_13 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_imo1_13( sixbit *state, imo1_13 *result )
{
    int length;
    int j;

    if( !state )
        return 1;
    if( !result )
        return 1;

	length = sixbit_length(state);
    if( (length < 0) || (length > 1008) )
        return 2;

    /* Clear out the structure first */
    memset( result, 0, sizeof( imo1_13 ) );

    j = 0;
    while( j != 20 )
    {
        result->reason[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->reason[j] = 0;

    j = 0;
    while( j != 20 )
    {
        result->location_from[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->location_from[j] = 0;

    j = 0;
    while( j != 20 )
    {
        result->location_to[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->location_to[j] = 0;

    result->extension   = (int)  get_6bit( state, 10 );
    result->units       = (char) get_6bit( state, 2 );
    result->from_day    = (char) get_6bit( state, 5 );
    result->from_month  = (char) get_6bit( state, 4 );
    result->from_hour   = (char) get_6bit( state, 5 );
    result->from_minute = (char) get_6bit( state, 6 );
    result->to_day      = (char) get_6bit( state, 5 );
    result->to_month    = (char) get_6bit( state, 4 );
    result->to_hour     = (char) get_6bit( state, 5 );
    result->to_minute   = (char) get_6bit( state, 6 );
    result->spare       = (char) get_6bit( state, 4 );

	return 0;
}


/* ----------------------------------------------------------------------- */
/** Tidal Window

    \param state pointer to parser state
    \param result pointer to result structure

    return
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Meteorological and Hydrological message into
	a imo1_14 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_imo1_14( sixbit *state, imo1_14 *result )
{
    int length;
    int i;

    if( !state )
        return 1;
    if( !result )
        return 1;

	length = sixbit_length(state);
    if( (length < 0) || (length > 1008) )
        return 2;

    /* Clear out the structure first */
    memset( result, 0, sizeof( imo1_14 ) );

    for( i=0; i<3; i++ )
    {
	    result->windows[i].latitude      = (long) get_6bit( state, 27 );
	    result->windows[i].longitude     = (long) get_6bit( state, 28 );
	    result->windows[i].from_hour     = (char) get_6bit( state, 5 );
	    result->windows[i].from_minute   = (char) get_6bit( state, 6 );
	    result->windows[i].to_hour       = (char) get_6bit( state, 5 );
	    result->windows[i].to_minute     = (char) get_6bit( state, 6 );
	    result->windows[i].current_dir   = (int) get_6bit( state, 9 );
	    result->windows[i].current_speed = (char) get_6bit( state, 7 );

   		/* Convert the position to signed value */
   		conv_pos( &result->windows[i].latitude, &result->windows[i].longitude);
	}

	return 0;
}


/* ----------------------------------------------------------------------- */
/** Extended Ship Static and Voyage Related Data

    \param state pointer to parser state
    \param result pointer to result structure

    return
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Meteorological and Hydrological message into
	a imo1_15 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_imo1_15( sixbit *state, imo1_15 *result )
{
    int length;

    if( !state )
        return 1;
    if( !result )
        return 1;

	length = sixbit_length(state);
    if( (length < 0) || (length > 1008) )
        return 2;

    /* Clear out the structure first */
    memset( result, 0, sizeof( imo1_15 ) );

    result->ais_draught = (int)  get_6bit( state, 11 );
    result->spare       = (char) get_6bit( state, 5 );

	return 0;
}


/* ----------------------------------------------------------------------- */
/** Number of Persons on Board

    \param state pointer to parser state
    \param result pointer to result structure

    return
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Meteorological and Hydrological message into
	a imo1_16 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_imo1_16( sixbit *state, imo1_16 *result )
{
    int length;

    if( !state )
        return 1;
    if( !result )
        return 1;

	length = sixbit_length(state);
    if( (length < 0) || (length > 1008) )
        return 2;

    /* Clear out the structure first */
    memset( result, 0, sizeof( imo1_16 ) );

    result->num_persons  = (int)  get_6bit( state, 13 );
    result->spare        = (char) get_6bit( state, 3 );

	return 0;
}


/* ----------------------------------------------------------------------- */
/** Number of Persons on Board

    \param state pointer to parser state
    \param result pointer to result structure

    return
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Meteorological and Hydrological message into
	a imo1_16 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_imo1_17( sixbit *state, imo1_17 *result )
{
    int length;
    int i;
    int j;

    if( !state )
        return 1;
    if( !result )
        return 1;

	length = sixbit_length(state);
    if( (length < 0) || (length > 1008) )
        return 2;

    /* Clear out the structure first */
    memset( result, 0, sizeof( imo1_17 ) );

    for( i=0; i<4; i++ )
    {
		result->targets[i].type = (char) get_6bit( state, 2 );

		switch(result->targets[i].type)
		{
			case 0:	// MMSI
				get_6bit( state, 12 );
				result->targets[i].mmsi = (long) get_6bit( state, 30 );
				break;
			case 1:	// IMO
				get_6bit( state, 12 );
				result->targets[i].imo = (long) get_6bit( state, 30 );
				break;
			case 2:	// Callsign
				j = 0;
			    while( j != 7 )
			    {
			        result->targets[i].callsign[j] = ais2ascii( (char) get_6bit( state, 6 ));
			        j++;
			    }
			    result->targets[i].callsign[j] = 0;
	    		break;
			case 3:	// Other
				j = 0;
			    while( j != 7 )
			    {
			        result->targets[i].other[j] = ais2ascii( (char) get_6bit( state, 6 ));
			        j++;
			    }
			    result->targets[i].other[j] = 0;
				break;
		}
		result->targets[i].spare = (char) get_6bit( state, 4 );
		result->targets[i].latitude = (long) get_6bit( state, 24 );
		result->targets[i].longitude = (long) get_6bit( state, 25 );
		result->targets[i].cog = (int) get_6bit( state, 9 );
		result->targets[i].timestamp = (char) get_6bit( state, 6 );
		result->targets[i].sog = (char) get_6bit( state, 8 );

		/* Convert the position to signed value */
		result->targets[i].longitude *= 10;
		result->targets[i].latitude *= 10;
	   	conv_pos( &result->targets[i].latitude, &result->targets[i].longitude);

	}

	return 0;
}



