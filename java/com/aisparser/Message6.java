package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 6 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */


/**
 * AIS Message 6 class
 * Binary Addressed Message
 * 
 */
public class Message6 extends Messages {
    int            sequence;          // 2 bits   : Sequence number
    long           destination;       // 30 bits  : Destination MMSI
    int            retransmit;        // 1 bit    : Retransmit
    int            spare;             // 1 bit    : Spare
    int            app_id;            // 16 bits  : Application ID
    Sixbit         data;              // 960 bits : Data payload

    public int sequence() { return this.sequence; }
    public long destination() { return this.destination; }
    public int retransmit() { return this.retransmit; }
    public int spare() { return this.spare; }
    public int app_id() { return this.app_id; }
    public Sixbit data() { return this.data; }
    
	public Message6()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if ((six_state.bit_length() < 88) || (six_state.bit_length() > 1008))
			throw new AISMessageException("Message 6 wrong length");
		
		super.parse( 6, six_state );
		
	    this.sequence     = (int)   six_state.get( 2 );
	    this.destination  = (long)  six_state.get( 30 );
	    this.retransmit   = (int)   six_state.get( 1 );
	    this.spare        = (int)   six_state.get( 1 );
	    this.app_id       = (int)   six_state.get( 16 );

	    /* Store the remaining payload of the packet for further processing */
		this.data = six_state;

	}
}
