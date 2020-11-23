/* -----------------------------------------------------------------------
   Header file for NMEA string parser
   Copyright 2006-2020 by Brian C. Lane <bcl@brianlane.com>
   All Rights Reserved
   ----------------------------------------------------------------------- */
/*! \file
    \brief Header file for nmea.c
    \author Brian C. Lane <bcl@brianlane.com>

    Definitions and structures for the NMEA sentence parser

*/

#define MAX_NMEA_LENGTH  255
#define MAX_NMEA_FIELDS  50
#define START  0
#define END    1
#define DONE   2


/** NMEA parser state structure
*/
typedef struct {
    unsigned char  search;                     //!< State of the search: START, END or DONE
    char           *field[MAX_NMEA_FIELDS];    //!< Pointers to fields in the buffer
    char           str[MAX_NMEA_LENGTH];       //!< Incoming NMEA 0183 string
    unsigned long  str_len;                    //!< Number of bytes in str
} nmea_state;


/* Prototypes */
unsigned int __stdcall check_nmea_checksum( char *buffer, unsigned char *checksum );
unsigned int __stdcall nmea_checksum( char *buffer, unsigned char *checksum );
char * __stdcall find_nmea_start( char *buffer );
char __stdcall ahextobin( char *c );
char * __stdcall nmea_next_field( char *p );
unsigned int __stdcall nmea_uint( char *p );
char * __stdcall nmea_copy_field( char *dest, char *src, int len );
