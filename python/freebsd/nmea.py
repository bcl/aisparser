#!/bin/env python
"""
   NMEA string parser class
   by Brian C. Lane <bcl@brianlane.com>
   Copyright 2006 by Brian C. Lane
   All Rights Reserved
"""

def check_checksum(msg):
    """
    Check the checksum of a NMEA 0183 sentence. Skips leading ! or $ and stops
    at *
    It ignores non-standard extensions after the checksum
    It ignores anything before the first $ or ! in the string

    Returns tuple with:
      good checksum
      calculated checksum
      checksum from string

      Checksums will be None if no start character is found
      Second checksum, from string, will be -1 if none was found.
    """

    # Find the start of the NMEA sentence
    startchars = "!$"
    for c in startchars:
        i = msg.find(c)
        if i >= 0:
            break
    else:
        return (False, None, None)

    # Calculate the checksum on the message
    sum1 = 0
    for c in msg[i+1:]:
        if c == '*':
            break
        sum1 = sum1 ^ ord(c)
    sum1 = sum1 & 0xFF

    # Find the checksum at the end of the string
    sum2 = -1
    try:
        i = msg.index('*')
        sum2 = int(msg[i+1:i+3], 16)
    except ValueError:
        pass

    return (sum1 == sum2, sum1, sum2)


def make_checksum(msg):
    """
    Make a NMEA 0183 checksum on a string. Skips leading ! or $ and stops
    at *
    It ignores anything before the first $ or ! in the string

    Returns None if there was an invalid start character
    Otherwise returns the checksum
    """

    # Find the start of the NMEA sentence
    startchars = "!$"
    for c in startchars:
        i = msg.find(c)
        if i >= 0:
            break
    else:
        return None

    # Calculate the checksum on the message
    sum1 = 0
    for c in msg[i+1:]:
        if c == '*':
            break
        sum1 = sum1 ^ ord(c)
    sum1 = sum1 & 0xFF

    return sum1
