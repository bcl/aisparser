package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 19 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 19 class
 * Extended Class B Equipment Position Report
 * 
 */
public class Message19 extends Messages {
    int             regional1;         // 8 bits   : Regional Bits
    int             sog;               // 10 bits  : Speed Over Ground
    int             pos_acc;           // 1 bit    : Position Accuracy
    Position        pos;               //          : Lat/Long 1/100000 minute
    int             cog;               // 12 bits  : Course Over Ground
    int             true_heading;      // 9 bits   : True Heading
    int             utc_sec;           // 6 bits   : UTC Seconds
    int             regional2;         // 4 bits   : Regional Bits
    String          name;              // 120 bits : Ship Name in ASCII
    int             ship_type;         // 8 bits   : Type of Ship and Cargo
    int             dim_bow;           // 9 bits   : GPS Ant. Distance from Bow
    int             dim_stern;         // 9 bits   : GPS Ant. Distance from Stern   
    int             dim_port;          // 6 bits   : GPS Ant. Distance from Port
    int             dim_starboard;     // 6 bits   : GPS Ant. Distance from Starboard
    int             pos_type;          // 4 bits   : Type of Position Fixing Device
    int             raim;              // 1 bit    : RAIM Flag
    int             dte;               // 1 bit    : DTE Flag
    int             spare;             // 5 bits   : Spare
    
    public int regional1() { return this.regional1; }
    public int sog() { return this.sog; }
    public int pos_acc() { return this.pos_acc; }
    public long longitude() { return this.pos.longitude(); }
    public long latitude() { return this.pos.latitude(); }
    public int cog() { return this.cog; }
    public int true_heading() { return this.true_heading; }
    public int utc_sec() { return this.utc_sec; }
    public int regional2() { return this.regional2; }
    public String name() { return this.name; }
    public int ship_type() { return this.ship_type; }
    public int dim_bow() { return this.dim_bow; }
    public int dim_stern() { return this.dim_stern; }
    public int dim_port() { return this.dim_port; }
    public int dim_starboard() { return this.dim_starboard; }
    public int pos_type() { return this.pos_type; }
    public int raim() { return this.raim; }
    public int dte() { return this.dte; }
    public int spare() { return this.spare; }
    
	public Message19()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if ( six_state.bit_length() != 312 )
			throw new AISMessageException("Message 19 wrong length");
		
		super.parse( 19, six_state );
		
	    this.regional1      = (int) six_state.get( 8  );
	    this.sog            = (int)            six_state.get( 10 );
	    this.pos_acc        = (int)            six_state.get( 1  );

	    this.pos = new Position();
	    this.pos.setLongitude((long) six_state.get( 28 ));
	    this.pos.setLatitude((long) six_state.get( 27 ));

	    this.cog            = (int)            six_state.get( 12 );
	    this.true_heading   = (int)            six_state.get( 9  );
	    this.utc_sec        = (int)            six_state.get( 6  );
	    this.regional2      = (int)            six_state.get( 4  );
	    this.name           = six_state.get_string(20);
	    this.ship_type      = (int)            six_state.get( 8  );
	    this.dim_bow        = (int)            six_state.get( 9  );
	    this.dim_stern      = (int)            six_state.get( 9  );
	    this.dim_port       = (int)            six_state.get( 6  );
	    this.dim_starboard  = (int)            six_state.get( 6  );
	    this.pos_type       = (int)            six_state.get( 4  );
	    this.raim           = (int)            six_state.get( 1  );
	    this.dte            = (int)            six_state.get( 1  );
	    this.spare          = (int)            six_state.get( 5  );   
	}
}
