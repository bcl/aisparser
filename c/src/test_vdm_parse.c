/* -----------------------------------------------------------------------
   libuais VDM/VDO sentence parser Test functions
   Copyright 2006-2019 by Brian C. Lane <bcl@brianlane.com>
   All rights Reserved
   ----------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portable.h"
#include "nmea.h"
#include "sixbit.h"
#include "vdm_parse.h"

/*! \file
    \brief VDM/VDO Sentence Parser Test functions

    This module contains functions to test the vdm_parser.c functions.
    Each function returns a 0 if the test fails and a 1 if it passes.

*/



int test_ais2ascii( void )
{
    fprintf( stderr, "ais2ascii skipped (passed)\n" );
    return 1;
}


int test_pos2ddd( void )
{
    long latitude;
    long longitude;
    double lat_dd;
    double long_ddd;


    /* North East quadrant (+,+)
       Trondheim, Norway  63.416277, 10.412227
     */
    latitude = 38049766;
    longitude = 6247336;
    pos2ddd( latitude, longitude, &lat_dd, &long_ddd );
    if ((lat_dd!=63.416276666666668) || (long_ddd!= 10.412226666666667))
    {
        fprintf( stderr, "test_pos2ddd() 1: failed\n");
        return 0;
    }

    /* North West quadrant (+,-)
       Lexington Green, USA  42.471021, -71.353514
     */
    latitude = 25482612;
    longitude = -42812108;
    pos2ddd( latitude, longitude, &lat_dd, &long_ddd );
    if ((lat_dd!=42.471020000000003) || (long_ddd!=-71.353513333333339))
    {
        fprintf( stderr, "test_pos2ddd() 2: failed\n");
        return 0;
    }

    /* South East quadrant (-,+)
       Tasmania  -42.000951, 146.594319
     */
    latitude = -25200570;
    longitude = 87956591;
    pos2ddd( latitude, longitude, &lat_dd, &long_ddd );
    if ((lat_dd!=-42.000950000000003) || (long_ddd!=146.59431833333332))
    {
        fprintf( stderr, "test_pos2ddd() 3: failed\n");
        return 0;
    }

    /* South West quadrant (-,-)
       Bolivia  -17.668295, -62.777665
     */
    latitude = -10600977;
    longitude = -37666599;
    pos2ddd( latitude, longitude, &lat_dd, &long_ddd );
    if ((lat_dd!=-17.668295000000001) || (long_ddd!=-62.777664999999999))
    {
        fprintf( stderr, "test_pos2ddd() 4: failed\n");
        return 0;
    }

    /* Center */
    latitude = 0;
    longitude = 0;
    pos2ddd( latitude, longitude, &lat_dd, &long_ddd );
    if ((lat_dd!=0) || (long_ddd!=0))
    {
        fprintf( stderr, "test_pos2ddd() 5: failed\n");
        return 0;
    }

    fprintf( stderr, "test_pos2ddd() Passed\n");
    return 1;
}


int test_pos2dmm( void )
{
    long latitude;
    long longitude;
    short lat_dd;
    double lat_min;
    short long_ddd;
    double long_min;

    /* North East quadrant (+,+)
       Trondheim, Norway  63.416277, 10.412227
     */
    latitude = 38049766;
    longitude = 6247336;
    pos2dmm( latitude, longitude, &lat_dd, &lat_min, &long_ddd, &long_min );
    if (    (lat_dd != 63)
         || (lat_min != 24.976600000000001)
         || (long_ddd != 10)
         || (long_min != 24.733599999999999))
    {
        fprintf( stderr, "test_pos2dmm() 1: failed\n");
        return 0;
    }

    /* North West quadrant (+,-)
       Lexington Green, USA  42.471021, -71.353514
     */
    latitude = 25482612;
    longitude = -42812108;
    pos2dmm( latitude, longitude, &lat_dd, &lat_min, &long_ddd, &long_min );
    if (    (lat_dd != 42)
         || (lat_min != 28.261199999999999)
         || (long_ddd != -71)
         || (long_min != 21.210799999999999))
    {
        fprintf( stderr, "test_pos2dmm() 2: failed\n");
        return 0;
    }

    /* South East quadrant (-,+)
       Tasmania  -42.000951, 146.594319
     */
    latitude = -25200570;
    longitude = 87956591;
    pos2dmm( latitude, longitude, &lat_dd, &lat_min, &long_ddd, &long_min );
    if (    (lat_dd != -42)
         || (lat_min != 0.057000000000000002)
         || (long_ddd != 146)
         || (long_min != 35.659100000000002))
    {
        fprintf( stderr, "test_pos2dmm() 3: failed\n");
        return 0;
    }

    /* South West quadrant (-,-)
       Bolivia  -17.668295, -62.777665
     */
    latitude = -10600977;
    longitude = -37666599;
    pos2dmm( latitude, longitude, &lat_dd, &lat_min, &long_ddd, &long_min );
    if (    (lat_dd != -17)
         || (lat_min != 40.097700000000003)
         || (long_ddd != -62)
         || (long_min != 46.6599))
    {
        fprintf( stderr, "test_pos2dmm() 4: failed\n");
        return 0;
    }

    /* Center */
    latitude = 0;
    longitude = 0;
    pos2dmm( latitude, longitude, &lat_dd, &lat_min, &long_ddd, &long_min );
    if (    (lat_dd != 0)
         || (lat_min != 0)
         || (long_ddd != 0)
         || (long_min != 0))
    {
        fprintf( stderr, "test_pos2ddd() 5: failed\n");
        return 0;
    }

    fprintf( stderr, "test_pos2ddd() Passed\n");
    return 1;



}


int test_conv_pos( void )
{
    long latitude;
    long longitude;


    /* North East quadrant (+,+)
       Trondheim, Norway  63.416277, 10.412227
     */
    latitude = 38049766;
    longitude = 6247336;
    conv_pos( &latitude, &longitude );
    if ((latitude!=38049766) || (longitude!=6247336))
    {
        fprintf( stderr, "test_conv_pos() 1: failed\n");
        return 0;
    }

    /* North West quadrant (+,-)
       Lexington Green, USA  42.471021, -71.353514
     */
    latitude = 25482612;
    longitude = 225623348;
    conv_pos( &latitude, &longitude );
    if ((latitude!=25482612) || (longitude!=-42812108))
    {
        fprintf( stderr, "test_conv_pos() 2: failed\n");
        return 0;
    }

    /* South East quadrant (-,+)
       Tasmania  -42.000951, 146.594319
     */
    latitude = 109017158;
    longitude = 87956591;
    conv_pos( &latitude, &longitude );
    if ((latitude!=-25200570) || (longitude!=87956591))
    {
        fprintf( stderr, "test_conv_pos() 3: failed\n");
        return 0;
    }

    /* South West quadrant (-,-)
       Bolivia  -17.668295, -62.777665
     */
    latitude = 123616751;
    longitude = 230768857;
    conv_pos( &latitude, &longitude );
    if ((latitude!=-10600977) || (longitude!=-37666599))
    {
        fprintf( stderr, "test_conv_pos() 4: failed\n");
        return 0;
    }

    /* Center */
    latitude = 0;
    longitude = 0;
    conv_pos( &latitude, &longitude );
    if ((latitude!=0) || (longitude!=0))
    {
        fprintf( stderr, "test_conv_pos() 5: failed\n");
        return 0;
    }

    fprintf( stderr, "test_conv_pos() Passed\n");
    return 1;
}

int test_conv_pos27( void )
{
	long latitude;
	long longitude;


	/* North East quadrant (+,+)
       Trondheim, Norway  63.416277, 10.412227
	 */
	latitude = 38049;
	longitude = 6247;
	conv_pos27( &latitude, &longitude );
	if ((latitude!=38049000) || (longitude!=6247000))
	{
		fprintf( stderr, "test_conv_pos27() 1: failed\n");
		return 0;
	}

	/* North West quadrant (+,-)
       Lexington Green, USA  42.471021, -71.353514
	 */
	latitude = 25482;
	longitude = 219331;
	conv_pos27( &latitude, &longitude );
	if ((latitude!=25482000) || (longitude!=-42813000))
	{
		fprintf( stderr, "test_conv_pos27() 2: failed\n");
		return 0;
	}

	/* South East quadrant (-,+)
       Tasmania  -42.000951, 146.594319
	 */
	latitude = 105871;
	longitude = 87956;
	conv_pos27( &latitude, &longitude );
	if ((latitude!=-25201000) || (longitude!=87956000))
	{
		fprintf( stderr, "test_conv_pos27() 3: failed\n");
		return 0;
	}

	/* South West quadrant (-,-)
       Bolivia  -17.668295, -62.777665
	 */
	latitude = 120472;
	longitude = 224477;
	conv_pos27( &latitude, &longitude );
	if ((latitude!=-10600000) || (longitude!=-37667000))
	{
		fprintf( stderr, "test_conv_pos27() 4: failed  %ld %ld\n",latitude,longitude);
		return 0;
	}

	/* Center */
	latitude = 0;
	longitude = 0;
	conv_pos27( &latitude, &longitude );
	if ((latitude!=0) || (longitude!=0))
	{
		fprintf( stderr, "test_conv_pos27() 5: failed\n");
		return 0;
	}

	fprintf( stderr, "test_conv_pos27() Passed\n");
	return 1;
}


int test_assemble_vdm( void )
{
    ais_state state;
    char *buf[255] = { "!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n",
                       "!AIVDM,2,1,9,A,55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H,0*49\r\n",
                       "!AIVDM,2,2,9,A,==40HtI4i@E531H1QDTVH51DSCS0,2*16\r\n",
                       /* Out of order 2nd part */
                       "!AIVDM,3,1,4,B,53nFBv01SJ<thHp6220H4heHTf2222222222221?50:454o<`9QSlUDp,0*05\r\n",
                       "!AIVDM,3,3,4,B,==40HtI4i@E531H1QDTVH51DSCS0,2*18\r\n",
                       /* Out of sequence, good part # */
                       "!AIVDM,2,1,3,B,55P5TL01VIaAL@7WKO@mBplU@<PDhh000000001S;AJ::4A80?4i@E53,0*3E\r\n",
                       "!AIVDM,2,2,7,B,1@0000000000000,2*51\r\n",
                       /* No start to the sequence */
                       "!BSVDM,2,2,4,A,54SkDki@000,2*06\r\n",
                       /* Good part # and sequence, wrong channel */
                       "!AIVDM,2,1,9,A,55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H,0*49\r\n",
                       "!AIVDM,2,2,9,B,==40HtI4i@E531H1QDTVH51DSCS0,2*15\r\n",
                       /* Wrong part #, wrong sequence */
                       "!AIVDM,2,1,3,B,55P5TL01VIaAL@7WKO@mBplU@<PDhh000000001S;AJ::4A80?4i@E53,0*3E\r\n",
                       "!AIVDM,2,1,5,B,1@0000000000000,2*50\r\n",
                       };

    memset( &state, 0, sizeof(state) );

    if (assemble_vdm( &state, buf[0] ) != 0)
    {
        fprintf( stderr, "test_assemble_vdm() 1: failed\n" );
        return 0;
    }
    if (strcmp( state.six_state.bits, "19NS7Sp02wo?HETKA2K6mUM20<L=" ) != 0)
    {
        fprintf( stderr, "test_assemble_vdm() 2: failed\n" );
        return 0;
    }

    if (assemble_vdm( &state, buf[1] ) != 1)
    {
        fprintf( stderr, "test_assemble_vdm() 3: failed\n" );
        return 0;
    }

    if (assemble_vdm( &state, buf[2] ) != 0)
    {
        fprintf( stderr, "test_assemble_vdm() 4: failed\n" );
        return 0;
    }
    if (strcmp( state.six_state.bits, "55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H==40HtI4i@E531H1QDTVH51DSCS0" ) != 0)
    {
        fprintf( stderr, "test_assemble_vdm() 5: failed\n" );
        return 0;
    }

    /* Test out of order 2nd part */
    if (assemble_vdm( &state, buf[3] ) != 1)
    {
        fprintf( stderr, "test_assemble_vdm() 6: failed\n" );
        return 0;
    }
    /* Out of order should return a 5 */
    if (assemble_vdm( &state, buf[4] ) != 5)
    {
        fprintf( stderr, "test_assemble_vdm() 7: failed\n" );
        return 0;
    }

    /* Test out of sequence, good part # */
    if (assemble_vdm( &state, buf[5] ) != 1)
    {
        fprintf( stderr, "test_assemble_vdm() 8: failed\n" );
        return 0;
    }
    /* Out of sequence should return a 5 */
    if (assemble_vdm( &state, buf[6] ) != 5)
    {
        fprintf( stderr, "test_assemble_vdm() 9: failed\n" );
        return 0;
    }

    /* Test out of order part #, no start */
    if (assemble_vdm( &state, buf[7] ) != 5)
    {
        fprintf( stderr, "test_assemble_vdm() 10: failed\n" );
        return 0;
    }

    /* Good part # and sequence, wrong channel */
    if (assemble_vdm( &state, buf[8] ) != 1)
    {
        fprintf( stderr, "test_assemble_vdm() 11: failed\n" );
        return 0;
    }
    /* Wrong channel should return 5 */
    if (assemble_vdm( &state, buf[9] ) != 5)
    {
        fprintf( stderr, "test_assemble_vdm() 12: failed\n" );
        return 0;
    }

    /* Wrong part #, wrong sequence */
    if (assemble_vdm( &state, buf[10] ) != 1)
    {
        fprintf( stderr, "test_assemble_vdm() 13: failed\n" );
        return 0;
    }
    /* Wrong part #, wrong sequence should return 5 */
    if (assemble_vdm( &state, buf[11] ) != 5)
    {
        fprintf( stderr, "test_assemble_vdm() 14: failed\n" );
        return 0;
    }

    fprintf( stderr, "test_assemble_vdm() Passed\n");
    return 1;
}


int test_ais_1( void )
{
    ais_state state;
    aismsg_1  message;
    unsigned int  result;
    /* !AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27 */
    char s[] = "19NS7Sp02wo?HETKA2K6mUM20<L=";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_1( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_1() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 1 )
    {
        fprintf( stderr, "test_ais_1() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_1() failed: repeat\n");
        return 0;
    }

    if( message.userid != 636012431 )
    {
        fprintf( stderr, "test_ais_1() failed: userid/MMSI\n");
        return 0;
    }

    if( message.nav_status != 8 )
    {
        fprintf( stderr, "test_ais_1() failed: nav_status\n");
        return 0;
    }

    if( message.rot != 0 )
    {
        fprintf( stderr, "test_ais_1() failed: rot\n");
        return 0;
    }

    if( message.sog != 191 )
    {
        fprintf( stderr, "test_ais_1() failed: sog\n");
        return 0;
    }

    if( message.longitude != -73481550 )
    {
        fprintf( stderr, "test_ais_1() failed: longitude\n");
        return 0;
    }

    if( message.latitude !=  28590700 )
    {
        fprintf( stderr, "test_ais_1() failed: latitude\n");
        return 0;
    }

    if( message.cog != 1750 )
    {
        fprintf( stderr, "test_ais_1() failed: cog\n");
        return 0;
    }

    if( message.true != 174 )
    {
        fprintf( stderr, "test_ais_1() failed: true\n");
        return 0;
    }

    if( message.utc_sec != 33 )
    {
        fprintf( stderr, "test_ais_1() failed: utc_sec\n");
        return 0;
    }

    if( message.regional != 0 )
    {
        fprintf( stderr, "test_ais_1() failed: regional\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_1() failed: spare\n");
        return 0;
    }

    if( message.raim != 0 )
    {
        fprintf( stderr, "test_ais_1() failed: raim\n");
        return 0;
    }

    if( message.sync_state != 0 )
    {
        fprintf( stderr, "test_ais_1() failed: sync_state\n");
        return 0;
    }

    if( message.slot_timeout != 3 )
    {
        fprintf( stderr, "test_ais_1() failed: slot_timeout\n");
        return 0;
    }

    if( message.sub_message != 1805 )
    {
        fprintf( stderr, "test_ais_1() failed: sub_message\n");
        return 0;
    }

    fprintf( stderr, "test_ais_1() passed\n");
    return 1;

}


int test_ais_2( void )
{

    fprintf( stderr, "test_ais_2() failed: Not Written\n");
    return 0;

}


int test_ais_3( void )
{
    ais_state state;
    aismsg_3  message;
    unsigned int  result;
    /* !AIVDM,1,1,,B,35Mk33gOkSG?bLtK?;B2dRO`00`A,0*30 */
    char s[] = "35Mk33gOkSG?bLtK?;B2dRO`00`A";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_3( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_3() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 3 )
    {
        fprintf( stderr, "test_ais_3() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_3() failed: repeat\n");
        return 0;
    }

    if( message.userid != 366789390 )
    {
        fprintf( stderr, "test_ais_3() failed: userid/MMSI\n");
        return 0;
    }

    if( message.nav_status != 15 )
    {
        fprintf( stderr, "test_ais_3() failed: nav_status\n");
        return 0;
    }

    if( message.rot != 127 )
    {
        fprintf( stderr, "test_ais_3() failed: rot\n");
        return 0;
    }

    if( message.sog != 227 )
    {
        fprintf( stderr, "test_ais_3() failed: sog\n");
        return 0;
    }

    if( message.longitude != -73444450 )
    {
        fprintf( stderr, "test_ais_3() failed: longitude\n");
        return 0;
    }

    if( message.latitude != 28560200 )
    {
        fprintf( stderr, "test_ais_3() failed: latitude\n");
        return 0;
    }

    if( message.cog != 690 )
    {
        fprintf( stderr, "test_ais_3() failed: cog\n");
        return 0;
    }

    if( message.true != 79 )
    {
        fprintf( stderr, "test_ais_3() failed: true\n");
        return 0;
    }

    if( message.utc_sec != 52 )
    {
        fprintf( stderr, "test_ais_3() failed: utc_sec\n");
        return 0;
    }

    if( message.regional != 0 )
    {
        fprintf( stderr, "test_ais_3() failed: regional\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_3() failed: spare\n");
        return 0;
    }

    if( message.raim != 0 )
    {
        fprintf( stderr, "test_ais_3() failed: raim\n");
        return 0;
    }

    if( message.sync_state != 0 )
    {
        fprintf( stderr, "test_ais_3() failed: sync_state\n");
        return 0;
    }

    if( message.slot_increment != 161 )
    {
        fprintf( stderr, "test_ais_3() failed: slot_increment\n");
        return 0;
    }

    if( message.num_slots != 0 )
    {
        fprintf( stderr, "test_ais_3() failed: num_slots\n");
        return 0;
    }

    if( message.keep != 1 )
    {
        fprintf( stderr, "test_ais_3() failed: keep\n");
        return 0;
    }

    fprintf( stderr, "test_ais_3() passed\n");
    return 1;
}



int test_ais_4( void )
{
    ais_state state;
    aismsg_4  message;
    unsigned int  result;
    /* !AIVDM,1,1,,A,403OwpiuIKl:Ro=sbvK=CG700<3b,0*5E */
    char s[] = "403OwpiuIKl:Ro=sbvK=CG700<3b";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_4( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_4() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 4 )
    {
        fprintf( stderr, "test_ais_4() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_4() failed: repeat\n");
        return 0;
    }

    if( message.userid != 3669987 )
    {
        fprintf( stderr, "test_ais_4() failed: userid\n");
        return 0;
    }

    if( message.utc_year != 2006 )
    {
        fprintf( stderr, "test_ais_4() failed: utc_year\n");
        return 0;
    }

    if( message.utc_month != 5 )
    {
        fprintf( stderr, "test_ais_4() failed: utc_month\n");
        return 0;
    }

    if( message.utc_day != 23 )
    {
        fprintf( stderr, "test_ais_4() failed: utc_day\n");
        return 0;
    }

    if( message.utc_hour != 20 )
    {
        fprintf( stderr, "test_ais_4() failed: utc_hour\n");
        return 0;
    }

    if( message.utc_minute != 10 )
    {
        fprintf( stderr, "test_ais_4() failed: utc_minute\n");
        return 0;
    }

    if( message.utc_second != 34)
    {
        fprintf( stderr, "test_ais_4() failed: utc_second\n");
        return 0;
    }

    if( message.pos_acc != 1 )
    {
        fprintf( stderr, "test_ais_4() failed: pos_acc\n");
        return 0;
    }

    if( message.longitude != -73671329 )
    {
        fprintf( stderr, "test_ais_4() failed: longitude\n");
        return 0;
    }

    if( message.latitude != 28529500 )
    {
        fprintf( stderr, "test_ais_4() failed: utc_latitude\n");
        return 0;
    }

    if( message.pos_type != 7 )
    {
        fprintf( stderr, "test_ais_4() failed: pos_type\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_4() failed: spare\n");
        return 0;
    }

    if( message.raim != 0 )
    {
        fprintf( stderr, "test_ais_4() failed: raim\n");
        return 0;
    }

    if( message.sync_state != 0 )
    {
        fprintf( stderr, "test_ais_4() failed: sync_state\n");
        return 0;
    }

    if( message.slot_timeout != 3 )
    {
        fprintf( stderr, "test_ais_4() failed: slot_timeout\n");
        return 0;
    }

    if( message.sub_message != 234 )
    {
        fprintf( stderr, "test_ais_4() failed: sub_message\n");
        return 0;
    }

    fprintf( stderr, "test_ais_4() passed\n");
    return 1;

}


int test_ais_5( void )
{

    ais_state state;
    aismsg_5  message;
    unsigned int  result;
    /* !AIVDM,2,1,9,A,55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H,0*49
       !AIVDM,2,2,9,A,==40HtI4i@E531H1QDTVH51DSCS0,2*16 */
    char s[] = "55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H==40HtI4i@E531H1QDTVH51DSCS0";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_5( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_4() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 5 )
    {
        fprintf( stderr, "test_ais_5() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_5() failed: repeat\n");
        return 0;
    }

    if( message.userid != 366710810 )
    {
        fprintf( stderr, "test_ais_5() failed: userid\n");
        return 0;
    }

    if( message.version != 0 )
    {
        fprintf( stderr, "test_ais_5() failed: version\n");
        return 0;
    }

    if( message.imo != 0 )
    {
        fprintf( stderr, "test_ais_5() failed: imo\n");
        return 0;
    }

    if( strcmp(message.callsign, "WYX2158") != 0 )
    {
        fprintf( stderr, "test_ais_5() failed: callsign\n");
        return 0;
    }

    if( strcmp( message.name, "WALLA WALLA         ")  != 0 )
    {
        fprintf( stderr, "test_ais_5() failed: name\n");
        return 0;
    }

    if( message.ship_type != 60 )
    {
        fprintf( stderr, "test_ais_5() failed: ship_type\n");
        return 0;
    }

    if( message.dim_bow != 32 )
    {
        fprintf( stderr, "test_ais_5() failed: dim_bow\n");
        return 0;
    }

    if( message.dim_stern != 88 )
    {
        fprintf( stderr, "test_ais_5() failed: dim_stern\n");
        return 0;
    }

    if( message.dim_port != 13 )
    {
        fprintf( stderr, "test_ais_5() failed: dim_port\n");
        return 0;
    }

    if( message.dim_starboard != 13 )
    {
        fprintf( stderr, "test_ais_5() failed: dim_starboard\n");
        return 0;
    }

    if( message.pos_type != 1 )
    {
        fprintf( stderr, "test_ais_5() failed: pos_type\n");
        return 0;
    }

    if( message.eta != 1596 )
    {
        fprintf( stderr, "test_ais_5() failed: eta\n");
        return 0;
    }

    if( message.draught != 100 )
    {
        fprintf( stderr, "test_ais_5() failed: draught\n");
        return 0;
    }

    if( strcmp(message.dest, "SEATTLE FERRY TERMNL") != 0 )
    {
        fprintf( stderr, "test_ais_5() failed: dest\n");
        return 0;
    }

    if( message.dte != 0 )
    {
        fprintf( stderr, "test_ais_5() failed: dte\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_5() failed: spare\n");
        return 0;
    }

    fprintf( stderr, "test_ais_5() passed\n");
    return 1;
}


int test_ais_6( void )
{

    fprintf( stderr, "test_ais_6() failed: Not Written\n");
    return 0;

}


int test_ais_7( void )
{
    ais_state state;
    aismsg_7  message;
    unsigned int  result;
    /* !AIVDM,1,1,,A,703Owpi9lmaQ,0*3B */
    char s[] = "703Owpi9lmaQ";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_7( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_7() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 7 )
    {
        fprintf( stderr, "test_ais_7() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_7() failed: repeat\n");
        return 0;
    }

    if( message.userid != 3669987 )
    {
        fprintf( stderr, "test_ais_7() failed: userid\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_7() failed: spare\n");
        return 0;
    }

    if( message.destid_1 != 309647000 )
    {
        fprintf( stderr, "test_ais_7() failed: destid_1\n");
        return 0;
    }

    if( message.sequence_1 != 1 )
    {
        fprintf( stderr, "test_ais_7() failed: sequence_1\n");
        return 0;
    }

    if( message.num_acks != 1 )
    {
        fprintf( stderr, "test_ais_7() failed: num_acks\n");
        return 0;
    }

    fprintf( stderr, "test_ais_7() passed\n");
    return 1;
}


int test_ais_8( void )
{

    fprintf( stderr, "test_ais_8() failed: Not Written\n");
    return 0;

}


int test_ais_9( void )
{
    ais_state state;
    aismsg_9  message;
    unsigned int  result;
    /* !AIVDM,1,1,,B,900048wwTcJb0mpF16IobRP2086Q,0*48 */
    char s[] = "900048wwTcJb0mpF16IobRP2086Q";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_9( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 9 )
    {
        fprintf( stderr, "test_ais_9() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: repeat\n");
        return 0;
    }

    if( message.userid != 1059 )
    {
        fprintf( stderr, "test_ais_9() failed: userid\n");
        return 0;
    }

    if( message.altitude != 4094 )
    {
        fprintf( stderr, "test_ais_9() failed: altitude\n");
        return 0;
    }

    if( message.sog != 299 )
    {
        fprintf( stderr, "test_ais_9() failed: sog\n");
        return 0;
    }

    if( message.pos_acc != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: pos_acc\n");
        return 0;
    }

    if( message.longitude != -44824900 )
    {
        fprintf( stderr, "test_ais_9() failed: longitude\n");
        return 0;
    }

    if( message.latitude != 23086695 )
    {
        fprintf( stderr, "test_ais_9() failed: latitude\n");
        return 0;
    }

    if( message.cog != 1962 )
    {
        fprintf( stderr, "test_ais_9() failed: cog\n");
        return 0;
    }

    if( message.utc_sec != 10 )
    {
        fprintf( stderr, "test_ais_9() failed: utc_sec\n");
        return 0;
    }

    if( message.regional != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: regional\n");
        return 0;
    }

    if( message.dte != 1 )
    {
        fprintf( stderr, "test_ais_9() failed: dte\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: spare\n");
        return 0;
    }

    if( message.assigned != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: assigned\n");
        return 0;
    }

    if( message.raim != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: raim\n");
        return 0;
    }

    if( message.comm_state != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: comm_state\n");
        return 0;
    }

    if( message.comm_state != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: comm_state\n");
        return 0;
    }

    if( message.sotdma.sync_state != 0 )
    {
        fprintf( stderr, "test_ais_9() failed: sotdma.sync_state\n");
        return 0;
    }

    if( message.sotdma.slot_timeout != 2 )
    {
        fprintf( stderr, "test_ais_9() failed: sotdma.slot_timeout\n");
        return 0;
    }

    if( message.sotdma.sub_message != 417 )
    {
        fprintf( stderr, "test_ais_9() failed: sotdma.sub_message\n");
        return 0;
    }

    fprintf( stderr, "test_ais_9() passed\n");
    return 1;
}


int test_ais_10( void )
{

    fprintf( stderr, "test_ais_10() failed: Not Written\n");
    return 0;

}


int test_ais_11( void )
{

    fprintf( stderr, "test_ais_11() failed: Not Written\n");
    return 0;

}


int test_ais_12( void )
{
    ais_state state;
    aismsg_12  message;
    unsigned int  result;
    /* !AIVDM,1,1,,A,<03Owph00002QG51D85BP1<5BDQP,0*7D */
    char s[] = "<03Owph00002QG51D85BP1<5BDQP";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_12( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_12() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 12 )
    {
        fprintf( stderr, "test_ais_12() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_12() failed: repeat\n");
        return 0;
    }

    if( message.userid != 3669987 )
    {
        fprintf( stderr, "test_ais_12() failed: userid\n");
        return 0;
    }

    if( message.sequence != 0 )
    {
        fprintf( stderr, "test_ais_12() failed: sequence\n");
        return 0;
    }

    if( message.destination != 0 )
    {
        fprintf( stderr, "test_ais_12() failed: destination\n");
        return 0;
    }

    if( message.retransmit != 1 )
    {
        fprintf( stderr, "test_ais_12() failed: retransmit\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_12() failed: spare\n");
        return 0;
    }

    if( strcmp( message.message, "!WEATHER ALERT! ")  != 0 )
    {
        fprintf( stderr, "test_ais_12() failed: message\n");
        return 0;
    }

    fprintf( stderr, "test_ais_12() passed\n");
    return 1;
}


int test_ais_13( void )
{
    ais_state state;
    aismsg_13  message;
    unsigned int  result;
    /* !AIVDM,1,1,,A,=03Owpi;Eo7`,0*7F */
    char s[] = "=03Owpi;Eo7`";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_13( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_13() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 13 )
    {
        fprintf( stderr, "test_ais_13() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_13() failed: repeat\n");
        return 0;
    }

    if( message.userid != 3669987 )
    {
        fprintf( stderr, "test_ais_13() failed: userid\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_13() failed: spare\n");
        return 0;
    }

    if( message.destid_1 != 316005498 )
    {
        fprintf( stderr, "test_ais_13() failed: destid_1\n");
        return 0;
    }

    if( message.sequence_1 != 0 )
    {
        fprintf( stderr, "test_ais_13() failed: sequence_1\n");
        return 0;
    }

    if( message.num_acks != 1 )
    {
        fprintf( stderr, "test_ais_13() failed: num_acks\n");
        return 0;
    }

    fprintf( stderr, "test_ais_13() passed\n");
    return 1;
}


int test_ais_14( void )
{

    fprintf( stderr, "test_ais_14() failed: Not Written\n");
    return 0;

}


int test_ais_15( void )
{
    ais_state state;
    aismsg_15  message;
    unsigned int  result;
    /* !AIVDM,1,1,,A,?03OwpiGPmD0000,2*07
       Note that this is an invlid interrogation, it does not specify a
       message number or an offset. But it did come from a base station so I
       believe that it is not noise.
    */
    char s[] = "?03OwpiGPmD0000";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_15( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 15 )
    {
        fprintf( stderr, "test_ais_15() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: repeat\n");
        return 0;
    }

    if( message.userid != 3669987 )
    {
        fprintf( stderr, "test_ais_15() failed: userid\n");
        return 0;
    }

    if( message.spare1 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: spare1\n");
        return 0;
    }

    if( message.destid1 != 367056192 )
    {
        fprintf( stderr, "test_ais_15() failed: destid1\n");
        return 0;
    }

    if( message.msgid1_1 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: msgid1_1\n");
        return 0;
    }

    if( message.offset1_1 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: offset1_1\n");
        return 0;
    }

    if( message.spare2 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: spare2\n");
        return 0;
    }

    if( message.msgid1_2 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: msgid1_2\n");
        return 0;
    }

    if( message.offset1_2 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: offset1_2\n");
        return 0;
    }

    if( message.spare3 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: spare3\n");
        return 0;
    }

    if( message.destid2 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: destid2\n");
        return 0;
    }

    if( message.msgid2_1 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: msgid2_1\n");
        return 0;
    }

    if( message.offset2_1 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: offset2_1\n");
        return 0;
    }

    if( message.spare4 != 0 )
    {
        fprintf( stderr, "test_ais_15() failed: spare4\n");
        return 0;
    }

    fprintf( stderr, "test_ais_15() passed\n");
    return 1;
}


int test_ais_16( void )
{

    fprintf( stderr, "test_ais_16() failed: Not Written\n");
    return 0;

}


int test_ais_17( void )
{

    fprintf( stderr, "test_ais_17() failed: Not Written\n");
    return 0;

}


int test_ais_18( void )
{
    ais_state state;
    aismsg_18  message;
    unsigned int  result;
    /* !AIVDM,1,1,,A,B52IRsP005=abWRnlQP03w`UkP06,0*2A */
    char s[] = "B52IRsP005=abWRnlQP03w`UkP06";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_18( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 18 )
    {
        fprintf( stderr, "test_ais_18() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: repeat\n");
        return 0;
    }

    if( message.userid != 338060014 )
    {
        fprintf( stderr, "test_ais_18() failed: userid\n");
        return 0;
    }

    if( message.regional1 != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: regional1\n");
        return 0;
    }

    if( message.sog != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: sog\n");
        return 0;
    }

    if( message.pos_acc != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: pos_acc\n");
        return 0;
    }

    if( message.longitude != -93506225 )
    {
        fprintf( stderr, "test_ais_18() failed: longitude\n");
        return 0;
    }

    if( message.latitude != 11981336 )
    {
        fprintf( stderr, "test_ais_18() failed: latitude\n");
        return 0;
    }

    if( message.cog != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: cog\n");
        return 0;
    }

    if( message.true != 511 )
    {
        fprintf( stderr, "test_ais_18() failed: true\n");
        return 0;
    }

    if( message.utc_sec != 17 )
    {
        fprintf( stderr, "test_ais_18() failed: utc_sec\n");
        return 0;
    }

    if( message.regional2 != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: regional2\n");
        return 0;
    }

    if( message.unit_flag != 1 )
    {
        fprintf( stderr, "test_ais_18() failed: unit_flag\n");
        return 0;
    }

    if( message.display_flag != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: display_flag\n");
        return 0;
    }

    if( message.dsc_flag != 1 )
    {
        fprintf( stderr, "test_ais_18() failed: dsc_flag\n");
        return 0;
    }

    if( message.band_flag != 1 )
    {
        fprintf( stderr, "test_ais_18() failed: band_flag\n");
        return 0;
    }

    if( message.msg22_flag != 1 )
    {
        fprintf( stderr, "test_ais_18() failed: msg22_flag\n");
        return 0;
    }

    if( message.mode_flag != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: mode_flag\n");
        return 0;
    }

    if( message.raim != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: raim\n");
        return 0;
    }

    if( message.comm_state != 1 )
    {
        fprintf( stderr, "test_ais_18() failed: comm_state\n");
        return 0;
    }

    if( message.itdma.sync_state != 3 )
    {
        fprintf( stderr, "test_ais_18() failed: itdma.sync_state\n");
        return 0;
    }

    if( message.itdma.slot_inc != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: itdma.slot_inc\n");
        return 0;
    }

    if( message.itdma.num_slots != 3 )
    {
        fprintf( stderr, "test_ais_18() failed: itdma.num_slots\n");
        return 0;
    }

    if( message.itdma.keep_flag != 0 )
    {
        fprintf( stderr, "test_ais_18() failed: itdma.keep_flag\n");
        return 0;
    }

    fprintf( stderr, "test_ais_18() passed\n");
    return 1;

}


int test_ais_19( void )
{

    fprintf( stderr, "test_ais_19() failed: Not Written\n");
    return 0;

}


int test_ais_20( void )
{
    ais_state state;
    aismsg_20  message;
    unsigned int  result;
    /* !AIVDM,1,1,,A,D03OwphiIN>4,0*25 */
    char s[] = "D03OwphiIN>4";

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_20( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 20 )
    {
        fprintf( stderr, "test_ais_20() failed: msgid\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: repeat\n");
        return 0;
    }

    if( message.userid != 3669987 )
    {
        fprintf( stderr, "test_ais_20() failed: userid\n");
        return 0;
    }

    if( message.spare1 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: spare1\n");
        return 0;
    }

    if( message.offset1 != 790 )
    {
        fprintf( stderr, "test_ais_20() failed: offset1\n");
        return 0;
    }

    if( message.slots1 != 5 )
    {
        fprintf( stderr, "test_ais_20() failed: slots1\n");
        return 0;
    }

    if( message.timeout1 != 7 )
    {
        fprintf( stderr, "test_ais_20() failed: timeout1\n");
        return 0;
    }

    if( message.increment1 != 225 )
    {
        fprintf( stderr, "test_ais_20() failed: increment1\n");
        return 0;
    }

    if( message.offset2 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: offset2\n");
        return 0;
    }

    if( message.slots2 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: slots2\n");
        return 0;
    }

    if( message.timeout2 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: timeout2\n");
        return 0;
    }

    if( message.increment2 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: increment2\n");
        return 0;
    }

    if( message.offset3 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: offset3\n");
        return 0;
    }

    if( message.slots3 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: slots3\n");
        return 0;
    }

    if( message.timeout3 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: timeout3\n");
        return 0;
    }

    if( message.increment3 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: increment3\n");
        return 0;
    }

    if( message.offset4 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: offset4\n");
        return 0;
    }

    if( message.slots4 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: slots4\n");
        return 0;
    }

    if( message.timeout4 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: timeout4\n");
        return 0;
    }

    if( message.increment4 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: increment4\n");
        return 0;
    }

    if( message.spare2 != 0 )
    {
        fprintf( stderr, "test_ais_20() failed: spare2\n");
        return 0;
    }

    fprintf( stderr, "test_ais_20() passed\n");
    return 1;
}


int test_ais_21( void )
{

    fprintf( stderr, "test_ais_21() failed: Not Written\n");
    return 0;

}


int test_ais_22( void )
{

    fprintf( stderr, "test_ais_22() failed: Not Written\n");
    return 0;

}


int test_ais_23( void )
{

    fprintf( stderr, "test_ais_23() failed: Not Written\n");
    return 0;

}


int test_ais_24A( void )
{
    ais_state state;
    aismsg_24  message;
    unsigned int  result;
    /* 24A !AIVDM,1,1,,A,H52IRsP518Tj0l59D0000000000,2*45 */
    char s[] = "H52IRsP518Tj0l59D0000000000";

    /* Clear out the structure first */
    memset( &message, 0, sizeof( aismsg_24 ));

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_24( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_24A() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 24 )
    {
        fprintf( stderr, "test_ais_24A() failed: msgid\n");
        return 0;
    }

    if( message.flags != 1 )
    {
        fprintf( stderr, "test_ais_24A() failed: flags\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_24A() failed: repeat\n");
        return 0;
    }

    if( message.userid != 338060014 )
    {
        fprintf( stderr, "test_ais_24A() failed: userid\n");
        return 0;
    }

    if( message.part_number != 0 )
    {
        fprintf( stderr, "test_ais_24A() failed: part_number\n");
        return 0;
    }

    if( strcmp( message.name, "APRIL MARU@@@@@@@@@@")  != 0 )
    {
        fprintf( stderr, "test_ais_24A() failed: name\n");
        return 0;
    }

    fprintf( stderr, "test_ais_24A() passed\n");
    return 1;
}


int test_ais_24B( void )
{
    ais_state state;
    aismsg_24  message;
    unsigned int  result;
    /* 24B !AIVDM,1,1,,A,H52IRsTU000000000000000@5120,0*76 */
    char s[] = "H52IRsTU000000000000000@5120";

    /* Clear out the structure first */
    memset( &message, 0, sizeof( aismsg_24 ));

    init_6bit( &state.six_state );
    strcpy( state.six_state.bits, s );
    state.msgid = (char) get_6bit( &state.six_state, 6 );
    if( (result = parse_ais_24( &state, &message )) != 0 )
    {
        fprintf( stderr, "test_ais_24B() failed: error %d\n", result);
        return 0;
    }

    /* Check the results */
    if( message.msgid != 24 )
    {
        fprintf( stderr, "test_ais_24B() failed: msgid\n");
        return 0;
    }

    if( message.flags != 2 )
    {
        fprintf( stderr, "test_ais_24B() failed: flags\n");
        return 0;
    }

    if( message.repeat != 0 )
    {
        fprintf( stderr, "test_ais_24B() failed: repeat\n");
        return 0;
    }

    if( message.userid != 338060014 )
    {
        fprintf( stderr, "test_ais_24B() failed: userid\n");
        return 0;
    }

    if( message.part_number != 1 )
    {
        fprintf( stderr, "test_ais_24B() failed: part_number\n");
        return 0;
    }

    if( message.ship_type != 37 )
    {
        fprintf( stderr, "test_ais_24B() failed: ship_type\n");
        return 0;
    }

    if( strcmp( message.vendor_id, "@@@@@@@")  != 0 )
    {
        fprintf( stderr, "test_ais_24B() failed: vendor_id\n");
        return 0;
    }

    if( strcmp( message.callsign, "@@@@@@@")  != 0 )
    {
        fprintf( stderr, "test_ais_24B() failed: callsign\n");
        return 0;
    }

    if( message.dim_bow != 2 )
    {
        fprintf( stderr, "test_ais_24B() failed: dim_bow\n");
        return 0;
    }

    if( message.dim_stern != 5 )
    {
        fprintf( stderr, "test_ais_24B() failed: dim_stern\n");
        return 0;
    }

    if( message.dim_port != 1 )
    {
        fprintf( stderr, "test_ais_24B() failed: dim_port\n");
        return 0;
    }

    if( message.dim_starboard != 2 )
    {
        fprintf( stderr, "test_ais_24B() failed: dim_starboard\n");
        return 0;
    }

    if( message.spare != 0 )
    {
        fprintf( stderr, "test_ais_24B() failed: spare\n");
        return 0;
    }

    fprintf( stderr, "test_ais_24B() passed\n");
    return 1;
}

int test_ais_27( void )
{
   ais_state state;
   aismsg_27  message;
   unsigned int  result;
   /* 27 !AIVDM,1,1,,B,K3M@PpqK>Qkv=PEp,0*60 */
   char s[] = "K3M@PpqK>Qkv=PEp";

   /* Clear out the structure first */
   memset( &message, 0, sizeof( aismsg_27 ));

   init_6bit( &state.six_state );
   strcpy( state.six_state.bits, s );
   state.msgid = (char) get_6bit( &state.six_state, 6 );
   if( (result = parse_ais_27( &state, &message )) != 0 )
   {
       fprintf( stderr, "test_ais_27() failed: error %d\n", result);
       return 0;
   }

   /* Check the results */
   if( message.msgid != 27 )
   {
       fprintf( stderr, "test_ais_27() failed: msgid\n");
       return 0;
   }

   if( message.repeat != 0 )
   {
       fprintf( stderr, "test_ais_27() failed: repeat\n");
       return 0;
   }

   if( message.userid != 232005859 )
   {
       fprintf( stderr, "test_ais_27() failed: MMSI/userid\n");
       return 0;
   }

   if( message.pos_acc != 1 )
   {
       fprintf( stderr, "test_ais_27() failed: pos_acc\n");
       return 0;
   }

   if( message.raim != 0 )
   {
       fprintf( stderr, "test_ais_27() failed: raim\n");
       return 0;
   }

   if( message.nav_status != 5 )
   {
       fprintf( stderr, "test_ais_27() failed: nav_status\n");
       return 0;
   }

   if( message.longitude != -78201000 )
   {
       fprintf( stderr, "test_ais_27() failed: longituden");
       return 0;
   }

   if( message.latitude != 32539000 )
   {
       fprintf( stderr, "test_ais_27() failed: latitude\n");
       return 0;
   }

   if( message.sog != 0 )
   {
       fprintf( stderr, "test_ais_27() failed: sog\n");
       return 0;
   }

   if( message.cog != 350 )
   {
       fprintf( stderr, "test_ais_27() failed: cog\n");
       return 0;
   }

   if( message.gnss != 0 )
   {
       fprintf( stderr, "test_ais_27() failed: gnss\n");
       return 0;
    }
 
    if( message.spare != 0 )
    {
       fprintf( stderr, "test_ais_27() failed: spare\n");
       return 0;
    }
 
   fprintf( stderr, "test_ais_27() passed\n");
   return 1;
 }
