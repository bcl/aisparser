#!/bin/env python
"""
   NMEA string parser class
   by Brian C. Lane <bcl@brianlane.com>
   Copyright 2006 by Brian C. Lane
   All Rights Reserved
"""

def check_checksum( msg ):
    """
    Check the checksum of a NMEA 0183 sentence. Skips leading ! or $ and stops
    at *
    It ignores non-standard extensions after the checksum
    It ignores anything before the first $ or ! in the string
    """

    # Find the start of the NMEA sentence
    startchars = "!$"
    for c in startchars:
        i = msg.find(c)
        if i >= 0: break
    else:
        return (False, None, None)

    # Calculate the checksum on the message
    sum1 = 0
    for c in msg[i+1:]:
        if c == '*':
	    break;
        sum1 = sum1 ^ ord(c)
    sum1 = sum1 & 0xFF
    
    # Find the checksum at the end of the string
    sum2 = -1
    try:
        i = msg.index( '*' )
	sum2 = int(msg[i+1:i+3], 16)
	
	if sum1 == sum2:
	    return (True, sum1, sum2)
    except:
        pass

    return (False, sum1, sum2)


def make_checksum( msg ):
    """
    Make a NMEA 0183 checksum on a string. Skips leading ! or $ and stops
    at *
    It ignores anything before the first $ or ! in the string
    """

    # Find the start of the NMEA sentence
    startchars = "!$"
    for c in startchars:
        i = msg.find(c)
        if i >= 0: break
    else:
        return (False, None, None)

    # Calculate the checksum on the message
    sum1 = 0
    for c in msg[i+1:]:
        if c == '*':
	    break;
        sum1 = sum1 ^ ord(c)
    sum1 = sum1 & 0xFF
    
    return sum1
