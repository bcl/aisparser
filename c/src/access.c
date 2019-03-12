/* -----------------------------------------------------------------------
   AIS Parser SDK
   Copyright 2006-20078 by Brian C. Lane <bcl@brianlane.com>
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
#include "imo.h"

/*! \file
    \brief Header file for access.c

    This file contains functions to make accessing binary messages from
	Visual Basic 6 and Python easier to handle.

	Examples:


*/



/* ----------------------------------------------------------------------- */
/** Get the data struct from a message 6

    \param msg pointer to a aismsg_6 struct

	\retval pointer to a sixbit struct
*/
/* ----------------------------------------------------------------------- */
sixbit * __stdcall get_msg6_data( aismsg_6 *msg )
{
	return &msg->data;
}


/* ----------------------------------------------------------------------- */
/** Get the data struct from a message 8

    \param msg pointer to a aismsg_8 struct

	\retval pointer to a sixbit struct
*/
/* ----------------------------------------------------------------------- */
sixbit * __stdcall get_msg8_data( aismsg_8 *msg )
{
	return &msg->data;
}


/* ----------------------------------------------------------------------- */
/** Get the data struct from a message 17

    \param msg pointer to a aismsg_17 struct

	\retval pointer to a sixbit struct
*/
/* ----------------------------------------------------------------------- */
sixbit * __stdcall get_msg17_data( aismsg_17 *msg )
{
	return &msg->data;
}


/* ----------------------------------------------------------------------- */
/** Get a weather report from seaway1_1 message

    \param msg pointer to a seaway1_1 message
	\param idx Index of report to retrieve (0-3)

	\retval pointer to a weather_report structure
*/
/* ----------------------------------------------------------------------- */
weather_report * __stdcall get_weather_report( seaway1_1 *msg, int idx )
{
	if (idx > 3)
		return NULL;
	return &msg->report[idx];
}


/* ----------------------------------------------------------------------- */
/** Get the weather report timetag from a weather_report structure

    \param msg pointer to a weather_report structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_weather_utc_time( weather_report *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get a wind report from seaway1_2 message

    \param msg pointer to a seaway1_2 message
	\param idx Index of report to retrieve (0-5)

	\retval pointer to a wind_report structure
*/
/* ----------------------------------------------------------------------- */
wind_report * __stdcall get_wind_report( seaway1_2 *msg, int idx )
{
	if (idx > 5)
		return NULL;
	return &msg->report[idx];
}


/* ----------------------------------------------------------------------- */
/** Get the wind report timetag from a wind_report structure

    \param msg pointer to a wind_report structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_wind_utc_time( wind_report *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get a water level report from seaway1_3 message

    \param msg pointer to a seaway1_3 message
	\param idx Index of report to retrieve (0-5)

	\retval pointer to a water_level_report structure
*/
/* ----------------------------------------------------------------------- */
water_level_report * __stdcall get_water_level_report( seaway1_3 *msg, int idx )
{
	if (idx > 5)
		return NULL;
	return &msg->report[idx];
}


/* ----------------------------------------------------------------------- */
/** Get the water level report timetag from a water_level_report structure

    \param msg pointer to a water_level_report structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_water_level_utc_time( water_level_report *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get a water flow report from seaway1_6 message

    \param msg pointer to a seaway1_6 message
	\param idx Index of report to retrieve (0-5)

	\retval pointer to a water_flow_report structure
*/
/* ----------------------------------------------------------------------- */
water_flow_report * __stdcall get_water_flow_report( seaway1_6 *msg, int idx )
{
	if (idx > 5)
		return NULL;
	return &msg->report[idx];
}


/* ----------------------------------------------------------------------- */
/** Get the water flow report timetag from a water_flow_report structure

    \param msg pointer to a water_flow_report structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_water_flow_utc_time( water_flow_report *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get a lock schedule from seaway2_1 message

    \param msg pointer to a seaway2_1 message
	\param idx Index of report to retrieve (0-5)

	\retval pointer to a lock schedule structure
*/
/* ----------------------------------------------------------------------- */
lock_schedule * __stdcall get_lock_schedule( seaway2_1 *msg, int idx )
{
	if (idx > 5)
		return NULL;
	return &msg->schedule[idx];
}


/* ----------------------------------------------------------------------- */
/** Get the lock utc timetag from a seaway2_1 message

    \param msg pointer to a seaway2_1 structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_lock_schedule_utc_time( seaway2_1 *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get the lock eta timetag from a lock_schedule structure

    \param msg pointer to a lock_schedule structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_lock_schedule_eta( lock_schedule *msg )
{
	return &msg->eta;
}


/* ----------------------------------------------------------------------- */
/** Get the lock utc timetag from a seaway2_2 message

    \param msg pointer to a seaway2_2 structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_lock_times_utc_time( seaway2_2 *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get the lock last_ata timetag from a seaway2_2 message

    \param msg pointer to a seaway2_2 structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_lock_times_last_ata( seaway2_2 *msg )
{
	return &msg->last_ata;
}


/* ----------------------------------------------------------------------- */
/** Get the lock first_eta timetag from a seaway2_2 message

    \param msg pointer to a seaway2_2 structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_lock_times_first_eta( seaway2_2 *msg )
{
	return &msg->first_eta;
}


/* ----------------------------------------------------------------------- */
/** Get the lock second_eta timetag from a seaway2_2 message

    \param msg pointer to a seaway2_2 structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_lock_times_second_eta( seaway2_2 *msg )
{
	return &msg->second_eta;
}


/* ----------------------------------------------------------------------- */
/** Get a current report from pawss1_4 message

    \param msg pointer to a pawss1_4 message
	\param idx Index of report to retrieve (0-5)

	\retval pointer to a current_report structure
*/
/* ----------------------------------------------------------------------- */
current_report * __stdcall get_current_report( pawss1_4 *msg, int idx )
{
	if (idx > 5)
		return NULL;
	return &msg->report[idx];
}


/* ----------------------------------------------------------------------- */
/** Get the current report timetag from a current_report structure

    \param msg pointer to a current_report structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_current_utc_time( current_report *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get a salinity report from pawss1_5 message

    \param msg pointer to a pawss1_5 message
	\param idx Index of report to retrieve (0-5)

	\retval pointer to a salinity_report structure
*/
/* ----------------------------------------------------------------------- */
salinity_report * __stdcall get_salinity_report( pawss1_5 *msg, int idx )
{
	if (idx > 5)
		return NULL;
	return &msg->report[idx];
}


/* ----------------------------------------------------------------------- */
/** Get the salinity report timetag from a salinity_report structure

    \param msg pointer to a salinity_report structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_salinity_utc_time( salinity_report *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get a procession order from pawss2_3 message

    \param msg pointer to a pawss2_3 message
	\param idx Index of report to retrieve (0-5)

	\retval pointer to a prcession_order structure
*/
/* ----------------------------------------------------------------------- */
procession_order * __stdcall get_procession_order( pawss2_3 *msg, int idx )
{
	if (idx > 5)
		return NULL;
	return &msg->report[idx];
}


/* ----------------------------------------------------------------------- */
/** Get the procession order timetag from a pawss2_3 message

    \param msg pointer to a pawss2_3 structure

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_procession_order_utc_time( pawss2_3 *msg )
{
	return &msg->utc_time;
}


/* ----------------------------------------------------------------------- */
/** Get the atd timetag from imo1_12 message

    \param msg pointer to a imo1_12 message

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_imo1_12_atd( imo1_12 *msg )
{
	return &msg->atd;
}


/* ----------------------------------------------------------------------- */
/** Get the eta timetag from imo1_12 message

    \param msg pointer to a imo1_12 message

	\retval pointer to a timetag structure
*/
/* ----------------------------------------------------------------------- */
timetag * __stdcall get_imo1_12_eta( imo1_12 *msg )
{
	return &msg->eta;
}


/* ----------------------------------------------------------------------- */
/** Get a tidal window from imo1_14 message

    \param msg pointer to a imo1_14 message
	\param idx Index of window to retrieve (0-2)

	\retval pointer to a pseudo-target structure
*/
/* ----------------------------------------------------------------------- */
tidal_window * __stdcall get_tidal_window( imo1_14 *msg, int idx )
{
	if (idx > 2)
		return NULL;
	return &msg->windows[idx];
}


/* ----------------------------------------------------------------------- */
/** Get a pseudo-target from imo1_17 message

    \param msg pointer to a imo1_17 message
	\param idx Index of target to retrieve (0-3)

	\retval pointer to a pseudo-target structure
*/
/* ----------------------------------------------------------------------- */
pseudo_target * __stdcall get_pseudo_target( imo1_17 *msg, int idx )
{
	if (idx > 3)
		return NULL;
	return &msg->targets[idx];
}


