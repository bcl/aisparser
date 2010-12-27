/* -----------------------------------------------------------------------
   Speed Test AIS Parser SDK
   Copyright 2008 by Brian C. Lane
   All Rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "portable.h"
#include "nmea.h"
#include "sixbit.h"
#include "vdm_parse.h"


void run_test( char *demo_msgs[], unsigned int num_msgs, struct timeval *t_start, struct timeval *t_end )
{
    ais_state     ais;
    unsigned int  i;
	unsigned long x;

    /* AIS message structures */
    aismsg_1  msg_1;
    aismsg_5  msg_5;
	
	/* Clear out the structures */
    memset( &ais, 0, sizeof( ais_state ) );

	gettimeofday( t_start, NULL );
	for(x=0; x<250000L; x++)
	{
	    for (i=0; i<num_msgs; i++)
	    {
	        if( assemble_vdm( &ais, demo_msgs[i] ) != 0 )
	            continue;

	        ais.msgid = (unsigned char) get_6bit( &ais.six_state, 6 );
    
	        /* Process the AIS message */

	        /* process message with appropriate parser */
	        switch( ais.msgid ) {
	            case 1:
	                if( parse_ais_1( &ais, &msg_1 ) == 0 )
	                {
	                }
	            break;
	            case 5:
	                if( parse_ais_5( &ais, &msg_5 ) == 0 )
	                {
	                }
	            break;
	        }
	    }
	}
	gettimeofday( t_end, NULL );
}

double elapsed_time( struct timeval t_start, struct timeval t_end )
{
	double d_start, d_end;
	
	d_start = t_start.tv_sec + (t_start.tv_usec / 1000000.0);
	d_end = t_end.tv_sec + (t_end.tv_usec / 1000000.0);
	
	return d_end - d_start;
}


int main( int argc, char *argv[] )
{
	struct timeval t_start, t_end;
	double acc, avg, speed;
	unsigned int  x;
	
    char *demo_msg1[] = { "!AIVDM,1,1,,B,15MqvC0Oh9G?qinK?VlPhA480@2n,0*1F,123,14",
                          "!AIVDM,1,1,,B,15Mf@6P001G?v68K??4SejL<00Sl,0*71",
                          "!AIVDM,1,1,,B,15Mn4kPP01G?qNvK>:grkOv<0<11,0*55",
                          "!AIVDM,1,1,,B,15O1Pv0022o?GeNKB3f7QV2>00SP,0*26",
                          "!AIVDM,1,1,,B,15MqvC0Oh:G?qj0K?Vp@di4B0@5>,0*44",
                          "!AIVDM,1,1,,B,15NcRf0P3wG?Wq>o=RP?vB0<1J,0*7B",
                          "$BSVDM,1,1,,B,15MqvC0Oh:G?qj0K?Vp@di4B0@5>,0*5D",
                          "!BSVDM,1,1,,B,15NcRf0P3wG?Wq>o=RP?vB0<1J,0*02,142,aass,12311",
    };


    char *demo_msg5[] = { 
				"!AIVDM,2,1,6,B,55ArUT02:nkG<I8GB20nuJ0p5HTu>0hT9860TV16000006420BDi@E53,0*33",
				"!AIVDM,2,2,6,B,1KUDhH888888880,2*6A",
				"!AIVDM,2,1,7,B,55N6RQ000001L@?WWC4h5=B0l4pLv2222222220U1@6335oA0543lU83,0*14",
				"!AIVDM,2,2,7,B,5A33mp888888880,2*49",
				"!AIVDM,2,1,8,B,55N0=SP00001Lt??;OL<PTDJ1<D5A@hF2222220k2@>2640005h00000,0*70",
				"!AIVDM,2,2,8,B,000000000000000,2*2F",
				"!AIVDM,2,1,9,B,55Mj3MP00001LgO73N0dh4PuMT61M<J22222220t3iJ??40Ht<Uli`51,0*57",
				"!AIVDM,2,2,9,B,DSBCPC888888880,2*2B",
				"!AIVDM,2,1,1,B,55Mj2u01o97qMU;?C;5M<J0dUA<522222222220t2Q@>:40Ht<Uli`51,0*16",
				"!AIVDM,2,2,1,B,DSBCPC888888880,2*23",
				"!AIVDM,2,1,2,B,54QKkL01uDfUM@k;SO918v1=E9HETu800000000010e2:40000000000,0*20",
				"!AIVDM,2,2,2,B,000000000000000,2*25",
				"!AIVDM,2,1,5,B,54eGL:00543h=WWG7V0`u<F0p59H4Eb22222220l10A5262k0<S0APDQ,0*62",
				"!AIVDM,2,2,5,B,iH4i@E531H88880,2*24"
				
    };

	/* Run message 1 Test */
	printf("Message1 test\n");
	acc = 0;
	for (x=0; x<5; x++)
	{
		sleep(5);
		run_test( demo_msg1, 7, &t_start, &t_end );
		acc += elapsed_time( t_start, t_end );		
		printf("Run #%d = %0.4f\n", x+1, elapsed_time( t_start, t_end ));
	}
	avg = acc / 5.0;
	speed = (250000.0 * 7) / avg;
	printf("Average = %0.4f\n", avg);
	printf("Speed = %0.4f messages/sec\n", speed);
	
	/* Run message 5 Test */
	printf("Message5 test\n");
	acc = 0;
	for (x=0; x<5; x++)
	{
		sleep(5);
		run_test( demo_msg5, 14, &t_start, &t_end );
		acc += elapsed_time( t_start, t_end );		
		printf("Run #%d = %0.4f\n", x+1, elapsed_time( t_start, t_end ));
	}
	avg = acc / 5.0;
	speed = (250000.0 * 14) / avg;
	printf("Average = %0.4f\n", avg);
	printf("Speed = %0.4f messages/sec\n", speed);
	
	
    return 0;
}
