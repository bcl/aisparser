package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 14 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 14 class
 * Safety Related Broadcast
 * 
 */
public class Message14 extends Messages {
    int            spare;            // 2 bits   : Spare
    String         message;          // 968 bits : Message in ASCII

    public int spare() { return this.spare; }
    public String message() { return this.message; }
    
	public Message14()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		int length = six_state.bit_length();
		if ((length < 40) || (length > 1008))
			throw new AISMessageException("Message 14 wrong length");
		
		super.parse( 14, six_state );

	    this.spare        = (int)   six_state.get( 2  );
	    this.message = six_state.get_string( (length-40)/6 );		
	}
}
