package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 20 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 20 class
 * Data Link Management Message
 * 
 */
public class Message20 extends Messages {
    int             spare1;            // 2 bits   : Spare
    int             offset1;           // 12 bits  : Slot Offset 1
    int             slots1;            // 4 bits   : Number of Slots 1
    int             timeout1;          // 3 bits   : Timeout in Minutes 2
    int             increment1;        // 11 bits  : Slot Increment 1
    int             offset2;           // 12 bits  : Slot Offset 2
    int             slots2;            // 4 bits   : Number of Slots 2
    int             timeout2;          // 3 bits   : Timeout in Minutes 2
    int             increment2;        // 11 bits  : Slot Increment 2
    int             offset3;           // 12 bits  : Slot Offset 3
    int             slots3;            // 4 bits   : Number of Slots 3
    int             timeout3;          // 3 bits   : Timeout in Minutes 3
    int             increment3;        // 11 bits  : Slot Increment 3
    int             offset4;           // 12 bits  : Slot Offset 4
    int             slots4;            // 4 bits   : Number of Slots 4
    int             timeout4;          // 3 bits   : Timeout in Minutes 4
    int             increment4;        // 11 bits  : Slot Increment 4
    int             spare2;            // 0-6 bits : Spare
    int             num_cmds;          // Number of commands received

    public int spare1() { return this.spare1; }
    public int offset1() { return this.offset1; }
    public int slots1() { return this.slots1; }
    public int timeout1() { return this.timeout1; }
    public int increment1() { return this.increment1; }
    public int offset2() { return this.offset2; }
    public int slots2() { return this.slots2; }
    public int timeout2() { return this.timeout2; }
    public int increment2() { return this.increment2; }
    public int offset3() { return this.offset3; }
    public int slots3() { return this.slots3; }
    public int timeout3() { return this.timeout3; }
    public int increment3() { return this.increment3; }
    public int offset4() { return this.offset4; }
    public int slots4() { return this.slots4; }
    public int timeout4() { return this.timeout4; }
    public int increment4() { return this.increment4; }
    public int spare2() { return this.spare2; }
    public int num_cmds() { return this.num_cmds; }
    
	public Message20()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		int length = six_state.bit_length();
		if ((length < 72) || (length > 162))
			throw new AISMessageException("Message 20 wrong length");
		
		super.parse( 20, six_state );

		this.spare1         = (int) six_state.get( 2  );
	    this.offset1        = (int) six_state.get( 12 );
	    this.slots1         = (int) six_state.get( 4  );
	    this.timeout1       = (int) six_state.get( 3  );
	    this.increment1     = (int) six_state.get( 11 );
	    this.num_cmds       = 1;

	    if( length > 72 )
	    {
	        this.offset2    = (int) six_state.get( 12 );
	        this.slots2     = (int) six_state.get( 4  );
	        this.timeout2   = (int) six_state.get( 3  );
	        this.increment2 = (int) six_state.get( 11 );
	        this.num_cmds   = 2;
	    }

	    if( length > 104 )
	    {
	        this.offset3    = (int)  six_state.get( 12 );
	        this.slots3     = (int) six_state.get( 4  );
	        this.timeout3   = (int) six_state.get( 3  );
	        this.increment3 = (int)  six_state.get( 11 );
	        this.num_cmds   = 3;
	    }

	    if( length > 136 )
	    {
	        this.offset4    = (int)  six_state.get( 12 );
	        this.slots4     = (int) six_state.get( 4  );
	        this.timeout4   = (int) six_state.get( 3  );
	        this.increment4 = (int)  six_state.get( 11 );
	        this.num_cmds   = 4;
	    }
	}
}
