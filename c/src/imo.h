/* -----------------------------------------------------------------------
   IMO Binary Message Data Header
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
   All rights Reserved
   ----------------------------------------------------------------------- */

/*! \file
    \brief Header file for imo.c

    Structures for each IMO binary message type

    For exact definitions of each field you should see the IMO SN/Circ. 236
    28 May 2004

    The format of the !AIVDM strings are covered in IEC 61993-2

*/


/** Application 1 - Meteorological and Hydrological data
*/
typedef struct {
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
    unsigned int    timedate;          //!< 16 bits  : Date and Time DDHHMM
    char            wind_avg;          //!< 7 bits   : Average of wind speed for the last 10 minutes 0-120kts
    char            wind_gust;         //!< 7 bits   : Maximum wind speed during the last 10 minutes 0-120kt
    int             wind_dir;          //!< 9 bits   : 0-359 degrees
    int             gust_dir;          //!< 9 bits   : 0-359 degrees
    int             air_temp;          //!< 11 bits  : Dry bulb temp. -60.0 to +60.0 C
    char            humidity;          //!< 7 bits   : 0-100%
    int             dew_point;         //!< 10 bits  : -20.0 to +50.0 degrees
    int             pressure;          //!< 9 bits   : 800-1200 hPa
    char            tendency;          //!< 2 bits   : 0=steady 1=decreasing 2=increasing
    int             visibility;        //!< 8 bits   : 0.0 to 25.0 NM
    int             water_level;       //!< 9 bits   : -10.0 to +30.0m
    char            water_trend;       //!< 2 bits   : 0=steady 1=decreasing 2=increasing
    char            surface_speed;     //!< 8 bits   : 0.0 to 25.0 kts
    int             surface_dir;       //!< 9 bits   : 0-359 degrees
    char            speed_2;           //!< 8 bits   : Current measured at chosen level 0.0 to 25.0 kts
    int             dir_2;             //!< 9 bits   : Current direction 0-359 degrees
    char            level_2;           //!< 5 bits   : Measuring level 0-30m
    char            speed_3;           //!< 8 bits   : Current measured at chosen level 0.0 to 25.0 kts
    int             dir_3;             //!< 9 bits   : Current direction 0-359 degrees
	char            level_3;           //!< 5 bits   : Measuring level 0-30m
    char            wave_height;       //!< 8 bits   : 0.0 to 25.0 m
    char            wave_period;       //!< 6 bits   : Period in second 0-60
	int				wave_dir;          //!< 9 bits   : 0-359 degrees
    char            swell_height;      //!< 8 bits   : 0.0 to 25.0 m
    char            swell_period;      //!< 6 bits   : Period in second 0-60
    int             swell_dir;         //!< 9 bits   : 0-359 degrees
    char            sea_state;         //!< 4 bits   : Beaufort scale
    int             water_temp;        //!< 10 bits  : -10.0 to +50.0 degrees
    char            precip_type;       //!< 3 bits   : According to WMO
    int             salanity;          //!< 9 bits   : 0.0 to 50.0%o
    char            ice;               //!< 2 bits   : yes/no
    char            spare;             //!< 6 bits   : spare
} imo1_11;


/** Application 2 - Dangerous Cargo Indication
*/
typedef struct {
    char            last_port[6];      //!< 5x6 (30) bits : Last Port of Call UN Locode
    timetag			atd;               //!< 20 bits  : Actual Time of Departure
    char            next_port[6];      //!< 5x6 (30) bits : Last Port of Call UN Locode
    timetag			eta;               //!< 20 bits  : Estimated Time of Arrival
    char            good[21];          //!< 20x6 (120) bits : Main Dangerous Good
    char			imd[5];            //!< 4x6 (24) bits : IMD Category of Main Dangerous Good
    int             un_number;         //!< 13 bits  : UN Number 1-3363  0=not available
    int             quantity;          //!< 10 bits  : Quantity of Main Dangerous Good 
    char            units;             //!< 2 bits   : 0=not avail. 1=kg 2=tons (1E3kg) 3=1000tons(1E6kg)
    char            spare;             //!< 3 bits   : spare
} imo1_12;



/** Application 3 - Fairway Closed
*/
typedef struct {
    char            reason[21];        //!< 20x6 (120) bits : Reason for closing
    char            location_from[21]; //!< 20x6 (120) bits : Location of closing from
    char            location_to[21];   //!< 20x6 (120) bits : Location of closing to
    int             extension;         //!< 10 bits : Extension of closed area radius (0-1000 1001=not available)
    char            units;             //!< 2 bits  : 0=m 1=km 2=nm 3=cbl
    char            from_day;          //!< 5 bits  : 1-31 0=not available
    char            from_month;        //!< 4 bits  : 1-12 0=not available
    char            from_hour;         //!< 5 bits  : 0-23 24=not available
    char            from_minute;       //!< 6 bits  : 0-59 60=not available
    char            to_day;            //!< 5 bits  : 1-31 0=not available
    char            to_month;          //!< 4 bits  : 1-12 0=not available
    char            to_hour;           //!< 5 bits  : 0-23 24=not available
    char            to_minute;         //!< 6 bits  : 0-59 60=not available
    char            spare;             //!< 4 bits  : spare
} imo1_13;


/** Struct for imo1_14 Tidal Window information
*/
typedef struct {
    long            latitude;         //!< 27 bits  : Latitude in 1/10000 minute
    long            longitude;        //!< 28 bits  : Longitude in 1/10000 minute
    char            from_hour;        //!< 5 bits   : 0-23 24=not available
    char            from_minute;      //!< 6 bits   : 0-59 60=not available
    char            to_hour;          //!< 5 bits   : 0-23 24=not available
    char            to_minute;        //!< 6 bits   : 0-59 60=not available
    int             current_dir;      //!< 9 bits   : Current direction in degrees 0-359 360=not available
    char            current_speed;    //!< 7 bits   : Current speed 0-12.6 kts 127=not available
} tidal_window;


/** Application 4 - Tidal Window
*/
typedef struct {
    char            utc_month;         //!< 4 bits  : 1-12 0=not available
    char            utc_day;           //!< 5 bits  : 1-31 0=not available
    tidal_window    windows[3];
} imo1_14;


/** Application 5 - Extended Ship Static and Voyage Related Data
*/
typedef struct {
    int             ais_draught;       //!< 11 bits  : Height over Keel in 1/10m 0=not available
    char            spare;             //!< 5 bits   : spare
} imo1_15;


/** Application 6 - Number of Persons on Board
*/
typedef struct {
	int             num_persons;       //!< 13 bits  : Number of Persons on Board 0-8191 0=not available
	char            spare;             //!< 3 bits   : spare
} imo1_16;
	

/** Pseudo-AIS Targets used by imo1_17
*/
typedef struct {
    char            type;              //!< 2 bits   : 0=MMSI 1=IMO 2=Callsign 3=Other
    // 42 bits are allocated for MMSI, ISO or Callsign 
    // When MMSI and ISO are used, skip 12 bits first
    long            mmsi;              //!< 30 bits  : MMSI
    long            imo;               //!< 30 bits  : IMO
    char            callsign[8];       //!< 7x6 (42) bits : Callsign
    char            other[8];          //!< 7x6 (42) bits : Callsign
    char            spare;             //!< 4 bits  : spare
    long            latitude;          //!< 24 bits  : Latitude in 1/1000 minute
    long            longitude;         //!< 25 bits  : Longitude in 1/1000 minute
	int             cog;               //!< 9 bits   : Course over ground 0-359 degrees 360=not available
	char            timestamp;         //!< 6 bits   : UTC Second when generated 60=not available
	unsigned char   sog;               //!< 8 bits   : Speed over ground 0-254kts 255=not available		
}  pseudo_target;

/** Application 6 - Number of Persons on Board
*/
typedef struct {
	pseudo_target   targets[4];        //!< 1-4 targets
} imo1_17;


int __stdcall parse_imo1_11( sixbit *state, imo1_11 *result );
int __stdcall parse_imo1_12( sixbit *state, imo1_12 *result );
int __stdcall parse_imo1_13( sixbit *state, imo1_13 *result );
int __stdcall parse_imo1_14( sixbit *state, imo1_14 *result );
int __stdcall parse_imo1_15( sixbit *state, imo1_15 *result );
int __stdcall parse_imo1_16( sixbit *state, imo1_16 *result );
int __stdcall parse_imo1_17( sixbit *state, imo1_17 *result );

