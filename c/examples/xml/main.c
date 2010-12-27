/* -----------------------------------------------------------------------
   Process AIS messages read from stdin and output an XML structure
   Copyright 2006 by Brian C. Lane
   All Rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "portable.h"
#include "nmea.h"
#include "sixbit.h"
#include "vdm_parse.h"


int main( int argc, char *argv[] )
{
    ais_state     ais;
    char          buf[256];

    /* AIS message structures, only parse ones with positions */
    aismsg_1  msg_1;
    aismsg_2  msg_2;
    aismsg_3  msg_3;
    aismsg_4  msg_4;
    aismsg_9  msg_9;
    aismsg_18 msg_18;
    aismsg_19 msg_19;
    
    /* Position in DD.DDDDDD */
    double lat_dd;
    double long_ddd;
    long   userid;

    /* Clear out the structures */
    memset( &ais, 0, sizeof( ais_state ) );

    /* Output JSON structure header */
    printf("<markers>\n");
    
    /* Process incoming packets from stdin */
    while( !feof(stdin) )
    {
        if (fgets( buf, 255, stdin ) == NULL ) break;

        if (assemble_vdm( &ais, buf ) == 0)
        {
            /* Get the 6 bit message id */
            ais.msgid = (unsigned char) get_6bit( &ais.six_state, 6 );
            
            /* process message with appropriate parser */
            switch( ais.msgid ) {
                case 1:
                    if( parse_ais_1( &ais, &msg_1 ) == 0 )
                    {
                        userid = msg_1.userid;
                        pos2ddd( msg_1.latitude, msg_1.longitude, &lat_dd, &long_ddd );
                    }
                    break;
                            
                case 2:
                    if( parse_ais_2( &ais, &msg_2 ) == 0 )
                    {
                        userid = msg_2.userid;
                        pos2ddd( msg_2.latitude, msg_2.longitude, &lat_dd, &long_ddd );
                    }
                    break;
                            
                case 3:
                    if( parse_ais_3( &ais, &msg_3 ) == 0 )
                    {
                        userid = msg_3.userid;
                        pos2ddd( msg_3.latitude, msg_3.longitude, &lat_dd, &long_ddd );
                    }
                    break;
                            
                case 4:
                    if( parse_ais_4( &ais, &msg_4 ) == 0 )
                    {
                        userid = msg_4.userid;
                        pos2ddd( msg_4.latitude, msg_4.longitude, &lat_dd, &long_ddd );
                    }
                    break;
                            
                case 9:
                    if( parse_ais_9( &ais, &msg_9 ) == 0 )
                    {
                        userid = msg_9.userid;
                        pos2ddd( msg_9.latitude, msg_9.longitude, &lat_dd, &long_ddd );
                    }
                    break;
                    
                case 18:
                    if( parse_ais_18( &ais, &msg_18 ) == 0 )
                    {
                        userid = msg_18.userid;
                        pos2ddd( msg_18.latitude, msg_18.longitude, &lat_dd, &long_ddd );
                    }
                    break;
                    

                case 19:
                    if( parse_ais_19( &ais, &msg_19 ) == 0 )
                    {
                        userid = msg_19.userid;
                        pos2ddd( msg_19.latitude, msg_19.longitude, &lat_dd, &long_ddd );
                    }
                    break;
                    
            }  /* switch msgid */
            printf( "    <marker lat=\"%0.6f\" lng=\"%0.6f\" />\n", lat_dd, long_ddd );
        }  /* if */
    }  /* while */

    /* Output JSON footer */
    printf("</markers>\n");
    
    return 0;
}
