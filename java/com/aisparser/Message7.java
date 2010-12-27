package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 7 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 7 class
 * Binary Acknowledgement
 * 
 */
public class Message7 extends Messages {
    int    spare;             // 2 bits   : Spare
    long   destid_1;          // 30 bits  : Destination MMSI 1
    int    sequence_1;        // 2 bits   : Sequence Number 1
    long   destid_2;          // 30 bits  : Destination MMSI 2
    int    sequence_2;        // 2 bits   : Sequence Number 2
    long   destid_3;          // 30 bits  : Destination MMSI 3
    int    sequence_3;        // 2 bits   : Sequence Number 3
    long   destid_4;          // 30 bits  : Destination MMSI 4
    int    sequence_4;        // 2 bits   : Sequence Number 4
    int    num_acks;          // Number of acks 

    public int spare() { return this.spare; }
    public long destid_1() { return this.destid_1; }
    public int sequence_1() { return this.sequence_1; }
    public long destid_2() { return this.destid_2; }
    public int sequence_2() { return this.sequence_2; }
    public long destid_3() { return this.destid_3; }
    public int sequence_3() { return this.sequence_3; }
    public long destid_4() { return this.destid_4; }
    public int sequence_4() { return this.sequence_4; }
    public int num_acks() { return this.num_acks; }
    
	public Message7()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		int length = six_state.bit_length();
		if ((length < 72) || (length > 168))
			throw new AISMessageException("Message 7 wrong length");
		
		super.parse( 7, six_state );

	    this.spare        = (int)   six_state.get( 2  );
	    this.destid_1     = (long)  six_state.get( 30 );
	    this.sequence_1   = (int)   six_state.get( 2  );
	    this.num_acks = 1;

	    if( length > 72 )
	    {
	        this.destid_2     = (long)  six_state.get( 30 );
	        this.sequence_2   = (int)   six_state.get( 2  );
	        this.num_acks++;
	    }
	    if( length > 104 )
	    {
	        this.destid_3     = (long)  six_state.get( 30 );
	        this.sequence_3   = (int)   six_state.get( 2  );
	        this.num_acks++;
	    }
	    if( length > 136 )
	    {
	        this.destid_4     = (long)  six_state.get( 30 );
	        this.sequence_4   = (int)   six_state.get( 2  );
	        this.num_acks++;
	    }
	}
}
