package com.aisparser;
/**
 * AIS Parser SDK
 * Base Class for Messages
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */

class AISMessageException extends Exception
{
	public AISMessageException() {}
	public AISMessageException( String str )
	{
		super(str);
	}
}


/**
 * AIS Message base class
 * 
 * All the messages are derived from this class which provides the msgid, 
 * repeat value and userid
 *
 */
public class Messages {
    private int    msgid;             // 6 bits  : Message ID (1)
    private int    repeat;            // 2 bits  : Repeated
    private long   userid;            // 30 bits : UserID / MMSI    

    public Messages() {}
    
    public int msgid() { return this.msgid; }
    public int repeat() { return this.repeat; }
    public long userid() { return this.userid; }
    
    // Subclasses need to override with their own parsing method
    public void parse( int msgid, Sixbit six_state )
    	throws SixbitsExhaustedException
    {
    	this.msgid = msgid;
		this.repeat = (int)  six_state.get(2);
		this.userid = (long) six_state.get(30);
    }
}
