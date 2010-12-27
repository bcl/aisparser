package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 5 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 5 class
 * Static and Voyage Related Data
 * 
 */
public class Message5 extends Messages {
    int            version;           // 2 bits          : AIS Version
    long           imo;               // 30 bits         : IMO Number
    String         callsign;          // 7x6 (42) bits   : Callsign
    String         name;              // 20x6 (120) bits : Ship Name
    int            ship_type;         // 8 bits          : Type of Ship and Cargo
    int            dim_bow;           // 9 bits          : GPS Ant. Distance from Bow
    int            dim_stern;         // 9 bits          : GPS Ant. Distance from stern
    int            dim_port;          // 6 bits          : GPS Ant. Distance from port
    int            dim_starboard;     // 6 bits          : GPS Ant. Distance from starboard
    int            pos_type;          // 4 bits          : Type of position fixing device
    long           eta;               // 20 bits         : Estimated Time of Arrival MMDDHHMM
    int            draught;           // 8 bits          : Maximum present static draught
    String         dest;              // 6x20 (120) bits : Ship Destination
    int            dte;               // 1 bit           : DTE flag
    int            spare;             // 1 bit           : spare

    public int version() { return this.version; }
    public long imo() { return this.imo; }
    public String callsign() { return this.callsign; }
    public String name() { return this.name; }
    public int ship_type() { return this.ship_type; }
    public int dim_bow() { return this.dim_bow; }
    public int dim_stern() { return this.dim_stern; }
    public int dim_port() { return this.dim_port; }
    public int dim_starboard() { return this.dim_starboard; }
    public int pos_type() { return this.pos_type; }
    public long eta() { return this.eta; }
    public int draught() { return this.draught; }
    public String dest() { return this.dest; }
    public int dte() { return this.dte; }
    public int spare() { return this.spare; }
    
    
	public Message5()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if (six_state.bit_length() != 424 )
			throw new AISMessageException("Message 5 wrong length");
		
		super.parse( 5, six_state );
    
	    this.version      = (int)  six_state.get( 2 );
	    this.imo          = (long) six_state.get( 30 );
	    this.callsign     =        six_state.get_string(7);
	    this.name         =        six_state.get_string(20);	    
	    this.ship_type    = (int)  six_state.get( 8  );
	    this.dim_bow      = (int)  six_state.get( 9  );
	    this.dim_stern    = (int)  six_state.get( 9  );
	    this.dim_port     = (int)  six_state.get( 6  );
	    this.dim_starboard= (int)  six_state.get( 6  );
	    this.pos_type     = (int)  six_state.get( 4  );
	    this.eta          = (long) six_state.get( 20 );
	    this.draught      = (int)  six_state.get( 8  );
	    this.dest         =        six_state.get_string(20);
	    
	}
}
