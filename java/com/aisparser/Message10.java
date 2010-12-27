package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 10 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 10 class
 * UTC/date inquiry
 */
public class Message10 extends Messages {
    int            spare1;            // 2 bits   : Spare
    long           destination;       // 30 bits  : Destination MMSI
    int            spare2;            // 2 bits   : Spare

    public int spare1() { return this.spare1; }
    public long destination() { return this.destination; }
    public int spare2() { return this.spare2; }

	public Message10()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if (six_state.bit_length() != 72)
			throw new AISMessageException("Message 10 wrong length");
		
		super.parse( 10, six_state );
        
	    this.spare1     = (int)   six_state.get( 2  );
	    this.destination= (long)  six_state.get( 30 );
	    this.spare2     = (int)   six_state.get( 2  );
	}
}
