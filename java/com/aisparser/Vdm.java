package com.aisparser;
/**
 * AIS Parser SDK
 * VDM Parser Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */


class ChecksumFailedException extends Exception
{
	public ChecksumFailedException() {}
	public ChecksumFailedException( String str )
	{
		super(str);
	}
}


class VDMSentenceException extends Exception
{
	public VDMSentenceException() {}
	public VDMSentenceException( String str )
	{
		super(str);
	}
}



/**
 * VDM Class
 *
 * This keeps track partial messages until a complete message has been
 * received and it holds the sixbit state for exteacting bits from the
 * message.
 */	
public class Vdm {
    int    msgid;               //!< Message ID 0-31
    int    sequence;            //!< VDM message sequence number
    int    total;               //!< Total # of parts for the message
    int    num;                 //!< Number of the last part stored
    char   channel;             //!< AIS Channel character
    Sixbit six_state;           //!< sixbit parser state
	
    
    /*
     * Constructor, initialize the state
     */
    public Vdm()
    {
    	this.total = 0;
    	this.sequence = 0;
    	this.num = 0;
    }

    /**
     * Return the 6-bit state
     */
    public Sixbit sixbit()
    {
    	return this.six_state;
    }
    
    /**
     * Get the message id
     */
    public int msgid()
    {
    	return this.msgid;
    }
	
	/**
	 *  Assemble AIVDM/VDO sentences
	 *
     * This function handles re-assembly and extraction of the 6-bit data
     * from AIVDM/AIVDO sentences.
     *
     * Because the NMEA standard limits the length of a line to 80 characters
     * some AIS messages, such as message 5, are output as a multipart VDM 
     * messages. 
     * This routine collects the 6-bit encoded data from these parts and 
     * returns a 1 when all pieces have been reassembled.
     * 
     * It expects the sentences to:
     * - Be in order, part 1, part 2, etc.
     * - Be from a single sequence
     * 
     * It will return an error if it receives a piece out of order or from
     * a new sequence before the previous one is finished.
     * 
     * Returns
     *   - 0 Complete packet
     *   - 1 Incomplete packet
     *   - 2 NMEA 0183 checksum failed
     *   - 3 Not an AIS message
     *   - 4 Error with nmea_next_field
     *   - 5 Out of sequence packet
     *
     */	
	public int add( String str )
		throws ChecksumFailedException, StartNotFoundException, VDMSentenceException
	{
		String[] fields;
		Nmea nmea_message = new Nmea();
		int ptr;
		int	total;
		int num;
		int sequence;
		
		nmea_message.init(str);
		
		if (nmea_message.checkChecksum() != 0)
			throw new ChecksumFailedException();
		ptr = nmea_message.find_start();

		// Allow any sender type for VDM and VDO messages
		if (	!str.regionMatches(ptr+3, "VDM", 0, 3) &&
				!str.regionMatches(ptr+3, "VDO", 0, 3)		)
	    {
	        /* Not an AIS message */
	        throw new VDMSentenceException("Not a VDM or VDO message");
	    }

		fields = str.split(",|\\*");
		if (fields.length != 8)
			throw new VDMSentenceException("Does not have 8 fields");
		
		// Get the message info for multipart messages
		try {
			total = Integer.parseInt(fields[1]);
			num   = Integer.parseInt(fields[2]);
		} catch (NumberFormatException e) {
			throw new VDMSentenceException("total or num field is not an integer");
		}
		try {
		    sequence = Integer.parseInt(fields[3]);
		} catch (NumberFormatException e) {
			// null sequence is not fatal
			sequence = 0;
		}

		// Are we looking for more message parts?
		if (this.total > 0)
		{
			if( (this.sequence != sequence) || (this.num != num-1) )
	        {
	            this.total = 0;
	            this.sequence =0;
	            this.num = 0;
	            throw new VDMSentenceException("Out of sequence sentence");
	        }
	        this.num++;
		} else {
			this.total = total;
	        this.num = num;
	        this.sequence = sequence;
	        this.six_state = new Sixbit();
	        this.six_state.init("");
		}
		this.channel = fields[4].charAt(0);
		
		this.six_state.add(fields[5]);
		
		if ((total==0) || (this.total == num))
		{
		    this.total    = 0;
		    this.num      = 0;
		    this.sequence = 0;

		    // Get the message id
		    try {
		    	this.msgid = (int) six_state.get(6);
		    } catch (SixbitsExhaustedException e) {
		    	throw new VDMSentenceException("Not enough bits for msgid");
		    }
		    
		    // Adjust bit count
		   this.six_state.padBits( Integer.parseInt(fields[6]) );
		    
		    /* Found a complete packet */
		    return 0;
		}
		
		// No complete message yet
		return 1;
	}
}
