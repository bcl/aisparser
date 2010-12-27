package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 15 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 15 class
 * Interrogation
 * 
 */
public class Message15 extends Messages {
    int            spare1;            // 2 bits   : Spare
    long           destid1;           // 30 bits  : Destination MMSI 1
    int            msgid1_1;          // 6 bits   : MessageID 1.1
    int            offset1_1;         // 12 bits  : Slot Offset 1.1
    int            spare2;            // 2 bits   : Spare
    int            msgid1_2;          // 6 bits   : MessageID 1.2
    int            offset1_2;         // 12 bits  : Slot Offset 1.2
    int            spare3;            // 2 bits   : Spare
    long           destid2;           // 30 bits  : Destination MMSI 2
    int            msgid2_1;          // 6 bits   : MessageID 2.1
    int            offset2_1;         // 12 bits  : Slot Offset 2.1
    int            spare4;            // 2 bits   : Spare
    int            num_reqs;          // Number of interrogation requests     

    public int spare1() { return this.spare1; }
    public long destid1() { return this.destid1; }
    public int msgid1_1() { return this.msgid1_1; }
    public int offset1_1() { return this.offset1_1; }
    public int spare2() { return this.spare2; }
    public int msgid1_2() { return this.msgid1_2; }
    public int offset1_2() { return this.offset1_2; }
    public int spare3() { return this.spare3; }
    public long destid2() { return this.destid2; }
    public int msgid2_1() { return this.msgid2_1; }
    public int offset2_1() { return this.offset2_1; }
    public int spare4() { return this.spare4; }
    public int num_reqs() { return this.num_reqs; }
    
	public Message15()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		int length = six_state.bit_length();
		if ((length < 88) || (length > 162))
			throw new AISMessageException("Message 15 wrong length");
		
		super.parse( 15, six_state );
    
	    this.spare1       = (int)   six_state.get( 2  );
	    this.destid1      = (long)  six_state.get( 30 );
	    this.msgid1_1     = (int)   six_state.get( 6  );
	    this.offset1_1    = (int)   six_state.get( 12 );
	    this.num_reqs     = 1;
	    
	    if( length > 88 )
	    {
	        this.spare2    = (int)  six_state.get( 2 );
	        this.msgid1_2  = (int)  six_state.get( 6  );
	        this.offset1_2 = (int)  six_state.get( 12 );
	        this.num_reqs  = 2;
	    }
	    if( length == 160 )
	    {
	        this.spare3    = (int)  six_state.get( 2  );
	        this.destid2   = (long) six_state.get( 30 );
	        this.msgid2_1  = (int)  six_state.get( 6  );
	        this.offset2_1 = (int)  six_state.get( 12 );
	        this.spare4    = (int)  six_state.get( 2  );
	        this.num_reqs  = 3;
	    }
	}
}
