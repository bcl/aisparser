package com.aisparser;
/**
 * AIS Parser SDK
 * AIS Position Class
 * Copyright 2008 by Brian C. Lane <bcl@brianlane.com>
 * All Rights Reserved
 * 
 * @author Brian C. Lane
 */


/**
 * AIS Position class
 * 
 * Convert raw unsigned AIS position to signed 1/10000 degree position
 * and provide helper methods for other formats
 * 
 */
public class Position {
	long	longitude;
	long	latitude;
	
	public Position()
	{
		super();
	}
	
	public void setLongitude( long raw_longitude )
	{
	    /* Convert longitude to signed number */
	    if( raw_longitude >= 0x8000000 )
	    {
	        this.longitude = 0x10000000 - raw_longitude;
	        this.longitude *= -1;
	    } else {
	    	this.longitude = raw_longitude;
	    }
	}
	
	public void setLatitude( long raw_latitude )
	{
	    /* Convert latitude to signed number */
	    if( raw_latitude >= 0x4000000 )
	    {
	        this.latitude = 0x8000000 - raw_latitude;
	        this.latitude *= -1;
	    } else {
	    	this.latitude = raw_latitude;
	    }
	}
	
	public long longitude()
	{
		return this.longitude;
	}
	
	public long latitude()
	{
		return this.latitude;
	}
}
