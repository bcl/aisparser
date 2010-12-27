package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 22 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 22 class
 * Channel Management
 * 
 */
public class Message22 extends Messages {
    int             spare1;            // 2 bits   : Spare
    int             channel_a;         // 12 bits  : M.1084 Channel A Frequency
    int             channel_b;         // 12 bits  : M.1084 Channel B Frequency
    int             txrx_mode;         // 4 bits   : TX/RX Mode
    int             power;             // 1 bit    : Power Level
    Position        NE_pos;            //          : NE Corner Lat/Long in 1/1000 minutes
    long            addressed_1;       // 30 bits  : Destination MMSI 1
    Position        SW_pos;            //          : SW Corner Lat/Long in 1/1000 minutes
    long            addressed_2;       // 30 bits  : Destination MMSI 2
    int             addressed;         // 1 bit    : Addressed flag
    int             bw_a;              // 1 bit    : Channel A Bandwidth
    int             bw_b;              // 1 bit    : Channel B Bandwidth
    int             tz_size;           // 3 bits   : Transitional Zone size
    long            spare2;            // 23 bits  : Spare
    
    public int spare1() { return this.spare1; }
    public int channel_a() { return this.channel_a; }
    public int channel_b() { return this.channel_b; }
    public int txrx_mode() { return this.txrx_mode; }
    public int power() { return this.power; }
    public long NE_longitude() { return this.NE_pos.longitude(); }
    public long NE_latitude() { return this.NE_pos.latitude(); }
    public long addressed_1() { return this.addressed_1; }
    public long SW_longitude() { return this.SW_pos.longitude(); }
    public long SW_latitude() { return this.SW_pos.latitude(); }
    public long addressed_2() { return this.addressed_2; }
    public int addressed() { return this.addressed; }
    public int bw_a() { return this.bw_a; }
    public int bw_b() { return this.bw_b; }
    public int tz_size() { return this.tz_size; }
    public long spare2() { return this.spare2; }
    
	public Message22()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if (six_state.bit_length() != 168)
			throw new AISMessageException("Message 22 wrong length");
		
		super.parse( 22, six_state );

	    this.spare1         = (int)          six_state.get( 1  );
	    this.channel_a      = (int)           six_state.get( 12 );
	    this.channel_b      = (int)           six_state.get( 12 );
	    this.txrx_mode      = (int)          six_state.get( 4  );
	    this.power          = (int)          six_state.get( 1  );

	    long NE_longitude   = (long) six_state.get( 18 );
	    long NE_latitude    = (long) six_state.get( 17 );

	    long SW_longitude   = (long) six_state.get( 18 );
	    long SW_latitude    = (long) six_state.get( 17 );

	    this.addressed      = (int)          six_state.get( 1  );
	    this.bw_a           = (int)          six_state.get( 1  );
	    this.bw_b           = (int)          six_state.get( 1  );
	    this.tz_size        = (int)          six_state.get( 3  );

	    /* Is the position actually an address? */
	    if (this.addressed == 1)
	    {
	        /* Convert the positions to addresses */
	        this.addressed_1 = (NE_longitude << 12) + (NE_latitude >> 5);
	        this.addressed_2 = (SW_longitude << 12) + (SW_latitude >> 5);
	    } else {

		    this.NE_pos = new Position();
		    this.NE_pos.setLongitude( NE_longitude * 10 );
		    this.NE_pos.setLatitude( NE_latitude * 10 );

		    this.SW_pos = new Position();
		    this.SW_pos.setLongitude( SW_longitude * 10 );
		    this.SW_pos.setLatitude( SW_latitude * 10 );
	    }
	}
}
