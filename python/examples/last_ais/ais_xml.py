#!/usr/bin/python

""" Read AIS data from a TCP/IP port and write a sigfile and XML data for
a Google Map website.
"""
import os,sys
import aisparser
import socket
import shelve
import time
import pprint

debug = 0

# AIS Data Source
ais_source = ('collie.home', 3050)
sig_file = '/tmp/last_ais.txt'
xml_file = '/tmp/data.xml'
xml_update = 60
MAX_AGE = 60 * 60 * 4

months = [ "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
           "Sep", "Oct", "Nov", "Dec" ]

def calc_eta( eta ):
    min  = eta & 0x3F
    hour = (eta >> 6) & 0x1F
    day  = (eta >> 11) & 0x1F
    month= (eta >> 16) & 0x0F
    return( month, day, hour, min )


def calc_vector( last_pos, current_pos ):
    if current_pos and last_pos:
        current_pos.dy = (last_pos.latitude - current_pos.latitude) / 600000.0
        current_pos.dx = (last_pos.longitude - current_pos.longitude) / 600000.0
    else:
        current_pos.dx = 0.0
        current_pos.dy = 0.0


def write_last( (info, pos, last_update) ):
    # Check for invalid positions
    if (pos.latitude == 54600000) or (pos.longitude == 108600000):
        return

    if info == None or pos == None:
        return

    try:
        # Convert position
        (status,lat_dd,lat_min,long_ddd,long_min) = aisparser.pos2dmm( pos.latitude, pos.longitude );

    #    s = "%s (%s) @ %d %0.4f %d %0.4f" % (last_info[str(userid)][1].rstrip(), userid, lat_dd, lat_min, long_ddd, long_min )
        s = "%s (%s) @ %d %0.4f %d %0.4f" % (info.name.rstrip(), info.userid, lat_dd, lat_min, long_ddd, long_min )

        f = open( sig_file, 'w' )
        f.write( "---[%s]---\n" % (s.center(62)) )
        f.close()
    except:
        sys.stderr.write('Error writing sigfile\n')
        raise

def write_xml( ships ):
    """ Update the XML file with the most recent ship positions
    """
#    pp = pprint.PrettyPrinter(indent=4)
#    pp.pprint(ships)

    f = open( xml_file, 'w' )
    f.write( "<markers>\n" )

    for mmsi in ships:
        info, pos, last_update = ships[mmsi]
        if (pos != None) and (last_update > time.time() - MAX_AGE):
            # Skip unknown positions
            if (pos.latitude == 54600000) or (pos.longitude == 108600000):
                continue

            (status,lat_dd,long_ddd) = aisparser.pos2ddd( pos.latitude, pos.longitude );
            if debug: print "%d %s : %0.4f %0.4f" % (ord(pos.msgid),pos.userid,lat_dd,long_ddd)

            if info == None:
                name = mmsi
                callsign = ""
                dest = ""
                ship_type = 0
                imo = ""
                eta = ""
                length = 0
                width = 0
                draught = 0
            else:
                imo = str(info.imo)
                name = info.name
                callsign = info.callsign
                dest = info.dest
                ship_type = info.ship_type
                (month, day, hour, minute) = calc_eta( info.eta )
                if (month, day, hour, minute) == (0, 0, 24, 60):
                    eta = "NA (%d)" % (info.eta)
                else:
                    try:
                        month = months[month]
                    except:
                        month = ""
                    eta = "%s %d %d:%d (%d)" % (month, day, hour, minute, info.eta)
                length = info.dim_bow + info.dim_stern
                width = ord(info.dim_port) + ord(info.dim_starboard)
                draught = info.draught

            # Base Station, no speed, course or status
            if ord(pos.msgid) == 4:
                pos.sog = 0
                pos.cog = 0
                pos.nav_status = "\0"

#            pp = pprint.PrettyPrinter(indent=4)
#            print ">>>>>>>>"
#            pp.pprint(pos)
#            print ">>>>>>>>"

            # mmsi!imo!callsign!destination!eta!speed!course!status!length!width!draft!mtime
            ais_info = ""

            try:
                ais_info = "%s!%s!%s!%s!%s!%0.1f!" % (mmsi,imo,callsign,dest,eta,pos.sog/10.0)
                ais_info += "%0.1f!%d!" % (pos.cog/10.0,ord(pos.nav_status))
                ais_info += "%d!%d!%0.1f!%s" % (length, width, draught/10.0,time.asctime(time.gmtime(last_update)))

            except:
#                pp = pprint.PrettyPrinter(indent=4)
#                pp.pprint(info)
                raise

            f.write( "<marker lat='%0.4f' lon='%0.4f' name='%s' dx='%0.4f' dy='%0.4f' type='%d' ais='%s'/>\n" % (lat_dd,long_ddd,name,pos.dx,pos.dy,ship_type,ais_info))
        else:
            print "Skipped %s - %s" % (mmsi,time.asctime(time.gmtime(last_update)))

    f.write( "</markers>\n" )
    f.close()

    # Copy to live website
    os.system("scp /tmp/data.xml aisparser@neil.home:/home/aisparser/live_html/")

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

# Retrieve the last ship info
#last_info = shelve.open( '/tmp/last_ship' )
last_info = {}
last_xml = time.time() - 50

done = 0
try:
    while not done:
        # Read AIS messages from the source
        data = sf.readline()

        # Parse the data
        result = aisparser.assemble_vdm( ais_state, data )
        if( result == 0):
            ais_state.msgid = aisparser.get_6bit( ais_state.six_state, 6 )
            if debug: print "msgid = %d" % (ais_state.msgid)

            # Message 1: Position Update
            if ais_state.msgid == 1:
               msg = aisparser.aismsg_1()
               aisparser.parse_ais_1( ais_state, msg )
               msg.msgid = 1

               if debug:
                   print "msgid    : %d" % (ord(msg.msgid))
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               # Update the ship info
               if last_info.has_key(str(msg.userid)):
                   info, last_pos, null = last_info[str(msg.userid)]
                   calc_vector( last_pos, msg )
               else:
                   info = None
                   msg.dx = 0.0
                   msg.dy = 0.0
               last_info[str(msg.userid)] = (info,msg,time.time())

               write_last(last_info[str(msg.userid)])

            # Message 2: Position Update
            elif ais_state.msgid == 2:
               msg = aisparser.aismsg_2()
               aisparser.parse_ais_2( ais_state, msg )
               msg.msgid = 2

               if debug:
                   print "msgid    : %d" % (ord(msg.msgid))
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               # Update the ship info
               if last_info.has_key(str(msg.userid)):
                   info, last_pos, null = last_info[str(msg.userid)]
                   calc_vector( last_pos, msg )
               else:
                   info = None
                   msg.dx = 0.0
                   msg.dy = 0.0
               last_info[str(msg.userid)] = (info,msg,time.time())

               write_last(last_info[str(msg.userid)])

            # Message 3: Position Update
            elif ais_state.msgid == 3:
               msg = aisparser.aismsg_3()
               aisparser.parse_ais_3( ais_state, msg )
               msg.msgid = 3

               if debug:
                   print "msgid    : %d" % (ord(msg.msgid))
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               # Update the ship info
               if last_info.has_key(str(msg.userid)):
                   info, last_pos, null = last_info[str(msg.userid)]
                   calc_vector( last_pos, msg )
               else:
                   info = None
                   msg.dx = 0.0
                   msg.dy = 0.0
               last_info[str(msg.userid)] = (info,msg,time.time())

               write_last(last_info[str(msg.userid)])

            # Message 4: Base Station Report
            elif ais_state.msgid == 4:
               msg = aisparser.aismsg_4()
               aisparser.parse_ais_4( ais_state, msg )
               msg.msgid = 4

               if debug:
                   print "msgid    : %d" % (ord(msg.msgid))
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               # Update the ship info
               if last_info.has_key(str(msg.userid)):
                   info, last_pos, null = last_info[str(msg.userid)]
                   calc_vector( last_pos, msg )
               else:
                   info = None
                   msg.dx = 0.0
                   msg.dy = 0.0
               last_info[str(msg.userid)] = (info,msg,time.time())

               write_last(last_info[str(msg.userid)])

            # Message 5: Ship Static and Voyage Data
            elif ais_state.msgid == 5:
               msg = aisparser.aismsg_5()
               aisparser.parse_ais_5( ais_state, msg )
               msg.msgid = 5

               if debug:
                   print "msgid    : %d" % (ord(msg.msgid))
                   print "mmsi       : %d" % (msg.userid)
                   print "callsign   : %s" % (msg.callsign)
                   print "name       : %s" % (msg.name)
                   print "destination: %s" % (msg.dest)

               # Clean up any '@' characters
               msg.callsign = msg.callsign.replace('@', ' ')
               msg.name = msg.name.replace('@', ' ')
               msg.dest = msg.dest.replace('@', ' ')

               # Update the ship info
               if last_info.has_key(str(msg.userid)):
                   null, pos, null = last_info[str(msg.userid)]
               else:
                   pos = None
               last_info[str(msg.userid)] = (msg,pos,time.time())

            # Message 18: Class B Position Report
            elif ais_state.msgid == 18:
               msg = aisparser.aismsg_18()
               aisparser.parse_ais_18( ais_state, msg )
               msg.msgid = 18

               if debug:
                   print "msgid    : %d" % (ord(msg.msgid))
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)

               # Update the ship info
               if last_info.has_key(str(msg.userid)):
                   info, last_pos, null = last_info[str(msg.userid)]
                   calc_vector( last_pos, msg )
               else:
                   info = None
                   msg.dx = 0.0
                   msg.dy = 0.0
               last_info[str(msg.userid)] = (info,msg,time.time())

               write_last(last_info[str(msg.userid)])

            # Message 19: Class B Position + Ship Name
            elif ais_state.msgid == 19:
               msg = aisparser.aismsg_19()
               aisparser.parse_ais_19( ais_state, msg )
               msg.msgid = 19

               if debug:
                   print "msgid    : %d" % (ord(msg.msgid))
                   print "mmsi     : %d" % (msg.userid)
                   print "latitude : %d" % (msg.latitude)
                   print "longitude: %d" % (msg.longitude)
                   print "name       : %s" % (msg.name)

               name = msg.name.replace('@', ' ')
#               last_info[str(msg.userid)] = (None,name,None)

            # Message 24: Class B ship Name
            elif ais_state.msgid == 24:
               msg = aisparser.aismsg_24()
               aisparser.parse_ais_24( ais_state, msg )
               msg.msgid = 24

               if debug:
                   print "msgid    : %d" % (ord(msg.msgid))
                   print "mmsi       : %d" % (msg.userid)
                   print "callsign   : %s" % (msg.callsign)
                   print "name       : %s" % (msg.name)

               callsign = msg.callsign.replace('@', ' ')
               name = msg.name.replace('@', ' ')
#               last_info[str(msg.userid)] = (callsign,name,None)

        # Is it time to update the XML?
        if time.time() > last_xml + xml_update:
            write_xml( last_info )
            last_xml = time.time()

except:
    s.close()
#    last_info.close()

    raise
    