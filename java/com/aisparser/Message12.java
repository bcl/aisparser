package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 12 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 12 class
 * Addressed Safety Related Message
 * 
 */
public class Message12 extends Messages {
    int            sequence;          // 2 bits   : Sequence
    long           destination;       // 30 bits  : Destination MMSI
    int            retransmit;        // 1 bit    : Retransmit
    int            spare;             // 1 bit    : Spare
    String         message;           // 936 bits : Message in ASCII   
    
    public int sequence() { return this.sequence; }
    public long destination() { return this.destination; }
    public int retransmit() { return this.retransmit; }
    public int spare() { return this.spare; }
    public String message() { return this.message; }
    
	public Message12()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		int length = six_state.bit_length();
		if ((length < 72) || (length > 1008))
			throw new AISMessageException("Message 12 wrong length");
		
		super.parse( 12, six_state );
		
	    this.sequence     = (int)   six_state.get( 2  );
	    this.destination  = (long)  six_state.get( 30 );
	    this.retransmit   = (int)   six_state.get( 1  );
	    this.spare        = (int)   six_state.get( 1  );

	    this.message = six_state.get_string( (length-72)/6 );
	}
}
