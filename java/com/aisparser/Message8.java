package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 8 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 8 class
 * Binary Broadcast Message
 * 
 */
public class Message8 extends Messages {
    int    spare;             // 2 bits   : Spare
    int    app_id;            // 16 bits  : Application ID
    Sixbit data;              // 952 bits : Data payload
    
    public int spare() { return this.spare; }
    public int app_id() { return this.app_id; }
    public Sixbit data() { return this.data; }
    
	public Message8()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		int length = six_state.bit_length();
		if ((length < 56) || (length > 1008))
			throw new AISMessageException("Message 8 wrong length");
		
		super.parse( 8, six_state );

	    this.spare        = (int)  six_state.get( 2 );
	    this.app_id       = (int)  six_state.get( 16 );

	    /* Store the remaining payload of the packet for further processing */
		this.data = six_state;
	}
}
