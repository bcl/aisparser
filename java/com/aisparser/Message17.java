package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Message 17 Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

/**
 * AIS Message 17 class
 * DGNSS Broadcast binary message
 * 
 */
public class Message17 extends Messages {
    int             spare1;            // 2 bits      : Spare
    Position        pos;               //             : Lat/Long 1/100000 minute
    int             spare2;            // 5 bits      : Spare
    int             msg_type;          // 6 bits      : Mesage Type from M.823
    int             station_id;        // 10 bits     : Station ID from M.823
    int             z_count;           // 13 bits     : Z Count
    int             seq_num;           // 3 bits      : Sequence Number
    int             num_words;         // 5 bits      : Number of Data Words
    int             health;            // 3 bits      : Reference Station Health from M.823
    Sixbit          data;              // 0-696 bits  : Data payload
    
    public int spare1() { return this.spare1; }
    public long longitude() { return this.pos.longitude(); }
    public long latitude() { return this.pos.latitude(); }
    public int spare2() { return this.spare2; }
    public int msg_type() { return this.msg_type; }
    public int station_id() { return this.station_id; }
    public int z_count() { return this.z_count; }
    public int seq_num() { return this.seq_num; }
    public int num_words() { return this.num_words; }
    public int health() { return this.health; }
    public Sixbit data() { return this.data; }
    
	public Message17()
	{
		super();
	}
	
	public void parse( Sixbit six_state )
		throws SixbitsExhaustedException, AISMessageException
	{
		int length = six_state.bit_length();
		if ((length < 80) || (length > 816))
			throw new AISMessageException("Message 17 wrong length");
		
		super.parse( 17, six_state );

	    this.spare1       = (int)            six_state.get( 2  );

	    this.pos = new Position();
	    this.pos.setLongitude((long) six_state.get( 18 ) * 10);
	    this.pos.setLatitude((long) six_state.get( 17 ) * 10);

	    this.spare2       = (int)            six_state.get( 5  );
	    this.msg_type     = (int)            six_state.get( 6  );
	    this.station_id   = (int)            six_state.get( 10 );
	    this.z_count      = (int)            six_state.get( 13 );
	    this.seq_num      = (int)            six_state.get( 3  );
	    this.num_words    = (int)            six_state.get( 5  );
	    this.health       = (int)            six_state.get( 3  );

	    this.data = six_state;
	}
}
