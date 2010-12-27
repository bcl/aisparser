package com.aisparser;
/**
 * 6-bit packed ASCII functions
 * @author Copyright 2006-2008 by Brian C. Lane <bcl@brianlane.com
 * All Rights Reserved
 *
 */

class SixbitsExhaustedException extends Exception
{
	public SixbitsExhaustedException() {}
	public SixbitsExhaustedException( String str )
	{
		super(str);
	}
}


/**
 * This class's methods are used to extract data from the 6-bit packed
 * ASCII string used by AIVDM/AIVDO AIS messages.
 *   
 * init() should be called with a sixbit ASCII string.
 *	
 * Up to 32 bits of data are fetched from the string by calling get()
 * 
 * Use padBits() to set the number of padding bits at the end of the message,
 * it defaults to 0 if not set.
 *  
 *
 */
public class Sixbit {
	private final int pow2_mask[] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F };
	private String bits;          //!< raw 6-bit ASCII data string
	private int bitsIndex;        //!< Index of next character
    private int remainder;        //!< Remainder bits
    private int remainderLength;  //!< Number of remainder bits
    private int padBits;	      //!< Number of padding bits at end

    
    /**
     * Totally empty constructor
     */
    public Sixbit() {}


    /**
     * Initialize a 6-bit datastream structure
	 *
	 * This function initializes the state of the sixbit parser variables 
	 * 
     */
    public void init( String bits )
    {
    	this.bits = bits;
    	this.bitsIndex = 0;
        this.remainder = 0;
        this.remainderLength = 0;
        this.padBits = 0;
    }
    
    
    /**
     * Set the bit padding value
     */
    public void padBits( int num )
    {
    	this.padBits = num; 
    }
    
    
    /**
     * Add more bits to the buffer
     */
    public void add( String bits )
    {
    	this.bits += bits;
    }
    
    
    /*
     * Return the number of bits
     * 
     * Takes into account the number of padding bits.
     */
	public int bit_length()
	{
		return ((length() * 6) - this.padBits);
	}


	/**
	 * Return the number of bytes in the sixbit string
	 */
	public int length()
	{
		return this.bits.length();
	}

	
	/**
	 * Convert an ASCII value to a 6-bit binary value
	 *  
     * This function checks the ASCII value to make sure it can be converted.
     * If not, it throws an IllegalArgumentException
     * Otherwise it returns the 6-bit binary value.
     * 
     * @param ASCII character to convert
     *
     * This is used to convert the packed 6-bit value to a binary value. It
     * is not used to convert data from fields such as the name and 
     * destination -- Use ais2ascii() instead.
     */
	public int binfrom6bit( int ascii )
		throws IllegalArgumentException
	{
		if( (ascii < 0x30) || (ascii > 0x77) || ((ascii > 0x57) && (ascii < 0x60)) )
			throw new IllegalArgumentException("Illegal 6-bit ASCII value");
		if( ascii < 0x60 )
			return (ascii - 0x30) & 0x3F;
		else
			return (ascii - 0x38) & 0x3F;
	}
	
	
	/** Convert a binary value to a 6-bit ASCII value
	 *
  	 * This function checks the binary value to make sure it can be converted.
     * If not, it throws an IllegalArgumentException.
     * Otherwise it returns the 6-bit ASCII value.
     * 
     * @param value to convert
     * @returns 6-bit ASCII
     * 
	 */
	public int binto6bit( int value )
		throws IllegalArgumentException
	{
		if (value > 0x3F )
			throw new IllegalArgumentException("Value is out of range (0-0x3F)");
		if (value < 0x28)
			return value + 0x30;
		else
			return value + 0x38;
	}

	
	/** Convert a AIS 6-bit character to ASCII
	 * 
     * @param value 6-bit value to be converted
     *
     * return 
     *  - corresponding ASCII value (0x20-0x5F)
     *  
     * This function is used to convert binary data to ASCII. This is 
     * different from the 6-bit ASCII to binary conversion for VDM 
     * messages; it is used for strings within the datastream itself.
     * eg. Ship Name, Callsign and Destination.
     */
	public int ais2ascii( int value )
		throws IllegalArgumentException
	{
		if (value > 0x3F )
			throw new IllegalArgumentException("Value is out of range (0-0x3F)");
		if( value < 0x20 )
			return value + 0x40;
		else
			return value;
	}

	
	/** 
	 * Return 0-32 bits from a 6-bit ASCII stream
	 *
     * @param numbits number of bits to return
	 *
     * This method returns the requested number of bits to the caller.
     * It pulls the bits from the raw 6-bit ASCII as they are needed.
	 *
	 */
	public long get( int numbits )
		throws SixbitsExhaustedException
	{
	    long result;
	    int fetch_bits;
	    
	    result = 0;
	    fetch_bits = numbits;
	    
	    while( fetch_bits > 0 )
	    {
	        /*  Is there anything left over from the last call? */
	        if( this.remainderLength > 0 )
	        {
	            if( this.remainderLength <= fetch_bits )
	            {
	                /* reminder is less than or equal to what is needed */
	                result = (result << 6) + this.remainder;
	                fetch_bits -= this.remainderLength;
	                this.remainder = 0;
	                this.remainderLength = 0;
	            } else {
	                /* remainder is larger than what is needed
	                   Take the bits from the top of remainder
	                */
	                result = result << fetch_bits;
	                result += this.remainder >> (this.remainderLength - fetch_bits);

	                /* Fixup remainder */
	                this.remainderLength -= fetch_bits;
	                this.remainder &= pow2_mask[(int) this.remainderLength];

	                return result;
	            }
	        }
	        
	        /* Get the next block of 6 bits from the ASCII string */
	        if( this.bitsIndex < this.bits.length() )
	        {
	            this.remainder = binfrom6bit( (int) bits.charAt(this.bitsIndex) );
	            this.bitsIndex++;
	            if (this.bitsIndex == this.bits.length())
	            	this.remainderLength = 6 - this.padBits;
	            else
	            	this.remainderLength = 6;
	        } else if (fetch_bits > 0) {
	            /* Ran out of bits */
	            throw new SixbitsExhaustedException("Ran out of bits");
	        } else {
	        	return result;
	        }
	    }
	    return result;
	}

	
	/**
	 * Get an ASCII string from the 6-bit data stream
	 * 
	 * @param length Number of characters to retrieve
	 * 
	 * @return
	 * String of the characters
	 */
	public String get_string( int length )
	{
    	char[] tmp_str = new char[length];
	    	
        /* Get the 6-bit string, convert to ASCII */
    	for (int i=0; i < length; i++)
        {
    		try {
    			tmp_str[i] = (char) ais2ascii( (char) get( 6 ));
    		} catch ( SixbitsExhaustedException e) {
    			for (int j=i; j<length; j++)
    			{
    				tmp_str[j] = '@';
    			}
    			break;
    		}
	    }
	    return new String(tmp_str);
	}
}
