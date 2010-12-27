/* -----------------------------------------------------------------------
   AIS Parser SDK St. Lawrence Seaway Data Parsing
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

/*! \file
    \brief St. Lawrence Seaway Message Parsing
    \author Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>, All Rights Reserved
    \version 1.0
    
    For exact definitions of each field you should see the ITU M.1371,
    IEC 62287 and the IALA Clarifications on M.1371

    The format of the !AIVDM strings are covered in IEC 61993-2

    St. Lawrence Seaway messages are documented in the document titled -
    "St. Lawrence Seaway AIS Data Messaging Formats and Specifications"
    available from http://www.greatlakes-seaway.com/en/pdf/aisdata.pdf

    AIS Messages 6 and 8 are used to pass the data.
    
    Includes information like wind information, water level and flow, lock order,
    lock time.

	Example Code:
	\code
	ais_state state;
    aismsg_8  message;
	sixbit seaway;
	int dac, fi, spare, msgid;
	seaway1_3 msg1_3;
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
		memset( &seaway, 0, sizeof( seaway ) );

		// Copy the DAC and FI from the message 8 over to seaway
		dac = message.app_id >> 6;
		fi = message.app_id & 0x3F; 
		seaway = message.data;

		// Get the Seaway msgid
		spare = (char) get_6bit( &seaway, 2);
		msgid = (char) get_6bit( &seaway, 6);

		// Is it a Water Level Message?
		if ((fi == 1) && (msgid == 3))
		{
			result = parse_seaway1_3( &seaway, &msg1_3);
			
			// msg1_3 now holds the water level information
		}
    }
	\endcode

 */


/* ----------------------------------------------------------------------- */
/** Weather Station Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Weather Station message into seaway1_1 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_seaway1_1( sixbit *state, seaway1_1 *result )
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
    memset( result, 0, sizeof( seaway1_1 ) );

    for( i=0; i<4; i++ )
    {
        if( get_timetag( state, &result->report[i].utc_time ) )
			return 3;

        /* Get the Callsign, convert to ASCII */
        j = 0;
        while( j != 7 )
        {
            result->report[i].station_id[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->report[i].station_id[j] = 0;

        result->report[i].longitude = (long) get_6bit( state, 25 );
        result->report[i].latitude  = (long) get_6bit( state, 24 );
        result->report[i].speed     = (int) get_6bit( state, 10 );
        result->report[i].gust      = (int) get_6bit( state, 10 );
        result->report[i].direction = (int) get_6bit( state, 9 );
        result->report[i].pressure  = (int) get_6bit( state, 14 );
        result->report[i].air_temp  = (int) get_6bit( state, 10 );
        result->report[i].dew_point = (int) get_6bit( state, 10 );
        result->report[i].visibility= (unsigned char) get_6bit( state, 8 );
        result->report[i].water_temp= (int) get_6bit( state, 10 );
        
    	/* Convert the position to signed value */
		result->report[i].longitude *= 10;
		result->report[i].latitude *= 10;
    	conv_pos( &result->report[i].latitude, &result->report[i].longitude);

		/* Convert the air_temp, dew_point and water_temp to a signed value */
		conv_sign( 0x0200, &result->report[i].air_temp);
		conv_sign( 0x0200, &result->report[i].dew_point);
		conv_sign( 0x0200, &result->report[i].water_temp);

        /* Is there enough data for another? */
        if( sixbit_length(state) < 192)
            break;
    }
    return 0;
}


/* ----------------------------------------------------------------------- */
/** Wind Information Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Wind Information message into seaway1_2 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_seaway1_2( sixbit *state, seaway1_2 *result )
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
    memset( result, 0, sizeof( seaway1_2 ) );

    for( i=0; i<6; i++ )
    {
        if( get_timetag( state, &result->report[i].utc_time ) )
			return 3;

        /* Get the Callsign, convert to ASCII */
        j = 0;
        while( j != 7 )
        {
            result->report[i].station_id[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->report[i].station_id[j] = 0;

        result->report[i].longitude = (long) get_6bit( state, 25 );
        result->report[i].latitude  = (long) get_6bit( state, 24 );
        result->report[i].speed     = (int)  get_6bit( state, 10 );
        result->report[i].gust      = (int)  get_6bit( state, 10 );
        result->report[i].direction = (int)  get_6bit( state, 9 );
        result->report[i].spare     = (char) get_6bit( state, 4 );

    	/* Convert the position to signed value */
		result->report[i].longitude *= 10;
		result->report[i].latitude *= 10;
    	conv_pos( &result->report[i].latitude, &result->report[i].longitude);
        
        /* Is there enough data for another? */
        if( sixbit_length(state) < 144)
            break;
    }
    return 0;
}


/* ----------------------------------------------------------------------- */
/** Water Level Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Water Level message into seaway1_2 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_seaway1_3( sixbit *state, seaway1_3 *result )
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
    memset( result, 0, sizeof( seaway1_3 ) );

    for( i=0; i<6; i++ )
    {
        if( get_timetag( state, &result->report[i].utc_time ) )
			return 3;

        /* Get the Callsign, convert to ASCII */
        j = 0;
        while( j != 7 )
        {
            result->report[i].station_id[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->report[i].station_id[j] = 0;

        result->report[i].longitude = (long) get_6bit( state, 25 );
        result->report[i].latitude  = (long) get_6bit( state, 24 );
        result->report[i].type      = (char) get_6bit( state, 1 );
        result->report[i].level     = (unsigned int) get_6bit( state, 16 );
        result->report[i].datum     = (char) get_6bit( state, 2 );
        result->report[i].spare     = (int)  get_6bit( state, 14 );

    	/* Convert the position to signed value */
		result->report[i].longitude *= 10;
		result->report[i].latitude *= 10;
    	conv_pos( &result->report[i].latitude, &result->report[i].longitude);

		/* Convert level to a signed integer */
		conv_sign( 0x8000, &result->report[i].level );

        /* Is there enough data for another? */
        if( sixbit_length(state) < 144)
            break;
    }
    return 0;
}


/* ----------------------------------------------------------------------- */
/** Water Flow Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Water Flow message into seaway1_2 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_seaway1_6( sixbit *state, seaway1_6 *result )
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
    memset( result, 0, sizeof( seaway1_6 ) );

    for( i=0; i<6; i++ )
    {
        if( get_timetag( state, &result->report[i].utc_time ) )
			return 3;

        /* Get the Callsign, convert to ASCII */
        j = 0;
        while( j != 7 )
        {
            result->report[i].station_id[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->report[i].station_id[j] = 0;

        result->report[i].longitude = (long) get_6bit( state, 25 );
        result->report[i].latitude  = (long) get_6bit( state, 24 );
        result->report[i].flow      = (int)  get_6bit( state, 14 );
        result->report[i].spare     = (long) get_6bit( state, 19 );

    	/* Convert the position to signed value */
		result->report[i].longitude *= 10;
		result->report[i].latitude *= 10;
    	conv_pos( &result->report[i].latitude, &result->report[i].longitude);

        /* Is there enough data for another? */
        if( sixbit_length(state) < 144)
            break;
    }
    return 0;
}


/* ----------------------------------------------------------------------- */
/** Lockage Order Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Lockage Order message into seaway1_2 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_seaway2_1( sixbit *state, seaway2_1 *result )
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
    memset( result, 0, sizeof( seaway2_1 ) );

    if( get_timetag( state, &result->utc_time ) )
		return 3;

    /* Get the Callsign, convert to ASCII */
    j = 0;
    while( j != 7 )
    {
        result->lock_id[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->lock_id[j] = 0;

    result->longitude = (long) get_6bit( state, 25 );
    result->latitude  = (long) get_6bit( state, 24 );
    result->spare2    = (int) get_6bit( state, 9 );

   	/* Convert the position to signed value */
	result->longitude *= 10;
	result->latitude *= 10;
   	conv_pos( &result->latitude, &result->longitude);

    for( i=0; i<6; i++ )
    {
        /* Get the Callsign, convert to ASCII */
        j = 0;
        while( j != 15 )
        {
            result->schedule[i].name[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->schedule[i].name[j] = 0;

        result->schedule[i].direction = (char) get_6bit( state, 1 );
		if ( get_timetag( state, &result->schedule[i].eta ) )
			return 4;
        result->schedule[i].spare     = (int)  get_6bit( state, 9 );

        /* Is there enough data for another? */
        if( sixbit_length(state) < 120)
            break;
    }
    return 0;
}


/* ----------------------------------------------------------------------- */
/** Estimated Lock Times Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Estimated Lock Times message into seaway1_2 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_seaway2_2( sixbit *state, seaway2_2 *result )
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
    memset( result, 0, sizeof( seaway2_2 ) );

    if( get_timetag( state, &result->utc_time ) )
		return 3;

    /* Get the vessel name, convert to ASCII */
    j = 0;
    while( j != 15 )
    {
        result->name[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->name[j] = 0;

    /* Get the Last Location, convert to ASCII */
    j = 0;
    while( j != 7 )
    {
        result->last_location[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->last_location[j] = 0;

    if( get_timetag( state, &result->last_ata ) )
		return 3;

    /* Get the First Lock, convert to ASCII */
    j = 0;
    while( j != 7 )
    {
        result->first_lock[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->first_lock[j] = 0;

    if( get_timetag( state, &result->first_eta ) )
		return 3;

    /* Get the Second Lock, convert to ASCII */
    j = 0;
    while( j != 7 )
    {
        result->second_lock[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->second_lock[j] = 0;

    if( get_timetag( state, &result->second_eta ) )
		return 3;

    /* Get the Delay, convert to ASCII */
    j = 0;
    while( j != 7 )
    {
        result->delay[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->delay[j] = 0;

    result->spare2 = (int) get_6bit( state, 4 );

    return 0;
}


/* ----------------------------------------------------------------------- */
/** Version Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Version message into seaway1_2 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_seaway32_1( sixbit *state, seaway32_1 *result )
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
    memset( result, 0, sizeof( seaway32_1 ) );

    result->major = (unsigned char)  get_6bit( state, 8 );
    result->minor = (unsigned char)  get_6bit( state, 8 );
    result->spare2= (unsigned char)  get_6bit( state, 8 );

    return 0;
}


/* ----------------------------------------------------------------------- */
/** PAWSS Message 1.4 - Hydro/Current 

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Hydro/Current message into seaway1_1 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_pawss1_4( sixbit *state, pawss1_4 *result )
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
    memset( result, 0, sizeof( pawss1_4 ) );

    for( i=0; i<6; i++ )
    {
        if( get_timetag( state, &result->report[i].utc_time ) )
			return 3;

        /* Get the Callsign, convert to ASCII */
        j = 0;
        while( j != 7 )
        {
            result->report[i].station_id[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->report[i].station_id[j] = 0;

        result->report[i].longitude = (long) get_6bit( state, 25 );
        result->report[i].latitude  = (long) get_6bit( state, 24 );
        result->report[i].speed     = (unsigned char) get_6bit( state, 8 );
        result->report[i].direction = (int) get_6bit( state, 9 );
        result->report[i].spare     = (unsigned int) get_6bit( state, 16 );

    	/* Convert the position to signed value */
		result->report[i].longitude *= 10;
		result->report[i].latitude *= 10;
    	conv_pos( &result->report[i].latitude, &result->report[i].longitude);

        /* Is there enough data for another? */
        if( sixbit_length(state) < 144)
            break;
    }
    return 0;
}


/* ----------------------------------------------------------------------- */
/** PAWSS Message 1.5 - Hydro/Salinity Temp Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Hydro/Salinity Temp message into seaway1_1 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_pawss1_5( sixbit *state, pawss1_5 *result )
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
    memset( result, 0, sizeof( pawss1_5 ) );

    for( i=0; i<6; i++ )
    {
        if( get_timetag( state, &result->report[i].utc_time ) )
			return 3;

        /* Get the Callsign, convert to ASCII */
        j = 0;
        while( j != 7 )
        {
            result->report[i].station_id[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->report[i].station_id[j] = 0;

        result->report[i].longitude = (long) get_6bit( state, 25 );
        result->report[i].latitude  = (long) get_6bit( state, 24 );
        result->report[i].salinity  = (int) get_6bit( state, 10 );
        result->report[i].water_temp= (int) get_6bit( state, 10 );
        result->report[i].spare     = (unsigned int) get_6bit( state, 13 );

    	/* Convert the position to signed value */
		result->report[i].longitude *= 10;
		result->report[i].latitude *= 10;
    	conv_pos( &result->report[i].latitude, &result->report[i].longitude);

		/* Convert water temp to signed value */
		conv_sign( 0x0200, &result->report[i].water_temp);

        /* Is there enough data for another? */
        if( sixbit_length(state) < 144)
            break;
    }
    return 0;
}


/* ----------------------------------------------------------------------- */
/** PAWSS Message 2.3 - Vessel Procession Order Message

    \param state pointer to parser state
    \param result pointer to result structure
   
    return 
      - return 0 if there was no error
      - return 1 if there was an error

    This function parses a Vessel Procession Order Message into seaway1_1 structure
*/
/* ----------------------------------------------------------------------- */
int __stdcall parse_pawss2_3( sixbit *state, pawss2_3 *result )
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
    memset( result, 0, sizeof( pawss2_3 ) );

    if( get_timetag( state, &result->utc_time ) )
		return 3;

    /* Get the Direction, convert to ASCII */
    j = 0;
    while( j != 16 )
    {
        result->direction[j] = ais2ascii( (char) get_6bit( state, 6 ));
        j++;
    }
    result->direction[j] = 0;

    result->longitude = (long) get_6bit( state, 25 );
    result->latitude  = (long) get_6bit( state, 24 );
    result->spare2 = (char)  get_6bit( state, 3 );

   	/* Convert the position to signed value */
	result->longitude *= 10;
	result->latitude *= 10;
   	conv_pos( &result->latitude, &result->longitude);

    for( i=0; i<4; i++ )
    {
        result->report[i].order  = (char) get_6bit( state, 5 );
        
        /* Get the Vessel Name, convert to ASCII */
        j = 0;
        while( j != 15 )
        {
            result->report[i].vessel_name[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->report[i].vessel_name[j] = 0;

        /* Get the Vessel Name, convert to ASCII */
        j = 0;
        while( j != 13 )
        {
            result->report[i].position_name[j] = ais2ascii( (char) get_6bit( state, 6 ));
            j++;
        }
        result->report[i].position_name[j] = 0;
        
        result->report[i].time_hh = (char) get_6bit( state, 5 );
        result->report[i].time_mm = (char) get_6bit( state, 6 );
        result->report[i].spare   = (char) get_6bit( state, 6 );

        /* Is there enough data for another? */
        if( sixbit_length(state) < 184)
            break;
    }
    return 0;
}

