package com.aisparser;
/**
 * AIS Parser SDK
 * AIS sotdma Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Sotdma class
 * 
 */
public class Sotdma {
	int          sync_state;      //!< 2 bits   : SOTDMA Sync State
	int          slot_timeout;    //!< 3 bits   : SOTDMA Slot Timeout
	int          sub_message;     //!< 14 bits  : SOTDMA Sub-Messsage

	public int sync_state() { return this.sync_state; }
	public int slot_timeout() { return this.slot_timeout; }
	public int sub_message() { return this.sub_message; }
	
	public Sotdma()
	{
	}

	public void parse( Sixbit six_state )
	throws SixbitsExhaustedException, AISMessageException
	{
		if (six_state.bit_length() < 19)
			throw new AISMessageException("SOTDMA wrong length");
	
        this.sync_state   = (char)  six_state.get( 2  );
        this.slot_timeout = (char)  six_state.get( 3  );
        this.sub_message  = (int)   six_state.get( 14 );
	}
}
