package com.aisparser;

/**
 * AIS Parser SDK
 * AIS Message 18 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 18 class
 * Standard Class B equipment position report
 * 
 */
public class Message18 extends Messages {
    int             regional1;         // 8 bits   : Regional Bits
    int             sog;               // 10 bits  : Speed Over Ground
    int             pos_acc;           // 1 bit    : Position Accuracy
    Position        pos;               //          : Lat/Long 1/100000 minute
    int             cog;               // 12 bits  : Course Over Ground
    int             true_heading;      // 9 bits   : True Heading
    int             utc_sec;           // 6 bits   : UTC Seconds
    int             regional2;         // 2 bits   : Regional Bits
    int             unit_flag;         // 1 bit    : Class B CS Flag
    int             display_flag;      // 1 bit    : Integrated msg14 Display Flag
    int             dsc_flag;          // 1 bit    : DSC Capability flag
    int             band_flag;         // 1 bit    : Marine Band Operation Flag
    int             msg22_flag;        // 1 bit    : Msg22 Frequency Management Flag
    int             mode_flag;         // 1 bit    : Autonomous Mode Flag
    int             raim;              // 1 bit    : RAIM Flag
    int             comm_state;        // 1 bit    : Comm State Flag
    Sotdma sotdma_state = null;
    Itdma itdma_state = null;
    
    public int regional1() { return this.regional1; }
    public int sog() { return this.sog; }
    public int pos_acc() { return this.pos_acc; }
    public long longitude() { return this.pos.longitude(); }
    public long latitude() { return this.pos.latitude(); }
    public int cog() { return this.cog; }
    public int true_heading() { return this.true_heading; }
    public int utc_sec() { return this.utc_sec; }
    public int regional2() { return this.regional2; }
    public int unit_flag() { return this.unit_flag; }
    public int display_flag() { return this.display_flag; }
    public int dsc_flag() { return this.dsc_flag; }
    public int band_flag() { return this.band_flag; }
    public int msg22_flag() { return this.msg22_flag; }
    public int mode_flag() { return this.mode_flag; }
    public int raim() { return this.raim; }
    public int comm_state() { return this.comm_state; }
    public Sotdma sotdma_state() { return this.sotdma_state; }
    public Itdma itdma_state() { return this.itdma_state; }

	public Message18()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if ( six_state.bit_length() != 168 )
			throw new AISMessageException("Message 18 wrong length");
		
		super.parse( 18, six_state );

	    this.regional1      = (int) six_state.get( 8  );
	    this.sog            = (int)           six_state.get( 10 );
	    this.pos_acc        = (int)           six_state.get( 1  );

	    this.pos = new Position();
	    this.pos.setLongitude((long) six_state.get( 28 ));
	    this.pos.setLatitude((long) six_state.get( 27 ));

	    this.cog            = (int)           six_state.get( 12 );
	    this.true_heading   = (int)           six_state.get( 9  );
	    this.utc_sec        = (int)           six_state.get( 6  );
	    this.regional2      = (int)           six_state.get( 2  );
	    this.unit_flag      = (int)           six_state.get( 1  );
	    this.display_flag   = (int)           six_state.get( 1  );
	    this.dsc_flag       = (int)           six_state.get( 1  );
	    this.band_flag      = (int)           six_state.get( 1  );
	    this.msg22_flag     = (int)           six_state.get( 1  );
	    this.mode_flag      = (int)           six_state.get( 1  );
	    this.raim           = (int)           six_state.get( 1  );
	    this.comm_state     = (int)           six_state.get( 1  );

    	if (this.comm_state == 0)
    	{
    		this.sotdma_state = new Sotdma();
    		this.sotdma_state.parse( six_state );
    	} else {
    		this.itdma_state = new Itdma();
    		this.itdma_state.parse( six_state );
    	}
	}
}
