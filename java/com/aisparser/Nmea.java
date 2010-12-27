package com.aisparser;
/**
 * AIS Parser SDK
 * NMEA Parser Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 *
 */

class StartNotFoundException extends Exception
{
	public StartNotFoundException() {}
	public StartNotFoundException( String str )
	{
		super(str);
	}
}


class IllegalNMEACharacterException extends Exception
{
	public IllegalNMEACharacterException() {}
	public IllegalNMEACharacterException( String str )
	{
		super(str);
	}
}


/**
 * NMEA message class
 * 
 * Provides methods for finding the start of a NMEA message,
 * and calculating the checksum of the message.
 */
public class Nmea {
	private String msg;
	private int	checksum;
	
	/*
	 * A totally blank constructor
	 */
	public Nmea()
	{
		super();
	}
	
	/*
	 * Initialize it with a NMEA message string
	 */
	public void init( String msg )
	{
		this.msg = msg;
	}

	
	/**
	 * Find the start of a NMEA sentence '!' or '$' character
     *
     *  Returns:
     *   - index of the start character
     *   
     * The NMEA sentence may not always start at the beginning of the buffer,
     * use this routine to make sure the start of the sentence has been found.
     * 
	 */	
	public int find_start()
	throws StartNotFoundException
	{
		char[] msgArray = msg.toCharArray();
		int i = 0;
		
		for (char x : msgArray)
		{
			if ((x == '!') || (x == '$'))
				return i;
			i++;
		}
		throw new StartNotFoundException("NMEA Start Not Found");
	}
	
	
	/**
	 * Calculate the checksum of a NMEA 0183 sentence
     *   
     * This method will calculate the checksum of the string by skipping the
     * ! or $ character and stopping at the * character or end of string. 
     * 
     * The checksum is stored in this.checksum
     * 
   	 */	
	public void calculateChecksum() 
	throws StartNotFoundException, IllegalNMEACharacterException
	{
	    int ptr;
	  
	    /* Find start of sentence, after a '!' or '$' */
    	ptr = find_start() + 1;
	    
	    char[] msgArray = msg.substring(ptr).toCharArray();
	    
	    this.checksum = 0;
	    for ( char c : msgArray )
	    {
	    	if ( (c == '!') || (c == '$'))
	    		throw new IllegalNMEACharacterException("Start Character Found before Checksum");
	    	
	    	if (c == '*')
	    		break;
	    	
		    this.checksum ^= c;
	    }
	}
	
	
	/**
	 * Check and return the checksum of a NMEA 0183 sentence
	 *
     * Returns:
     *   - 0 if it does match
     *   - 1 if it does not match
     *   - 2 if there was an error
     *   
     *  This method calls calculateChecksum() and updates this.checksum
     * 
     */
	public int checkChecksum()
	{
		String msgChecksum;
		int ptr;
		
		try {
			calculateChecksum();
		} catch (StartNotFoundException e) {
			return 2;
		} catch (IllegalNMEACharacterException e) {
			return 2;
		}
		
		ptr = msg.indexOf('*');
		if (ptr < 0)
			return 2;
		
		try {
			msgChecksum = msg.substring(ptr+1, ptr+3);
		} catch (IndexOutOfBoundsException e) {
			return 2;
		}
		
		if ( Integer.parseInt(msgChecksum,16) != this.checksum )
			return 1;
		return 0;
	}
}
