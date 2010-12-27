/* -----------------------------------------------------------------------
   St. Lawrence Seaway Message Data Header
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
   All rights Reserved
   ----------------------------------------------------------------------- */

/*! \file
    \brief Header file for seaway.c

    Structures for each St. Lawrence Seaway message type

    For exact definitions of each field you should see the ITU M.1371,
    IEC 62287 and the IALA Clarifications on M.1371

    The format of the !AIVDM strings are covered in IEC 61993-2

    St. Lawrence Seaway messages are documented in the document titled -
    "St. Lawrence Seaway AIS Data Messaging Formats and Specifications"
    available from http://www.greatlakes-seaway.com/en/pdf/aisdata.pdf

*/


/** Seaway Wind Information Report
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time tag
    char            station_id[8];     //!< 7x6(42) bits  : Station ID
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    int             speed;             //!< 10 bits  : Average Wind Speed
    int             gust;              //!< 10 bits  : Wind Gust
    int             direction;         //!< 9 bits   : Wind Direction
    char            spare;             //!< 4 bits   : Reserved for Future Use
} wind_report;


/** Seaway Weather Station Report
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time tag
    char            station_id[8];     //!< 7x6(42) bits  : Station ID
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    int             speed;             //!< 10 bits  : Average Wind Speed
    int             gust;              //!< 10 bits  : Wind Gust
    int             direction;         //!< 9 bits   : Wind Direction
    int             pressure;          //!< 14 bits  : Atmospheric Pressure
    int             air_temp;          //!< 10 bits  : Air Temperature
    int             dew_point;         //!< 10 bits  : Dew Point
    unsigned char   visibility;        //!< 8 bits   : Visibility
    int             water_temp;        //!< 10 bits  : Water Temperature
} weather_report;


/** Seaway Water Level Report
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time tag
    char            station_id[8];     //!< 7x6(42) bits  : Station ID
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    char            type;              //!< 1 bit    : Water Level Type
    int			    level;             //!< 16 bits  : Water Level
    char            datum;             //!< 2 bits   : Reference Datum
    int             spare;             //!< 14 bits  : Reserved for Future Use
} water_level_report;


/** Seaway Water Flow Report
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time tag
    char            station_id[8];     //!< 7x6(42) bits  : Station ID
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    int             flow;              //!< 14 bits  : Water Flow
    long            spare;             //!< 19 bits  : Reserved for Future Use
} water_flow_report;


/** Seaway Lock Schedule Report
*/
typedef struct {
    char            name[16];          //!< 15x6(90) bits  : Vessel Name
    char            direction;         //!< 1 bit    : Direction
    timetag         eta;               //!< 20 bits  : ETA
    int             spare;             //!< 9 bits   : Reserved for Future Use
} lock_schedule;


/** PAWSS Hydro/Current Report
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time Tag
    char            station_id[8];     //!< 7x6(42) bits  : Station ID
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    unsigned char   speed;             //!< 8 bits   : Current Speed
    int             direction;         //!< 9 bits   : Current Direction
    unsigned int    spare;             //!< 16 bits  : Reserved for Future Use
} current_report;


/** PAWSS Hydro/Salinity Temp Report
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time Tag
    char            station_id[8];     //!< 7x6(42) bits  : Station ID
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    int             salinity;          //!< 10 bits  : Salinity
    int             water_temp;        //!< 10 bits  : Water Temp
    unsigned int    spare;             //!< 13 bits  : Reserved for Future Use
} salinity_report;


/** Vessel Procession Order Report
*/
typedef struct {
    char            order;             //!< 5 bits   : Order in list 1-31
    char            vessel_name[16];   //!< 15*6(90) bits : Vessel Name
    char            position_name[13]; //!< 13*6(72) bits : Position Name
    char            time_hh;           //!< 5 bits   : Hours of call in
    char            time_mm;           //!< 6 bits   : Minutes of call in
    char            spare;             //!< 6 bits   : Reserved for Future Use
} procession_order;


/** Seaway Message 1.1 - Weather Station Message
*/
typedef struct {
    weather_report  report[4];         //!< N * 192  : 1-4 Weather Reports
} seaway1_1;


/** Seaway Message 1.2 - Wind Information Message
*/
typedef struct {
    wind_report     report[6];         //!< N * 144  : 1-6 Wind Reports
} seaway1_2;


/** Seaway Message 1.3 - Water Level Message
*/
typedef struct {
    water_level_report report[6];      //!< N * 144  : 1-6 Water Level Reports
} seaway1_3;


/** Seaway Message 1.6 - Water Flow Message
*/
typedef struct {
    water_flow_report report[6];       //!< N * 144  : 1-6 Water Flow Reports
} seaway1_6;


/** Seaway Message 2.1 - Lockage Order Message
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time tag
    char            lock_id[8];        //!< 7x6(42) bits  : Lock ID
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    int             spare2;            //!< 9 bits   : Reserved for Future Use
    lock_schedule   schedule[6];       //!< N * 120  : 1-6 Lock Schedules
} seaway2_1;


/** Seaway Message 2.2 - Estimated Lock Times Message
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time tag
    char            name[16];          //!< 15x6(90) bits : Vessel Name
    char            last_location[8];  //!< 7x6(42) bits  : Last Location
    timetag         last_ata;          //!< 20 bits  : Last ATA in UTC Time
    char            first_lock[8];     //!< 7x6(42) bits  : First Lock
    timetag         first_eta;         //!< 20 bits  : First Lock ETA in UTC
    char            second_lock[8];    //!< 7x6(42) bits  : Second Lock
    timetag         second_eta;        //!< 20 bits  : Second Lock ETA in UTC
    char            delay[8];          //!< 7x6(42) bits  : Delay
    char            spare2;            //!< 4 bits   : Reserved for Future Use
} seaway2_2;


/** Seaway Message 32.1 - Version Message
*/
typedef struct {
    unsigned char   major;             //!< 8 bits   : Major revision number
    unsigned char   minor;             //!< 8 bits   : Minor revision number
    unsigned char   spare2;            //!< 8 bits   : Reserved for Future Use
} seaway32_1;


/** PAWSS Message 1.4 - Hydro/Current Message
*/
typedef struct {
    current_report  report[6];         //!< N * 144  : 1-6 Hydro Reports
} pawss1_4;


/** PAWSS Message 1.5 - Hydro/Salinity Temp Message
*/
typedef struct {
    salinity_report report[6];         //!< N * 144  : 1-6 Hydro Reports
} pawss1_5;


/** PAWSS Message ?.3 - Vessel Procession Order Message

    The DAC is not fully described in the spec. Is it 2.3?
*/
typedef struct {
    timetag         utc_time;          //!< 20 bits  : UTC Time tag
    char            direction[17];     //!< 16*6(96) bits : Direction
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    char            spare2;            //!< 3 bits   : Reserved for Future Use
    procession_order  report[4];       //!< N * 184  : 1-4 Vessel Procession Order Reports    
} pawss2_3;


/* Prototypes */
int __stdcall parse_seaway1_1( sixbit *state, seaway1_1 *result );
int __stdcall parse_seaway1_2( sixbit *state, seaway1_2 *result );
int __stdcall parse_seaway1_3( sixbit *state, seaway1_3 *result );
int __stdcall parse_seaway1_6( sixbit *state, seaway1_6 *result );
int __stdcall parse_seaway2_1( sixbit *state, seaway2_1 *result );
int __stdcall parse_seaway2_2( sixbit *state, seaway2_2 *result );
int __stdcall parse_seaway32_1( sixbit *state, seaway32_1 *result );
int __stdcall parse_pawss1_4( sixbit *state, pawss1_4 *result );
int __stdcall parse_pawss1_5( sixbit *state, pawss1_5 *result );
int __stdcall parse_pawss2_3( sixbit *state, pawss2_3 *result );
