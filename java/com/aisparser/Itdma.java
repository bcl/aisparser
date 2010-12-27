package com.aisparser;

/**
 * AIS Parser SDK
 * AIS itdma Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Itdma class
 * 
 */
public class Itdma {
    int           sync_state;      // 2 bits   : ITDMA Sync State
    int           slot_inc;        // 13 bits  : ITDMA Slot Increment
    int           num_slots;       // 3 bits   : ITDMA Number of Slots
    int           keep_flag;       // 1 bit    : ITDMA Keep Flag

    public int sync_state() { return this.sync_state; }
    public int slot_inc() { return this.slot_inc; }
    public int num_slots() { return this.num_slots; }
    public int keep_flag() { return this.keep_flag; }
    
	public Itdma()
	{
	}

	public void parse( Sixbit six_state )
	throws SixbitsExhaustedException, AISMessageException
	{
		if (six_state.bit_length() < 19)
			throw new AISMessageException("ITDMA wrong length");

        this.sync_state    = (char)  six_state.get( 2  );
        this.slot_inc      = (int)   six_state.get( 13 );
        this.num_slots     = (char)  six_state.get( 3  );
        this.keep_flag     = (char)  six_state.get( 1  );
	}
}
