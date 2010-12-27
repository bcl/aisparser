/* -----------------------------------------------------------------------
   AIS Parser SDK
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
   All rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include "portable.h"
#include "sixbit.h"
#include "vdm_parse.h"
#include "seaway.h"
#include "imo.h"
#include "access.h"


void test_access()
{
	int i;
	ais_state state;
    aismsg_8  message;
	seaway1_3 msg1_3;
	sixbit *seaway;
	water_level_report *report;
	timetag *utc_time;
	int dac, fi, spare, msgid;
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

			seaway = get_msg8_data( &message );
			dac = message.app_id >> 6;
			fi = message.app_id & 0x3F; 

			spare = (char) get_6bit( seaway, 2);
			msgid = (char) get_6bit( seaway, 6);

			printf( "AppID: 0x%04X\n", message.app_id );
			printf( "DAC  : 0x%03X (%d)\n", dac, dac );
			printf( "FI   : 0x%02X (%d)\n", fi, fi);
			printf( "Spare: 0x%02X (%d)\n", spare, spare );
			printf( "msgid: 0x%02X (%d)\n", msgid, msgid );

			if ((fi == 1) && (msgid == 3))
			{
				if ( (result = parse_seaway1_3( seaway, &msg1_3)) != 0 )
				{
					fprintf( stderr, "parse_seaway1_3 failed: %d\n", result);
				}

				int r;
				for (r=0; r<6; r++)
				{
					report = get_water_level_report( &msg1_3, r);
					utc_time = get_water_level_utc_time( report );
					
					printf("month     : %d\n", utc_time->month);
					printf("day       : %d\n", utc_time->day);
					printf("hours     : %d\n", utc_time->hours);
					printf("minutes   : %d\n", utc_time->minutes);
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
