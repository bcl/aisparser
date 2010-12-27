package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 4 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */


/**
 * AIS Message 4 class
 * Base Station Report
 * 
 */
public class Message4 extends Messages {
    int             utc_year;          // 14 bits : UTC Year
    int             utc_month;         // 4 bits  : UTC Month
    int             utc_day;           // 5 bits  : UTC Day
    int             utc_hour;          // 5 bits  : UTC Hour
    int             utc_minute;        // 6 bits  : UTC Minute
    int             utc_second;        // 6 bits  : UTC Second
    int             pos_acc;           // 1 bit   : Position Accuracy
    Position       pos;                //         : Lat/Long 1/100000 minute
    int             pos_type;          // 4 bits  : Type of position fixing device
    int             spare;             // 10 bits : Spare
    int             raim;              // 1 bit   : RAIM flag
    int             sync_state;        // 2 bits  : SOTDMA sync state
    int             slot_timeout;      // 3 bits  : SOTDMA Slot Timeout
    int             sub_message;       // 14 bits : SOTDMA sub-message

    public int utc_year() { return this.utc_year; }
    public int utc_month() { return this.utc_month; }
    public int utc_day() { return this.utc_day; }
    public int utc_hour() { return this.utc_hour; }
    public int utc_minute() { return this.utc_minute; }
    public int utc_second() { return this.utc_second; }
    public int pos_acc() { return this.pos_acc; }
    public long longitude() { return this.pos.longitude(); }
    public long latitude() { return this.pos.latitude(); }
    public int pos_type() { return this.pos_type; }
    public int spare() { return this.spare; }
    public int raim() { return this.raim; }
    public int sync_state() { return this.sync_state; }
    public int slot_timeout() { return this.slot_timeout; }
    public int sub_message() { return this.sub_message; }
    
    
	public Message4()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if (six_state.bit_length() != 168 )
			throw new AISMessageException("Message 5 wrong length");
		
		super.parse( 4, six_state );
		
		this.utc_year     = (int)  six_state.get( 14 );
	    this.utc_month    = (int)  six_state.get( 4  );
	    this.utc_day      = (int)  six_state.get( 5  );
	    this.utc_hour     = (int)  six_state.get( 5  );
	    this.utc_minute   = (int)  six_state.get( 6  );
	    this.utc_second   = (int)  six_state.get( 6  );
	    this.pos_acc      = (int)  six_state.get( 1  );

	    this.pos = new Position();
	    this.pos.setLongitude((long) six_state.get( 28 ));
	    this.pos.setLatitude((long) six_state.get( 27 ));

	    this.pos_type     = (int)  six_state.get( 4  );
	    this.spare        = (int)  six_state.get( 10 );
	    this.raim         = (int)  six_state.get( 1  );
	    this.sync_state   = (int)  six_state.get( 2  );
	    this.slot_timeout = (int)  six_state.get( 3  );
	    this.sub_message  = (int)  six_state.get( 14 );
	}
}
