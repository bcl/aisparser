package com.aisparser;

/**
 * AIS Parser SDK
 * AIS Message 9 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 9 class
 * Standard SAR Aircraft Position Report
 * 
 */
public class Message9 extends Messages {
    int            altitude;          // 12 bits  : Altitude
    int            sog;               // 10 bits  : Speed Over Ground
    int            pos_acc;           // 1 bit    : Position Accuracy
    Position       pos;               //          : Lat/Long 1/100000 minute
    int            cog;               // 12 bits  : Course Over Ground
    int            utc_sec;           // 6 bits   : UTC Seconds
    int            regional;          // 8 bits   : Regional bits
    int            dte;               // 1 bit    : DTE flag
    int            spare;             // 3 bits   : Spare
    int            assigned;          // 1 bit    : Assigned mode flag
    int            raim;              // 1 bit    : RAIM flag
    int            comm_state;        // 1 bit    : Comm State Flag
    Sotdma         sotdma_state = null;
    Itdma          itdma_state = null;
    
    public int altitiude() { return this.altitude; }
    public int sog() { return this.sog; }
    public int pos_acc() { return this.pos_acc; }
    public long longitude() { return this.pos.longitude(); }
    public long latitude() { return this.pos.latitude(); }
    public int cog() { return this.cog; }
    public int utc_sec() { return this.utc_sec; }
    public int regional() { return this.regional; }
    public int dte() { return this.dte; }
    public int spare() { return this.spare; }
    public int assigned() { return this.assigned; }
    public int raim() { return this.raim; }
    public int comm_state() { return this.comm_state; }
    public Sotdma sotdma_state() { return this.sotdma_state; }
    public Itdma itdma_state() { return this.itdma_state; }
    
	public Message9()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		if (six_state.bit_length() != 168)
			throw new AISMessageException("Message 9 wrong length");
		
		super.parse( 9, six_state );
        
		this.altitude   = (int)   six_state.get( 12 );
		this.sog        = (int)   six_state.get( 10 );
		this.pos_acc    = (int)   six_state.get( 1  );

	    this.pos = new Position();
	    this.pos.setLongitude((long) six_state.get( 28 ));
	    this.pos.setLatitude((long) six_state.get( 27 ));

    	this.cog        = (int)   six_state.get( 12 );
    	this.utc_sec    = (int)   six_state.get( 6  );
    	this.regional   = (char)  six_state.get( 8  );
    	this.dte        = (char)  six_state.get( 1  );
    	this.spare      = (char)  six_state.get( 3  );
    	this.assigned   = (char)  six_state.get( 1  );
    	this.raim       = (char)  six_state.get( 1  );
    	this.comm_state = (char)  six_state.get( 1  );
    	
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
