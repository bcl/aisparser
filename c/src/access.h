/* -----------------------------------------------------------------------
   AIS Parser SDK
   Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com>
   All rights Reserved
   ----------------------------------------------------------------------- */


/*! \file
    \brief Header file for access.h

	Prototypes for access.c
	
*/
sixbit * __stdcall get_msg6_data( aismsg_6 *msg );
sixbit * __stdcall get_msg8_data( aismsg_8 *msg );
sixbit * __stdcall get_msg17_data( aismsg_17 *msg );
weather_report * __stdcall get_weather_report( seaway1_1 *msg, int idx );
timetag * __stdcall get_weather_utc_time( weather_report *msg );
wind_report * __stdcall get_wind_report( seaway1_2 *msg, int idx );
timetag * __stdcall get_wind_utc_time( wind_report *msg );
water_level_report * __stdcall get_water_level_report( seaway1_3 *msg, int idx );
timetag * __stdcall get_water_level_utc_time( water_level_report *msg );
water_flow_report * __stdcall get_water_flow_report( seaway1_6 *msg, int idx );
timetag * __stdcall get_water_flow_utc_time( water_flow_report *msg );
lock_schedule * __stdcall get_lock_schedule( seaway2_1 *msg, int idx );
timetag * __stdcall get_lock_schedule_utc_time( seaway2_1 *msg );
timetag * __stdcall get_lock_schedule_eta( lock_schedule *msg );
timetag * __stdcall get_lock_times_utc_time( seaway2_2 *msg );
timetag * __stdcall get_lock_times_last_ata( seaway2_2 *msg );
timetag * __stdcall get_lock_times_first_eta( seaway2_2 *msg );
timetag * __stdcall get_lock_times_second_eta( seaway2_2 *msg );
current_report * __stdcall get_current_report( pawss1_4 *msg, int idx );
timetag * __stdcall get_current_utc_time( current_report *msg );
salinity_report * __stdcall get_salinity_report( pawss1_5 *msg, int idx );
timetag * __stdcall get_salinity_utc_time( salinity_report *msg );
procession_order * __stdcall get_procession_order( pawss2_3 *msg, int idx );
timetag * __stdcall get_procession_order_utc_time( pawss2_3 *msg );
timetag * __stdcall get_imo1_12_atd( imo1_12 *msg );
timetag * __stdcall get_imo1_12_eta( imo1_12 *msg );
tidal_window * __stdcall get_tidal_window( imo1_14 *msg, int idx );
pseudo_target * __stdcall get_pseudo_target( imo1_17 *msg, int idx );
