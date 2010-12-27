package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 24 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 24 class
 * Class B"CS" Static Data Report
 * 
 */
public class Message24 extends Messages {
    int            part_number;       // 2 bits   : Part Number

    //!< Message 24A 
    String         name;              // 120 bits : Ship Name in ASCII

    //!< Message 24B 
    int            ship_type;         // 8 bits   : Type of Ship and Cargo
    String         vendor_id;         // 42 bits  : Vendor ID in ASCII
    String         callsign;          // 42 bits  : Callsign in ASCII
    int            dim_bow;           // 9 bits   : GPS Ant. Distance from Bow
    int            dim_stern;         // 9 bits   : GPS Ant. Distance from Stern
    int            dim_port;          // 6 bits   : GPS Ant. Distance from Port
    int            dim_starboard;     // 6 bits   : GPS Ant. Distance from Starboard
    int            spare;             // 6 bits   : Spare

    int            flags;             // A/B flags - A = 1  B = 2  Both = 3  
    
    public int part_number() { return this.part_number; }
    public String name() { return this.name; }
    public int ship_type() { return this.ship_type; }
    public String vendor_id() { return this.vendor_id; }
    public String callsign() { return this.callsign; }
    public int dim_bow() { return this.dim_bow; }
    public int dim_stern() { return this.dim_stern; }
    public int dim_port() { return this.dim_port; }
    public int dim_starboard() { return this.dim_starboard; }
    public int spare() { return this.spare; }
    public int flags() { return this.flags; }
    
	public Message24()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		int length = six_state.bit_length();
		if ((length != 160) && (length != 168))
			throw new AISMessageException("Message 24 wrong length");
		
		super.parse( 24, six_state );
		
	    this.part_number    = (int)          six_state.get( 2  );

	    if( this.part_number == 0 )
	    {
	        /* Parse 24A */
	        /* Get the Ship Name, convert to ASCII */
	        this.name = six_state.get_string(20);
	        
	        /* Indicate reception of part A */
	        this.flags |= 0x01;
	    } else if( this.part_number == 1 ) {
	        /* Parse 24B */
	        this.ship_type = (int) six_state.get( 8  );
	        this.vendor_id = six_state.get_string(7);
	        this.callsign = six_state.get_string(7);

	        this.dim_bow      = (int)   six_state.get( 9  );
	        this.dim_stern    = (int)   six_state.get( 9  );
	        this.dim_port     = (int)  six_state.get( 6  );
	        this.dim_starboard= (int)  six_state.get( 6  );
	        this.spare        = (int)  six_state.get( 6  );

	        /* Indicate reception of part A */
	        this.flags |= 0x02;
	    } else {
	    	throw new AISMessageException("Unknown Message 24 Part #");
	    }
	}
}
