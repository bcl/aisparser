/* -----------------------------------------------------------------------
   AIS Parser SDK
   Visual Basic Interface
   Copyright 2006 by Brian C. Lane

   ----------------------------------------------------------------------- */
#include <windows.h>

#include "nmea.h"
#include "sixbit.h"
#include "vdm_parse.h"
#include "seaway.h"

/*! \file
    \brief Visual Baisc DLL Wrapper functions

    Functions to convert native data types to Visual Basic compatible
	types for easy interfacing to Visual Basic
*/


//Main DLL function
//This is a window's DLL's equivalent to WinMain()
HINSTANCE		hInst ;					//dll's instance handle

BOOL WINAPI DllMain(HINSTANCE hInstA, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		// The DLL is being mapped into the process's address space
		hInst = hInstA;
		
		// Initialize here if necessary*******
		//************************************
	   	//
		//
		break;

	case DLL_THREAD_ATTACH:
		// A thread is being created
	    break;

	case DLL_THREAD_DETACH:
		// A thread is exiting cleanly
	    break;

	case DLL_PROCESS_DETACH:
		// The DLL is being unmapped from the process's address space
		// Cleanup here if necessary*************
		//***************************************
		hInst = 0;
	    break;
	}

	return TRUE;
}



/* ----------------------------------------------------------------------- */
/** Get a new ais_state structure

    Allocate memory for the ais_state structure and return it to VB to
	use as an Object to pass to other functions
	
	\retval pointer to ais_state memory
	
	NOTE: Don't forget to free this memory!
*/
/* ----------------------------------------------------------------------- */
ais_state * __stdcall InitAisparser()
{
	ais_state *ptr;

	ptr = malloc(sizeof(ais_state));
	memset(ptr, 0, sizeof(ais_state));

	return ptr;
}


/* ----------------------------------------------------------------------- */
/** Clean up AIS Parser memory

	\param state Free the ais_state memory

    Free up the memory allocated for ais_state
*/
/* ----------------------------------------------------------------------- */
void __stdcall CloseAisparser( ais_state *state )
{
    if( state )
		free( state );
}



/* ----------------------------------------------------------------------- *
/** Assemble VDM Messages

    Assemble single or multi-part VDM messages into a single string.
*/
/* ----------------------------------------------------------------------- */
int __stdcall AssembleVdm( ais_state *state, LPSTR str)
{
    return assemble_vdm( state, (char *) str );
}


/* ----------------------------------------------------------------------- *
/** Get a value from the 6-bit stream

    Assemble single or multi-part VDM messages into a single string.
*/
/* ----------------------------------------------------------------------- */
unsigned long __stdcall Get6Bit( ais_state *state, short numbits)
{

    // !!!! does this work for 32 bit fetches? !!!!
	return get_6bit( &state->six_state, numbits );
}
