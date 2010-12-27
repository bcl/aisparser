package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 3 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */


/**
 * AIS Message 3 class
 * Position Report
 * 
 */
public class Message3 extends Messages {
    int            nav_status;        // 4 bits  : Navigational Status
    int			   rot;               // 8 bits  : Rate of Turn   
    int            sog;               // 10 bits : Speed Over Ground
    int            pos_acc;           // 1 bit   : Position Accuracy
    Position       pos;               //         : Lat/Long 1/10000 minute
    int            cog;               // 12 bits : Course over Ground
    int            true_heading;      // 9 bits  : True heading
    int            utc_sec;           // 6 bits  : UTC Seconds
    int            regional;          // 4 bits  : Regional bits
    int            spare;             // 1 bit   : Spare
    int            raim;              // 1 bit   : RAIM flag
    int            sync_state;        // 2 bits  : SOTDMA sync state
    int            slot_increment;    // 13 bits : ITDMA Slot Increment
    int            num_slots;         // 3 bits  : ITDMA Number of Slots
    int            keep;              // 1 bit   : ITDMA Keep Flag
    
    public int nav_status() { return this.nav_status; }
    public int rot() { return this.rot; }
    public int sog() { return this.sog; }
    public int pos_acc() { return this.pos_acc; }
    public long longitude() { return this.pos.longitude(); }
    public long latitude() { return this.pos.latitude(); }
    public int cog() { return this.cog; }
    public int true_heading() { return this.true_heading; }
    public int utc_sec() { return this.utc_sec; }
    public int regional() { return this.regional; }
    public int spare() { return this.spare; }
    public int raim() { return this.raim; }
    public int sync_state() { return this.sync_state; }
    public int slot_increment() { return this.slot_increment; }
    public int num_slots() { return this.num_slots; }
    public int keep() { return this.keep; }

    
	public Message3()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if (six_state.bit_length() != 168 )
			throw new AISMessageException("Message 3 wrong length");
		
		super.parse( 3, six_state );
		
	    /* Parse the Message 3 */
	    this.nav_status     = (int)  six_state.get( 4 );
	    this.rot            = (int)  six_state.get( 8 );
	    this.sog            = (int)  six_state.get( 10 );
	    this.pos_acc        = (int)  six_state.get( 1 );

	    this.pos = new Position();
	    this.pos.setLongitude((long) six_state.get( 28 ));
	    this.pos.setLatitude((long) six_state.get( 27 ));

	    this.cog            = (int)  six_state.get( 12 );
	    this.true_heading   = (int)  six_state.get( 9 );
	    this.utc_sec        = (int)  six_state.get( 6 );
	    this.regional       = (int)  six_state.get( 4 );
	    this.spare          = (int)  six_state.get( 1 );
	    this.raim           = (int)  six_state.get( 1 );
	    this.sync_state     = (int)  six_state.get( 2 );
	    this.slot_increment = (int)  six_state.get( 13 );
	    this.num_slots      = (int)  six_state.get( 3 );
	    this.keep           = (int)  six_state.get(1);
	}
}
