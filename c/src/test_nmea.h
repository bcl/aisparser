/* -----------------------------------------------------------------------
   Header file for NMEA string parser
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved
   ----------------------------------------------------------------------- */
/*! \file
    \brief Header file for test_nmea.c

    Definitions and structures for the NMEA sentence parser

*/

int test_ahextobin( void );
int test_nmea_checksum( void );
int test_check_nmea_checksum( void );
int test_get_nmea_fields( void );
int test_parse_nmea( void );
int test_nmea_next_field( void );
int test_nmea_uint( void );
int test_nmea_copy_field( void );
int test_find_nmea_start( void );

