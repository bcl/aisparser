/* -----------------------------------------------------------------------
   Test AIS routines
   Copyright 2006-2008 by Brian C. Lane
   All Rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portable.h"
#include "nmea.h"
#include "sixbit.h"
#include "vdm_parse.h"
#include "test_nmea.h"
#include "test_sixbit.h"
#include "test_vdm_parse.h"
#include "test_seaway.h"
#include "test_access.h"


int main( int argc, char *argv[] )
{
    ais_state     ais;
    unsigned int  i;

    /* AIS message structures */
    aismsg_1  msg_1;
    aismsg_2  msg_2;
    aismsg_3  msg_3;
    aismsg_4  msg_4;
    aismsg_5  msg_5;
    aismsg_12 msg_12;
	aismsg_18 msg_18;
	aismsg_24 msg_24;

    char *test_msgs[] = { "19NS7Sp02wo?HETKA2K6mUM20<L=",
                          "35Mk33gOkSG?bLtK?;B2dRO`00`",
                          "403OwpiuIKl:Ro=sbvK=CG700<3b",
                          "55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H==40HtI4i@E531H1QDTVH51DSCS0",
                          "703Owpi9lmaQ",
                          "<03Owph00002QG51D85BP1<5BDQP",
                          "=03Owpi;Eo7`",
                          "?03OwpiGPmD0000",
                          "D03OwphiIN>4",
                          "B52IRsP005=abWRnlQP03w`UkP06",
                          "H52IRsP518Tj0l59D0000000000",
                          "H52IRsTU000000000000000@5120"


    };

    #define NUM_DEMO_MSGS 9
    char *demo_msgs[] = { "!AIVDM,1,1,,B,15MqvC0Oh9G?qinK?VlPhA480@2n,0*1F,123,14",
                          "!AIVDM,1,1,,B,15Mf@6P001G?v68K??4SejL<00Sl,0*71",
                          "!AIVDM,1,1,,B,15Mn4kPP01G?qNvK>:grkOv<0<11,0*55",
                          "!AIVDM,1,1,,B,15O1Pv0022o?GeNKB3f7QV2>00SP,0*26",
                          "!AIVDM,1,1,,B,15MqvC0Oh:G?qj0K?Vp@di4B0@5>,0*44",
                          "!AIVDM,1,1,,B,15Mts3?P@;G8RB@JLbgamrBF0H6B,0*16",
                          "$BSVDM,1,1,,B,15MqvC0Oh:G?qj0K?Vp@di4B0@5>,0*5D",
                          "!AIVDM,1,1,,B,15Mts3?P@;G8RB@JLbgamrBF0H6B,0*16,142,aass,12311",
                          "!AIVDM,1,1,,A,34a=CB1001JwAEhHra0qk4wF2000,0*5B"
    };

    
    if (test_ahextobin() != 1)
    {
        exit(-1);
    }
    if (test_nmea_next_field() != 1)
    {
        exit(-1);
    }
    if (test_nmea_uint() != 1)
    {
        exit(-1);
    }
    if (test_nmea_copy_field() != 1)
    {
        exit(-1);
    }
    if (test_find_nmea_start() != 1)
    {
        exit(-1);
    }
    if (test_nmea_checksum() != 1)
    {
        exit(-1);
    }
    if (test_check_nmea_checksum() != 1)
    {
        exit(-1);
    }

    if (test_binfrom6bit() != 1)
    {
        exit(-1);
    }
    if (test_init_6bit() != 1)
    {
        exit(-1);
    }
    if (test_get_6bit() != 1)
    {
        exit(-1);
    }
    if (test_ais2ascii() != 1)
    {
        exit(-1);
    }
    if (test_conv_pos() != 1)
    {
        exit(-1);
    }
    if (test_pos2ddd() != 1)
    {
        exit(-1);
    }    
    if (test_pos2dmm() != 1)
    {
        exit(-1);
    }    
    if (test_assemble_vdm() != 1)
    {
        exit(-1);
    }
    if( test_ais_1() != 1 )
    {
        exit(-1);
    }
    if( test_ais_3() != 1 )
    {
        exit(-1);
    }
    if( test_ais_4() != 1 )
    {
        exit(-1);
    }
    if( test_ais_5() != 1 )
    {
        exit(-1);
    }
    if( test_ais_7() != 1 )
    {
        exit(-1);
    }
    if( test_ais_9() != 1 )
    {
        exit(-1);
    }
    if( test_ais_12() != 1 )
    {
        exit(-1);
    }
    if( test_ais_13() != 1 )
    {
        exit(-1);
    }
    if( test_ais_15() != 1 )
    {
        exit(-1);
    }
    if( test_ais_18() != 1 )
    {
        exit(-1);
    }
    if( test_ais_20() != 1 )
    {
        exit(-1);
    }
    if( test_ais_24A() != 1 )
    {
        exit(-1);
    }
    if( test_ais_24B() != 1 )
    {
        exit(-1);
    }

    printf("Testing test_msgs\n");
    
    /* Clear out the structures */
    memset( &ais, 0, sizeof( ais_state ) );

    for (i=0; i<12; i++)
    {
        init_6bit( &ais.six_state );
        strcpy( ais.six_state.bits, test_msgs[i] );
        ais.msgid = (unsigned char) get_6bit( &ais.six_state, 6 );
    
        /* Process the AIS message */
        printf( "Message %d\n", ais.msgid );
                
        /* process message with appropriate parser */
        switch( ais.msgid ) {
            case 1:
                if( parse_ais_1( &ais, &msg_1 ) == 0 )
                {
                    /* Do something with results */
                    printf( "MMSI : %09ld\n", msg_1.userid );
                    printf( "Lat  : %ld\n", msg_1.latitude  );
                    printf( "Long : %ld\n", msg_1.longitude  );
                }
                break;
                        
            case 2:
                if( parse_ais_2( &ais, &msg_2 ) == 0 )
                {
                    /* Do something with results */
                    printf( "MMSI : %09ld\n", msg_2.userid );
                    printf( "Lat  : %ld\n", msg_1.latitude  );
                    printf( "Long : %ld\n", msg_1.longitude  );
                }
                break;
                        
            case 3:
                if( parse_ais_3( &ais, &msg_3 ) == 0 )
                {
                    /* Do something with results */
                    printf( "MMSI : %09ld\n", msg_3.userid );
                    printf( "Lat  : %ld\n", msg_1.latitude  );
                    printf( "Long : %ld\n", msg_1.longitude  );
                }
                break;
                        
            case 4:
                if( parse_ais_4( &ais, &msg_4 ) == 0 )
                {
                    /* Do something with results */
                    printf( "MMSI : %09ld\n", msg_4.userid );
                    printf( "Lat  : %ld\n", msg_1.latitude  );
                    printf( "Long : %ld\n", msg_1.longitude  );
                }
                break;
                        
            case 5:
                if( parse_ais_5( &ais, &msg_5 ) == 0 )
                {
                    /* Do something with results */
                    printf( "MMSI        : %09ld\n", msg_5.userid );
                    printf( "Callsign    : %s\n", msg_5.callsign );
                    printf( "Name        : %s\n", msg_5.name );
                    printf( "Destination : %s\n", msg_5.dest );
                }
                break;
                
            case 12:
                if (parse_ais_12(&ais, &msg_12) == 0)
                {
                    /* Do something with the resu;ts */
                    printf( "MMSI        : %09ld\n", msg_12.userid );
                    printf( "Message     : %s\n", msg_12.message );
                }
                break;

            case 18:
                if( parse_ais_18( &ais, &msg_18 ) == 0 )
                {
                    /* Do something with results */
                    printf( "MMSI : %09ld\n", msg_18.userid );
                    printf( "Lat  : %ld\n", msg_18.latitude  );
                    printf( "Long : %ld\n", msg_18.longitude  );
                } 
                break;

            case 24:
                if( parse_ais_24( &ais, &msg_24 ) == 0 )
                {
                    /* Do something with results */
                    printf( "MMSI     : %09ld\n", msg_24.userid );
					if (msg_24.flags & 1)
					{
						printf("Name : %s\n", msg_24.name );
					}
					if (msg_24.flags & 2)
					{
						printf("Callsign : %s\n", msg_24.callsign );
					}
                } 
                break;


        }
    }

    /* St. Lawrence msg8 testing */
	test_seaway();

    printf("Testing demo_msgs\n");
    
    /* Clear out the structures */
    memset( &ais, 0, sizeof( ais_state ) );

    for (i=0; i<NUM_DEMO_MSGS; i++)
    {
        if( assemble_vdm( &ais, demo_msgs[i] ) != 0 )
            continue;

        ais.msgid = (unsigned char) get_6bit( &ais.six_state, 6 );
    
        /* Process the AIS message */
        printf( "%d of %d: %s\n", i+1, NUM_DEMO_MSGS, demo_msgs[i]);
        printf( "msgid       : %d\n", ais.msgid );

        /* process message with appropriate parser */
        switch( ais.msgid ) {
            case 1:
                if( parse_ais_1( &ais, &msg_1 ) == 0 )
                {
                    printf("repeat      : %d\n", msg_1.repeat );
                    printf("userid      : %ld\n", msg_1.userid );
                    printf("nav_status  : %d\n", msg_1.nav_status );
                    printf("rot         : %d\n", msg_1.rot ); 
                    printf("sog         : %d\n", msg_1.sog );
                    printf("pos_acc     : %d\n", msg_1.pos_acc );
                    printf("longitude   : %ld\n", msg_1.longitude );
                    printf("latitude    : %ld\n", msg_1.latitude );
                    printf("cog         : %d\n", msg_1.cog );
                    printf("true        : %d\n", msg_1.true );
                    printf("utc_sec     : %d\n", msg_1.utc_sec );
                    printf("regional    : %d\n", msg_1.regional );
                    printf("spare       : %d\n", msg_1.spare );
                    printf("raim        : %d\n", msg_1.raim );
                    printf("sync_state  : %d\n", msg_1.sync_state );
                    printf("slot_timeout: %d\n", msg_1.slot_timeout );
                    printf("sub_message : %d\n", msg_1.sub_message );
                }
            break;
            case 3:
                if( parse_ais_3( &ais, &msg_3 ) == 0 )
                {
                    printf("repeat      : %d\n", msg_3.repeat );
                    printf("userid      : %ld\n", msg_3.userid );
                    printf("nav_status  : %d\n", msg_3.nav_status );
                    printf("rot         : %d\n", msg_3.rot ); 
                    printf("sog         : %d\n", msg_3.sog );
                    printf("pos_acc     : %d\n", msg_3.pos_acc );
                    printf("longitude   : %ld\n", msg_3.longitude );
                    printf("latitude    : %ld\n", msg_3.latitude );
                    printf("cog         : %d\n", msg_3.cog );
                    printf("true        : %d\n", msg_3.true );
                    printf("utc_sec     : %d\n", msg_3.utc_sec );
                    printf("regional    : %d\n", msg_3.regional );
                    printf("spare       : %d\n", msg_3.spare );
                    printf("raim        : %d\n", msg_3.raim );
                    printf("sync_state  : %d\n", msg_3.sync_state );
                    printf("slot_inc    : %d\n", msg_3.slot_increment );
                    printf("num_slots   : %d\n", msg_3.num_slots );
                    printf("keep        : %d\n", msg_3.keep);
                }


            break;
        }
        printf("\n\n");
    }

	/* Test some possible seaway messages */
	test_seaway_msgs();

	/* Test the seaway and imo messages */
	test_access();

    return 0;
}
