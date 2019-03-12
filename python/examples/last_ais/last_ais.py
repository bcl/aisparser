#!/usr/bin/python
#
# AIS Parser SDK Python Example
# Copyright 2006 by Brian C. Lane
#
# Decode incoming AIS messages, recording ship names and associated MMSI
# numbers. Write the last heard AIS ship's information to a file. I use
# this to update my dynamic signature line.
#
import os,sys
import aisparser
import socket
import shelve

debug = 0

# AIS Data Source (TCP/IP)
ais_source = ('ais.source.fqdn', 3050)
sig_file = '/tmp/last_ais.txt'
db_file  = '/tmp/last_ship'

def write_last( userid, latitude, longitude ):
    """ Write the MMSI's name and position to the sig_file """
    global last_info

    # Check for invalid positions 181/91
    if (latitude == 54600000) or (longitude == 108600000):
        return

    # Convert position to DD DD.MMMM
    (status,lat_dd,lat_min,long_ddd,long_min) = aisparser.pos2dmm( latitude, longitude );

    # Output format
    s = "%s (%s) @ %d %0.4f %d %0.4f" % (last_info[str(userid)][1].rstrip(), userid, lat_dd, lat_min, long_ddd, long_min )

    f = open( sig_file, 'w' )
    f.write( "---[%s]---\n" % (s.center(62)) )
    f.close()


# Connect to the AIS Stream Source
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(ais_source)
    sf = s.makefile('r')
except:
    sys.stderr.write("Error connecting to %s:%d\n" % ais_source)
    raise

# Setup for AIS Parser
ais_state = aisparser.ais_state()

# Open a shelf to keep track of the MMSI to Name mappings
last_info = shelve.open( db_file )

done = 0
try:
    while not done:
        # Read AIS messages from the source
        data = sf.readline()

        # Reassemble AIS messages
        result = aisparser.assemble_vdm( ais_state, data )
        if( result == 0):
            # Get the msgid from the first 6 bits of the message
            ais_state.msgid = aisparser.get_6bit( ais_state.six_state, 6 )
            if debug: print "msgid = %d" % (ais_state.msgid)

            # Parse based on msgid
            if ais_state.msgid == 1:
               # Message 1 Class
               msg = aisparser.aismsg_1()

               # Parse the message 1 into msg
               aisparser.parse_ais_1( ais_state, msg )

               if debug:
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               # If we have name information, write the position to a file
               if last_info.has_key(str(msg.userid)):
                   write_last(msg.userid,msg.latitude,msg.longitude)

            elif ais_state.msgid == 2:
               msg = aisparser.aismsg_2()
               aisparser.parse_ais_2( ais_state, msg )

               if debug:
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               if last_info.has_key(str(msg.userid)):
                   write_last(msg.userid,msg.latitude,msg.longitude)

            elif ais_state.msgid == 3:
               msg = aisparser.aismsg_3()
               aisparser.parse_ais_3( ais_state, msg )

               if debug:
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               if last_info.has_key(str(msg.userid)):
                   write_last(msg.userid,msg.latitude,msg.longitude)

            elif ais_state.msgid == 4:
               msg = aisparser.aismsg_4()
               aisparser.parse_ais_4( ais_state, msg )

               if debug:
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               if last_info.has_key(str(msg.userid)):
                   write_last(msg.userid,msg.latitude,msg.longitude)

            elif ais_state.msgid == 5:
               msg = aisparser.aismsg_5()
               aisparser.parse_ais_5( ais_state, msg )

               if debug:
                   print "mmsi       : %d" % (msg.userid)
                   print "callsign   : %s" % (msg.callsign)
                   print "name       : %s" % (msg.name)
                   print "destination: %s" % (msg.dest)

               # Clean up any '@' characters
               callsign = msg.callsign.replace('@', ' ')
               name = msg.name.replace('@', ' ')
               dest = msg.dest.replace('@', ' ')

               # Add the name info to the shelf database
               last_info[str(msg.userid)] = (callsign,name,dest)

            elif ais_state.msgid == 18:
               msg = aisparser.aismsg_18()
               aisparser.parse_ais_18( ais_state, msg )

               if debug:
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               if last_info.has_key(str(msg.userid)):
                   write_last(msg.userid,msg.latitude,msg.longitude)

            elif ais_state.msgid == 19:
               msg = aisparser.aismsg_19()
               aisparser.parse_ais_19( ais_state, msg )

               if debug:
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)
                   print "name       : %s" % (msg.name)

               # Clean up any '@' characters
               name = msg.name.replace('@', ' ')

               # Add the name info to the shelf database
               last_info[str(msg.userid)] = (None,name,None)

            elif ais_state.msgid == 24:
               msg = aisparser.aismsg_24()
               aisparser.parse_ais_24( ais_state, msg )

               if debug:
                   print "mmsi     : %d" % (msg.userid)
                   print "callsign   : %s" % (msg.callsign)
                   print "name       : %s" % (msg.name)

               # Clean up any '@' characters
               callsign = msg.callsign.replace('@', ' ')
               name = msg.name.replace('@', ' ')

               # Add the name info to the shelf database
               last_info[str(msg.userid)] = (callsign,name,None)
except:
    s.close()
    last_info.close()

    raise
