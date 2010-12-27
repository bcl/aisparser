{ ---------------------------------------------------------------------------- }
{ Delphi Implementation of AISParser Library                                   }
{ Copyright 2006 by Brian C. Lane <bcl@brianlane.com>                          }
{ All Rights Reserved                                                          }
{                                                                              }
{ Delphi conversion by: Chris Krohn <ckrohn@caymanport.com>                    }
{                                                                              }
{                                                                              }
{  AIVDM/AIVDO AIS Message Parser                                              }
{  <center>Copyright 2006 by Brian C. Lane <bcl@brianlane.com><br>             }
{  http://www.aisparser.com/                                                   }
{                                                                              }
{  The Automatic Identification System (AIS) allows ships to be tracked in     }
{  realtime based on information transmitted by each ship. They are equipped   }
{  with either a Class A or Class B AIS transponder which includes a GPS       }
{  for accurate position and speed reporting. AIS receivers decode the         }
{  transmitted information and output the data as AIVDM messages.              }
{                                                                              }
{  The structure of the AIVDM message is described in IEC 61993-2 and it is    }
{  a variation of the NMEA 0183 sentence format that includes the raw data     }
{  encoded in a 6-bit format. A Message 1 example looks like this:             }
{                                                                              }
{  \code                                                                       }
{  !AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27                             }
{  \endcode                                                                    }
{                                                                              }
{  The meaning of each data element in the AIS messages is covered by the      }
{  ITU M.1371 and IEC 62287 documents. These documents are available from      }
{  the websites of these organizations:                                        }
{    - http://www.itu.int                                                      }
{    - http://www.iec.ch                                                       }
{    - http://www.navcenter.org/marcomms/IEC/                                  }
{                                                                              }
{  To fully understand the data produced by this library you should read       }
{  the above documents.                                                        }
{                                                                              }
{  This library allows you to write programs that accept ASCII AIVDM or        }
{  AIVDO messages and parse their packed 6-bit data into structures            }
{  containging the message data. The general flow of a typical aisparser       }
{  program would look like this:                                               }
{                                                                              }
{   - Receive ASCII data (via serial port, file or network socket)             }
{   - Pass the data to assemble_vdm() to reassemble multipart messages and     }
{     extract their 6-bit data.                                                }
{   - Fetch the first 6 bits from the data stream, this is the message id      }
{     and determines which parser function to call and what structure          }
{     to store the results in.                                                 }
{   - Call the appropriate parse_ais_XX function with aismsg_XX structure      }
{     passed to it.                                                            }
{   - Act on the message data: display a position, ship name, etc.             }
{                                                                              }
{  This library is thread safe, all variables used are contained in the        }
{  ais_state structure. To support multiple data streams all you need to       }
{  do is used 2 different instances of ais_state.                              }
{                                                                              }
{  To get started you should read the documentation on the vdm_parse.c         }
{  module and look at the example code in the example directory of the         }
{  source distribution.                                                        }
{                                                                              }
{ ---------------------------------------------------------------------------- }
Unit AISParser;
                                                                               
Interface                                                                      
                                                                               
Uses Windows, Classes, SysUtils, StrUtils, Dialogs;
                                                                               
Const
  MAX_NMEA_LENGTH = 255;
  MAX_NMEA_FIELDS = 50;
  NMEA_START = 0;
  NMEA_END = 1;
  NMEA_DONE = 2;

  SIXBIT_LEN = 255;

  SIXBUF_LEN = 255;

  pow2_mask: Array[0..6] Of Byte = ($00, $01, $03, $07, $0F, $1F, $3F);

Type
  { NMEA parser state structure }
  TNMEA_State = Record
    search: Byte;                                     //!< State of the search: START, END or DONE
    field: Array[0..MAX_NMEA_FIELDS-1] of PChar;      //!< Pointers to fields in the buffer
    str: Array[0..MAX_NMEA_LENGTH-1] of Char;         //!< Incoming NMEA 0183 string
    str_len: LongInt;                                 //!< Number of bytes in str
  end {nmea_state};

  { sixbit state }
  { The size of bits is enough to handle a little over 5 slots of data }
  { ((5* 256) / 6) = 214 }
  TSixBit = Record
    bits: Array[0..SIXBIT_LEN-1] of Char;             //!< raw 6-bit ASCII data string
    p: PChar;                                         //!< pointer to current character in bits
    remainder: Byte;                                  //!< Remainder bits
    remainder_bits: Byte;                             //!< Number of remainder bits
  end {sixbit};

  { AIS Message 1 - Position Report with SOTDMA }
  TAISMsg_1 = Record
    msgid: SmallInt;                   //!< 6 bits  : Message ID (1)
    isrepeat: SmallInt;                //!< 2 bits  : Repeated
    userid: LongInt;                   //!< 30 bits : UserID / MMSI
    nav_status: SmallInt;              //!< 4 bits  : Navigational Status
    rot: Byte;                         //!< 8 bits  : Rate of Turn
    sog: Integer;                      //!< 10 bits : Speed Over Ground
    pos_acc: SmallInt;                 //!< 1 bit   : Position Accuracy
    longitude: LongInt;                //!< 28 bits : Longitude in 1/10000 minute
    latitude: LongInt;                 //!< 27 bits : Latitude in 1/10000 minute
    cog: Integer;                      //!< 12 bits : Course over Ground
    true: Integer;                     //!< 9 bits  : True heading
    utc_sec: SmallInt;                 //!< 6 bits  : UTC Seconds
    regional: SmallInt;                //!< 4 bits  : Regional bits
    spare: SmallInt;                   //!< 1 bit   : Spare
    raim: SmallInt;                    //!< 1 bit   : RAIM flag
    sync_state: SmallInt;              //!< 2 bits  : SOTDMA sync state
    slot_timeout: SmallInt;            //!< 3 bits  : SOTDMA Slot Timeout
    sub_message: Integer;              //!< 14 bits : SOTDMA sub-message
  end {aismsg_1};

  { AIS Message 2 - Position Report with SOTDMA }
  TAISMsg_2 = Record
    msgid: SmallInt;                   //!< 6 bits  : Message ID (2)
    isrepeat: SmallInt;                //!< 2 bits  : Repeated
    userid: LongInt;                   //!< 30 bits : UserID / MMSI
    nav_status: SmallInt;              //!< 4 bits  : Navigational Status
    rot: Byte;                         //!< 8 bits  : Rate of Turn
    sog: Integer;                      //!< 10 bits : Speed Over ground
    pos_acc: SmallInt;                 //!< 1 bit   : Position Accuracy
    longitude: LongInt;                //!< 28 bits : Longitude in 1/10000 minute
    latitude: LongInt;                 //!< 27 bits : Latitude in 1/10000 minute
    cog: Integer;                      //!< 12 bits : Course over Ground
    true: Integer;                     //!< 9 bits  : True Heading
    utc_sec: SmallInt;                 //!< 6 bits  : UTC Seconds
    regional: SmallInt;                //!< 4 bits  : Regional bits
    spare: SmallInt;                   //!< 1 bit   : Spare
    raim: SmallInt;                    //!< 1 bit   : RAIM flag
    sync_state: SmallInt;              //!< 2 bits  : SOTDMA sync state
    slot_timeout: SmallInt;            //!< 3 bits  : SOTDMA Slot Timeout
    sub_message: Integer;              //!< 14 bits : SOTDMA sub-message
  end {aismsg_2};

  { AIS Message 3 - Position Report with ITDMA }
  TAISMsg_3 = Record
    msgid: SmallInt;                   //!< 6 bits  : MessageID (3)
    isrepeat: SmallInt;                //!< 2 bits  : Repeated
    userid: LongInt;                   //!< 30 bits : UserID / MMSI
    nav_status: SmallInt;              //!< 4 bits  : Navigational Status
    rot: Byte;                         //!< 8 bits  : Rate of Turn
    sog: Integer;                      //!< 10 bits : Speed Over Ground
    pos_acc: SmallInt;                 //!< 1 bit   : Position Accuracy
    longitude: LongInt;                //!< 28 bits : Longitude in 1/10000 minute
    latitude: LongInt;                 //!< 27 bits : Latitude in 1/10000 minute
    cog: Integer;                      //!< 12 bits : Course over Ground
    true: Integer;                     //!< 9 bits  : True Heading
    utc_sec: SmallInt;                 //!< 6 bits  : UTC Seconds
    regional: SmallInt;                //!< 4 bits  : Regional bits
    spare: SmallInt;                   //!< 1 bit   : Spare
    raim: SmallInt;                    //!< 1 bit   : RAIM Flag
    sync_state: SmallInt;              //!< 2 bits  : ITDMA sync state
    slot_increment: Integer;           //!< 13 bits : ITDMA Slot Increment
    num_slots: SmallInt;               //!< 3 bits  : ITDMA Number of Slots
    keep: SmallInt;                    //!< 1 bit   : ITDMA Keep Flag
  end {aismsg_3};

  { AIS Message 4 - Base Station Report }
  TAISMsg_4 = Record
    msgid: SmallInt;                   //!< 6 bits  : MessageID (4)
    isrepeat: SmallInt;                //!< 2 bits  : Repeated
    userid: LongInt;                   //!< 30 bits : UserID / MMSI
    utc_year: Integer;                 //!< 14 bits : UTC Year
    utc_month: SmallInt;               //!< 4 bits  : UTC Month
    utc_day: SmallInt;                 //!< 5 bits  : UTC Day
    utc_hour: SmallInt;                //!< 5 bits  : UTC Hour
    utc_minute: SmallInt;              //!< 6 bits  : UTC Minute
    utc_second: SmallInt;              //!< 6 bits  : UTC Second
    pos_acc: SmallInt;                 //!< 1 bit   : Position Accuracy
    longitude: LongInt;                //!< 28 bits : Longitude in 1/10000 minute
    latitude: LongInt;                 //!< 27 bits : Latitude in 1/10000 minute
    pos_type: SmallInt;                //!< 4 bits  : Type of position fixing device
    spare: Integer;                    //!< 10 bits : Spare
    raim: SmallInt;                    //!< 1 bit   : RAIM flag
    sync_state: SmallInt;              //!< 2 bits  : SOTDMA sync state
    slot_timeout: SmallInt;            //!< 3 bits  : SOTDMA Slot Timeout
    sub_message: Integer;              //!< 14 bits : SOTDMA sub-message
  end {aismsg_4};

  { AIS Message 5 - Ship static and voyage related data }
  TAISMsg_5 = Record
    msgid: SmallInt;                   //!< 6 bits  : MessageID (5)
    isrepeat: SmallInt;                //!< 2 bits  : Repeated
    userid: LongInt;                   //!< 30 bits : UserID / MMSI
    version: SmallInt;                 //!< 2 bits  : AIS Version
    imo: LongInt;                      //!< 30 bits : IMO Number
    callsign: Array[0..7] of Char;     //!< 7x6 (42) bits : Callsign
    name: Array[0..20] of Char;        //!< 20x6 (120) bits : Ship Name
    ship_type: Byte;                   //!< 8 bits  : Type of Ship and Cargo
    dim_bow: Integer;                  //!< 9 bits  : GPS Ant. Distance from Bow
    dim_stern: Integer;                //!< 9 bits  : GPS Ant. Distance from stern
    dim_port: SmallInt;                //!< 6 bits  : GPS Ant. Distance from port
    dim_starboard: SmallInt;           //!< 6 bits  : GPS Ant. Distance from starboard
    pos_type: SmallInt;                //!< 4 bits  : Type of position fixing device
    eta: LongInt;                      //!< 20 bits : Estimated Time of Arrival MMDDHHMM
    draught: Byte;                     //!< 8 bits  : Maximum present static draught
    dest: Array[0..20] of Char;        //!< 6x20 (120) bits : Ship Destination
    dte: SmallInt;                     //!< 1 bit   : DTE flag
    spare: SmallInt;                   //!< 1 bit   : spare
  end {aismsg_5};

  { AIS Message 6 - Addressed Binary Message }
  TAISMsg_6 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (6)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    sequence: SmallInt;                //!< 2 bits   : Sequence number
    destination: LongInt;              //!< 30 bits  : Destination MMSI
    retransmit: SmallInt;              //!< 1 bit    : Retransmit
    spare: SmallInt;                   //!< 1 bit    : Spare
    app_id: Word;                      //!< 16 bits  : Application ID
    data: Array[0..154] of SmallInt;   //!< 960 bits : Data payload (in 6-bit ASCII)
  end {aismsg_6};

  { AIS Message 7 - Binary Acknowledge }
  TAISMsg_7 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (7)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare: SmallInt;                   //!< 2 bits   : Spare
    destid_1: LongInt;                 //!< 30 bits  : Destination MMSI 1
    sequence_1: SmallInt;              //!< 2 bits   : Sequence Number 1
    destid_2: LongInt;                 //!< 30 bits  : Destination MMSI 2                                          
    sequence_2: SmallInt;              //!< 2 bits   : Sequence Number 2
    destid_3: LongInt;                 //!< 30 bits  : Destination MMSI 3
    sequence_3: SmallInt;              //!< 2 bits   : Sequence Number 3                                          
    destid_4: LongInt;                 //!< 30 bits  : Destination MMSI 4
    sequence_4: SmallInt;              //!< 2 bits   : Sequence Number 4
    num_acks: SmallInt;                //!< Number of acks                                          
  end {aismsg_7};

  { AIS Message 8 - Binary Broadcast Message }
  TAISMsg_8 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (8)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare: SmallInt;                   //!< 2 bits   : Spare
    app_id: Word;                      //!< 16 bits  : Application ID
    data: Array[0..160] of SmallInt;   //!< 952 bits : Data payload (in 6-bit ASCII)
  end {aismsg_8};

  Tsotdma = Record
    sync_state: SmallInt;              //!< 2 bits   : SOTDMA Sync State
    slot_timeout: SmallInt;            //!< 3 bits   : SOTDMA Slot Timeout
    sub_message: Integer;              //!< 14 bits  : SOTDMA Sub-Messsage
  End {sotdma};

  Titdma = Record
    sync_state: SmallInt;              //!< 2 bits   : ITDMA Sync State
    slot_inc: Integer;                 //!< 13 bits  : ITDMA Slot Increment
    num_slots: SmallInt;               //!< 3 bits   : ITDMA Number of Slots
    keep_flag: SmallInt;               //!< 1 bit    : ITDMA Keep Flag
  End {itdma};

  { AIS Message 9 - Standard SAR Aircraft position report }
  TAISMsg_9 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (9)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    altitude: Integer;                 //!< 12 bits  : Altitude
    sog: Integer;                      //!< 10 bits  : Speed Over Ground
    pos_acc: SmallInt;                 //!< 1 bit    : Position Accuracy
    longitude: LongInt;                //!< 28 bits  : Longitude in 1/10000 minute
    latitude: LongInt;                 //!< 27 bits  : Latitude in 1/10000 minute
    cog: Integer;                      //!< 12 bits  : Course Over Ground
    utc_sec: SmallInt;                 //!< 6 bits   : UTC Seconds
    regional: Byte;                    //!< 8 bits   : Regional bits
    dte: SmallInt;                     //!< 1 bit    : DTE flag
    spare: SmallInt;                   //!< 3 bits   : Spare
    assigned: SmallInt;                //!< 1 bit    : Assigned mode flag
    raim: SmallInt;                    //!< 1 bit    : RAIM flag
    comm_state: SmallInt;              //!< 1 bit    : Comm State Flag
    sotdma: Tsotdma;
    itdma: Titdma;
  End {aismsg_9};

  { AIS Message 10 - UTC and Date inquiry }
  TAISMsg_10 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (10)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare1: SmallInt;                  //!< 2 bits   : Spare
    destination: LongInt;              //!< 30 bits  : Destination MMSI
    spare2: SmallInt;                  //!< 2 bits   : Spare
  end {aismsg_10};

  { AIS Message 11 - UTC and Date response }
  TAISMsg_11 = Record
    msgid: SmallInt;                   //!< 6 bits  : MessageID (4)
    isrepeat: SmallInt;                //!< 2 bits  : Repeated
    userid: LongInt;                   //!< 30 bits : UserID / MMSI
    utc_year: Integer;                 //!< 14 bits : UTC Year
    utc_month: SmallInt;               //!< 4 bits  : UTC Month
    utc_day: SmallInt;                 //!< 5 bits  : UTC Day
    utc_hour: SmallInt;                //!< 5 bits  : UTC Hour
    utc_minute: SmallInt;              //!< 6 bits  : UTC Minute
    utc_second: SmallInt;              //!< 6 bits  : UTC Second
    pos_acc: SmallInt;                 //!< 1 bit   : Position Accuracy
    longitude: LongInt;                //!< 28 bits : Longitude in 1/10000 minute
    latitude: LongInt;                 //!< 27 bits : Latitude in 1/10000 minute                                          
    pos_type: SmallInt;                //!< 4 bits  : Type of position fixing device                                          
    spare: Integer;                    //!< 10 bits : Spare                                          
    raim: SmallInt;                    //!< 1 bit   : RAIM flag                                          
    sync_state: SmallInt;              //!< 2 bits  : SOTDMA sync state                                          
    slot_timeout: SmallInt;            //!< 3 bits  : SOTDMA Slot Timeout
    sub_message: Integer;              //!< 14 bits : SOTDMA sub-message                                          
  end {aismsg_11};

  { AIS Message 12 - Addressed safety related message }
  TAISMsg_12 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (12)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    sequence: SmallInt;                //!< 2 bits   : Sequence                                          
    destination: LongInt;              //!< 30 bits  : Destination MMSI                                          
    retransmit: SmallInt;              //!< 1 bit    : Retransmit                                          
    spare: SmallInt;                   //!< 1 bit    : Spare
    msg: Array[0..157] Of Char;        //!< 936 bits : Message in ASCII
  end {aismsg_12};

  { AIS Message 13 - Safety related Acknowledge }
  TAISMsg_13 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (7)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare: SmallInt;                   //!< 2 bits   : Spare
    destid_1: LongInt;                 //!< 30 bits  : Destination MMSI 1
    sequence_1: SmallInt;              //!< 2 bits   : Sequence Number 1
    destid_2: LongInt;                 //!< 30 bits  : Destination MMSI 2
    sequence_2: SmallInt;              //!< 2 bits   : Sequence Number 2
    destid_3: LongInt;                 //!< 30 bits  : Destination MMSI 3
    sequence_3: SmallInt;              //!< 2 bits   : Sequence Number 3
    destid_4: LongInt;                 //!< 30 bits  : Destination MMSI 4
    sequence_4: SmallInt;              //!< 2 bits   : Sequence Number 4
    num_acks: SmallInt;                //!< Number of acks
  end {aismsg_13};

  { AIS Message 14 - Safety related Broadcast Message }
  TAISMsg_14 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (14)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare: SmallInt;                   //!< 2 bits   : Spare
    msg: Array[0..163] Of Char;        //!< 968 bits : Message in ASCII
  end {aismsg_14};

  { AIS Message 15 - Interrogation }
  TAISMsg_15 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (15)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare1: SmallInt;                  //!< 2 bits   : Spare
    destid1: LongInt;                  //!< 30 bits  : Destination MMSI 1
    msgid1_1: SmallInt;                //!< 6 bits   : MessageID 1.1
    offset1_1: Integer;                //!< 12 bits  : Slot Offset 1.1
    spare2: SmallInt;                  //!< 2 bits   : Spare
    msgid1_2: SmallInt;                //!< 6 bits   : MessageID 1.2
    offset1_2: Integer;                //!< 12 bits  : Slot Offset 1.2
    spare3: SmallInt;                  //!< 2 bits   : Spare
    destid2: LongInt;                  //!< 30 bits  : Destination MMSI 2
    msgid2_1: SmallInt;                //!< 6 bits   : MessageID 2.1
    offset2_1: Integer;                //!< 12 bits  : Slot Offset 2.1
    spare4: SmallInt;                  //!< 2 bits   : Spare
    num_reqs: SmallInt;                //!< Number of interrogation requests
  end {aismsg_15};

  { AIS Message 16 - Assigned Mode Command }
  TAISMsg_16 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (16)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare1: SmallInt;                  //!< 2 bits   : Spare
    destid_a: LongInt;                 //!< 30 bits  : Destination MMSI A
    offset_a: Integer;                 //!< 12 bits  : Slot Offset A
    increment_a: Integer;              //!< 10 bits  : Increment A
    destid_b: LongInt;                 //!< 30 bits  : Destination MMSI B
    offset_b: Integer;                 //!< 12 bits  : Slot Offset B                                          
    increment_b: Integer;              //!< 10 bits  : Increment B                                          
    spare2: SmallInt;                  //!< 4 bits   : Spare
    num_cmds: SmallInt;                //!< Number of commands received
  end {aismsg_16};

  { AIS Message 17 - GNSS Binary Broadcast Message }
  TAISMsg_17 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (17)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare1: SmallInt;                  //!< 2 bits   : Spare
    longitude: LongInt;                //!< 18 bits  : Longiture in 1/1000 minute
    latitude: LongInt;                 //!< 17 bits  : Latitude in 1/1000 minute
    spare2: SmallInt;                  //!< 5 bits   : Spare
    msg_type: SmallInt;                //!< 6 bits   : Mesage Type from M.823
    station_id: Integer;               //!< 10 bits  : Station ID from M.823                                          
    z_count: Integer;                  //!< 13 bits  : Z Count                                          
    seq_num: SmallInt;                 //!< 3 bits   : Sequence Number                                          
    num_words: SmallInt;               //!< 5 bits   : Number of Data Words
    health: SmallInt;                  //!< 3 bits   : Reference Station Health from M.823
    data: Array[0..117] Of SmallInt;   //!< 0-696 bits  : Data in 6-bit format
  end {aismsg_17};

  { AIS Message 18 - Standard Class B CS Position Report }
  { From IEC 62287. This differs slightly from the original }
  { message, some of the regional bits were changed to flags. }
  TAISMsg_18 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (18)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    regional1: Byte;                   //!< 8 bits   : Regional Bits
    sog: Integer;                      //!< 10 bits  : Speed Over Ground
    pos_acc: SmallInt;                 //!< 1 bit    : Position Accuracy
    longitude: LongInt;                //!< 28 bits  : Longitude in 1/10000 minute
    latitude: LongInt;                 //!< 27 bits  : Latitude in 1/10000 minute
    cog: Integer;                      //!< 12 bits  : Course Over Ground
    true: Integer;                     //!< 9 bits   : True Heading
    utc_sec: SmallInt;                 //!< 6 bits   : UTC Seconds
    regional2: SmallInt;               //!< 2 bits   : Regional Bits
    unit_flag: SmallInt;               //!< 1 bit    : Class B CS Flag
    display_flag: SmallInt;            //!< 1 bit    : Integrated msg14 Display Flag
    dsc_flag: SmallInt;                //!< 1 bit    : DSC Capability flag
    band_flag: SmallInt;               //!< 1 bit    : Marine Band Operation Flag
    msg22_flag: SmallInt;              //!< 1 bit    : Msg22 Frequency Management Flag
    mode_flag: SmallInt;               //!< 1 bit    : Autonomous Mode Flag
    raim: SmallInt;                    //!< 1 bit    : RAIM Flag
    comm_state: SmallInt;              //!< 1 bit    : Comm State Flag
    sotdma: Tsotdma;
    itdma: Titdma;
  End {aismsg_18};

  { AIS Message 19 - Extended Class B Equipment Position Report }
  TAISMsg_19 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (19)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    regional1: SmallInt;               //!< 8 bits   : Regional Bits
    sog: Integer;                      //!< 10 bits  : Speed Over Ground
    pos_acc: SmallInt;                 //!< 1 bit    : Position Accuracy
    longitude: LongInt;                //!< 28 bits  : Longitude in 1/10000 minute
    latitude: LongInt;                 //!< 27 bits  : Latitude in 1/10000 minute
    cog: Integer;                      //!< 12 bits  : Course Over Ground
    true: Integer;                     //!< 9 bits   : True Heading
    utc_sec: SmallInt;                 //!< 6 bits   : UTC Seconds
    regional2: SmallInt;               //!< 4 bits   : Regional Bits
    name: Array[0..20] Of Char;        //!< 120 bits : Ship Name in ASCII
    ship_type: Byte;                   //!< 8 bits   : Type of Ship and Cargo
    dim_bow: Integer;                  //!< 9 bits   : GPS Ant. Distance from Bow
    dim_stern: Integer;                //!< 9 bits   : GPS Ant. Distance from Stern
    dim_port: SmallInt;                //!< 6 bits   : GPS Ant. Distance from Port
    dim_starboard: SmallInt;           //!< 6 bits   : GPS Ant. Distance from Starboard
    pos_type: SmallInt;                //!< 4 bits   : Type of Position Fixing Device
    raim: SmallInt;                    //!< 1 bit    : RAIM Flag
    dte: SmallInt;                     //!< 1 bit    : DTE Flag
    spare: SmallInt;                   //!< 5 bits   : Spare
  end {aismsg_19};

  { AIS Message 20 - Data Link Management }
  TAISMsg_20 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (20)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare1: SmallInt;                  //!< 2 bits   : Spare
    offset1: Integer;                  //!< 12 bits  : Slot Offset 1
    slots1: SmallInt;                  //!< 4 bits   : Number of Slots 1
    timeout1: SmallInt;                //!< 3 bits   : Timeout in Minutes 2
    increment1: Integer;               //!< 11 bits  : Slot Increment 1                                          
    offset2: Integer;                  //!< 12 bits  : Slot Offset 2                                          
    slots2: SmallInt;                  //!< 4 bits   : Number of Slots 2                                          
    timeout2: SmallInt;                //!< 3 bits   : Timeout in Minutes 2                                          
    increment2: Integer;               //!< 11 bits  : Slot Increment 2                                          
    offset3: Integer;                  //!< 12 bits  : Slot Offset 3
    slots3: SmallInt;                  //!< 4 bits   : Number of Slots 3
    timeout3: SmallInt;                //!< 3 bits   : Timeout in Minutes 3
    increment3: Integer;               //!< 11 bits  : Slot Increment 3
    offset4: Integer;                  //!< 12 bits  : Slot Offset 4
    slots4: SmallInt;                  //!< 4 bits   : Number of Slots 4
    timeout4: SmallInt;                //!< 3 bits   : Timeout in Minutes 4                                          
    increment4: Integer;               //!< 11 bits  : Slot Increment 4
    spare2: SmallInt;                  //!< 0-6 bits : Spare
    num_cmds: SmallInt;                //!< Number of commands received                                          
  end {aismsg_20};

  { AIS Message 21 - Aids-to-navigation Report }
  TAISMsg_21 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (21)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    aton_type: SmallInt;               //!< 5 bits   : Type of AtoN
    name: Array[0..20] Of Char;        //!< 120 bits : Name of AtoN in ASCII
    pos_acc: SmallInt;                 //!< 1 bit    : Position Accuracy
    longitude: LongInt;                //!< 28 bits  : Longitude in 1/10000 minutes
    latitude: LongInt;                 //!< 27 bits  : Latitude in 1/10000 minutes
    dim_bow: Integer;                  //!< 9 bits   : GPS Ant. Distance from Bow
    dim_stern: Integer;                //!< 9 bits   : GPS Ant. Distance from Stern
    dim_port: SmallInt;                //!< 6 bits   : GPS Ant. Distance from Port
    dim_starboard: SmallInt;           //!< 6 bits   : GPS Ant. Distance from Starboard
    pos_type: SmallInt;                //!< 4 bits   : Type of Position Fixing Device
    utc_sec: SmallInt;                 //!< 6 bits   : UTC Seconds
    off_position: SmallInt;            //!< 1 bit    : Off Position Flag
    regional: Byte;                    //!< 8 bits   : Regional Bits
    raim: SmallInt;                    //!< 1 bit    : RAIM Flag
    virtual: SmallInt;                 //!< 1 bit    : Virtual/Pseudo AtoN Flag
    assigned: SmallInt;                //!< 1 bit    : Assigned Mode Flag
    spare1: SmallInt;                  //!< 1 bit    : Spare
    name_ext: Array[0..15] Of Char;    //!< 0-84 bits : Extended name in ASCII
    spare2: SmallInt;                  //!< 0-6 bits : Spare
  end {aismsg_21};

  { AIS Message 22 - Channel Management }
  { If the message is for a geographic area its 2 corners are }
  { defined by the NE and SW positions. If it is an assigned }
  { message the destination MMSI will be in addressed_1 and }
  { addressed_2. }
  TAISMsg_22 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (22)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare1: SmallInt;                  //!< 2 bits   : Spare
    channel_a: Integer;                //!< 12 bits  : M.1084 Channel A Frequency
    channel_b: Integer;                //!< 12 bits  : M.1084 Channel B Frequency                                          
    txrx_mode: SmallInt;               //!< 4 bits   : TX/RX Mode                                          
    power: SmallInt;                   //!< 1 bit    : Power Level                                          
    NE_longitude: LongInt;             //!< 18 bits  : Longitude in 1/1000 minutes                                          
    NE_latitude: LongInt;              //!< 17 bits  : Latitude in 1/1000 minutes                                          
    addressed_1: LongInt;              //!< 30 bits  : Destination MMSI 1
    SW_longitude: LongInt;             //!< 18 bits  : Longitude in 1/1000 minutes
    SW_latitude: LongInt;              //!< 17 bits  : Latitude in 1/1000 minutes
    addressed_2: LongInt;              //!< 30 bits  : Destination MMSI 2                                          
    addressed: SmallInt;               //!< 1 bit    : Addressed flag
    bw_a: SmallInt;                    //!< 1 bit    : Channel A Bandwidth
    bw_b: SmallInt;                    //!< 1 bit    : Channel B Bandwidth
    tz_size: SmallInt;                 //!< 3 bits   : Transitional Zone size
    spare2: LongInt;                   //!< 23 bits  : Spare                                          
  end {aismsg_22};

  { AIS Message 23 - Group Assignment Command }
  { The geographic area is defined by 2 corners, stored in }
  { the NE and SW positions. }
  TAISMsg_23 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (23)                                          
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    spare1: SmallInt;                  //!< 2 bits   : Spare
    NE_longitude: LongInt;             //!< 18 bits  : Longitude in 1/1000 minutes
    NE_latitude: LongInt;              //!< 17 bits  : Latitude in 1/1000 minutes
    SW_longitude: LongInt;             //!< 18 bits  : Longitude in 1/1000 minutes
    SW_latitude: LongInt;              //!< 17 bits  : Latitude in 1/1000 minutes
    station_type: SmallInt;            //!< 4 bits   : Station Type
    ship_type: Byte;                   //!< 8 bits   : Type of Ship and Cargo
    spare2: LongInt;                   //!< 22 bits  : Spare
    txrx_mode: SmallInt;               //!< 2 bits   : TX/RX Mode
    report_interval: SmallInt;         //!< 4 bits   : Reporting Interval from IEC 62287 Table 17
    quiet_time: SmallInt;              //!< 4 bits   : Quiet Time in Minutes
    spare3: SmallInt;                  //!< 6 bits   : Spare
  end {aismsg_23};

  { AIS Message 24 - Class B CS Static Data Report }
  { Message 24 is a 2 part message, 24A with the name and }
  { 24B with the dimensions, etc. This structure is used }
  { to hold the results. }
  TAISMsg_24 = Record
    msgid: SmallInt;                   //!< 6 bits   : MessageID (24)
    isrepeat: SmallInt;                //!< 2 bits   : Repeated
    userid: LongInt;                   //!< 30 bits  : UserID / MMSI
    part_number: SmallInt;             //!< 2 bits   : Part Number

    ///!< Message 24A
    name: Array[0..20] Of Char;        //!< 120 bits : Ship Name in ASCII

    ///!< Message 24B
    ship_type: Byte;                   //!< 8 bits   : Type of Ship and Cargo
    vendor_id: Array[0..7] Of Char;    //!< 42 bits  : Vendor ID in ASCII
    callsign: Array[0..7] Of Char;     //!< 42 bits  : Callsign in ASCII
    dim_bow: Integer;                  //!< 9 bits   : GPS Ant. Distance from Bow
    dim_stern: Integer;                //!< 9 bits   : GPS Ant. Distance from Stern
    dim_port: SmallInt;                //!< 6 bits   : GPS Ant. Distance from Port
    dim_starboard: SmallInt;           //!< 6 bits   : GPS Ant. Distance from Starboard
    spare: SmallInt;                   //!< 6 bits   : Spare

    flags: SmallInt;                   //!< A/B flags - A = 1  B = 2  Both = 3
  end {aismsg_24};

  { ------------------------------------------------------------------------ }
  { ais_state the state for the Message Parser                               }
  { It keeps track partial messages until a complete message has been        }
  { received and it hols the sixbit state for exteacting bits from the       }
  { message.                                                                 }
  { ------------------------------------------------------------------------ }
  TAIS_State = Record
    msgid: Byte;                       //!< Message ID 0-31
    sequence: DWORD;                   //!< VDM message sequence number
    total: DWORD;                      //!< Total # of parts for the message
    num: DWORD;                        //!< Number of the last part stored
    channel: Char;                     //!< AIS Channel character
    six_state: TSixBit;                //!< sixbit parser state
  end {ais_state};


{ SixBit Functions }
Function BinFrom6bit(C: Char): ShortInt;
Procedure Init6bit(Var State: TSixBit);
Function Get6bit(Var State: TSixBit; NumBits: Integer): LongWord;

{ NMEA Functions }
Function IsXDigit(C: Char): Boolean;
Function NMEANextField(P: PChar): PChar;
Function NMEAUInt(P: PChar): DWORD;
Procedure NMEACopyField(Dest, Src: PChar; Len: Integer);
Function FindNMEAStart(Buffer: PChar): PChar;
Function NMEACheckSum(Buffer: PChar; Var CheckSum: Byte): Integer;
Function CheckNMEACheckSum(Buffer: PChar; Var CheckSum: Byte): Integer;

{ VDM_Parse Functions }
Function AIS2ASCII(C: Byte): Char;

Function Pos2DDD(Latitude, Longitude: LongInt; Var Lat_DD,
  Long_DDD: Double): Integer;
Function Pos2DMM(Latitude, Longitude: LongInt; Var Lat_DD: SmallInt;
  Var Lat_Min: Double; Var Long_DDD: SmallInt; Var Long_Min: Double): Integer;
Function ConvPos(Var Latitude, Longitude: LongInt): Integer;

Function AssembleVDM(Var State: TAIS_State; Str: PChar): Integer;

Function ParseAIS_1(Var State: TAIS_State; Var Msg: TAISMsg_1): Integer;
Function ParseAIS_2(Var State: TAIS_State; Var Msg: TAISMsg_2): Integer;
Function ParseAIS_3(Var State: TAIS_State; Var Msg: TAISMsg_3): Integer;
Function ParseAIS_4(Var State: TAIS_State; Var Msg: TAISMsg_4): Integer;
Function ParseAIS_5(Var State: TAIS_State; Var Msg: TAISMsg_5): Integer;
Function ParseAIS_6(Var State: TAIS_State; Var Msg: TAISMsg_6): Integer;
Function ParseAIS_7(Var State: TAIS_State; Var Msg: TAISMsg_7): Integer;
Function ParseAIS_8(Var State: TAIS_State; Var Msg: TAISMsg_8): Integer;
Function ParseAIS_9(Var State: TAIS_State; Var Msg: TAISMsg_9): Integer;
Function ParseAIS_10(Var State: TAIS_State; Var Msg: TAISMsg_10): Integer;
Function ParseAIS_11(Var State: TAIS_State; Var Msg: TAISMsg_11): Integer;
Function ParseAIS_12(Var State: TAIS_State; Var Msg: TAISMsg_12): Integer;
Function ParseAIS_13(Var State: TAIS_State; Var Msg: TAISMsg_13): Integer;
Function ParseAIS_14(Var State: TAIS_State; Var Msg: TAISMsg_14): Integer;
Function ParseAIS_15(Var State: TAIS_State; Var Msg: TAISMsg_15): Integer;
Function ParseAIS_16(Var State: TAIS_State; Var Msg: TAISMsg_16): Integer;
Function ParseAIS_17(Var State: TAIS_State; Var Msg: TAISMsg_17): Integer;
Function ParseAIS_18(Var State: TAIS_State; Var Msg: TAISMsg_18): Integer;
Function ParseAIS_19(Var State: TAIS_State; Var Msg: TAISMsg_19): Integer;
Function ParseAIS_20(Var State: TAIS_State; Var Msg: TAISMsg_20): Integer;
Function ParseAIS_21(Var State: TAIS_State; Var Msg: TAISMsg_21): Integer;
Function ParseAIS_22(Var State: TAIS_State; Var Msg: TAISMsg_22): Integer;
Function ParseAIS_23(Var State: TAIS_State; Var Msg: TAISMsg_23): Integer;
Function ParseAIS_24(Var State: TAIS_State; Var Msg: TAISMsg_24): Integer;

Implementation

{===============================================================================
  SIXBIT Functions

  6-bit packed ASCII Functions
  Copyright 2006 by Brian C. Lane <bcl@brianlane.com>, All Rights Reserved
  Version 1.0

  This module's Functions are used to extract data from the 6-bit packed
  ASCII string used by AIVDM/AIVDO AIS messages.

  The State of the sixbit machine is held in the sixbit structure so that
  multiple streams can be processed by maintaining separate instances
  of sixbit. init_6bit() should be called with a pointer to the sixbit
  instance, it will setup the structure for parsing. The 6-bit data
  should then be copied into the sixbit.bits buffer. A maximum of 255
  characters are allowed (this is changed by #SIXBIT_LEN in sixbit.h)

  Up to 32 bits of data are fetched from the string by calling get_6bit()

  The size of the packet can be calculated with strlen(State->bits) * 6 but
  note that due to padding at the end of the string the data may be
  1-6 bits longer than the the expected length for the message id.

  It is up to the calling Function to keep track of how many bits have
  been fetched. When it reaches the end of the 6-bit string get_6bit()
  will return 0's.

===============================================================================}

(* -----------------------------------------------------------------------
    Convert an ascii value to a 6-bit binary value

    This Function checks the ASCII value to make sure it can be coverted.
    If not, it returns a 0.
    Otherwise it returns the 6-bit ASCII value.

    returns:
      - -1 if it fails
      - 6-bit value (0x00-0x3F)

    This is used to convert the packed 6-bit value to a binary value. It
    is not used to convert data from fields such as the name and
    destination -- Use ais2ascii() instead.
   ----------------------------------------------------------------------- *)
Function BinFrom6bit(C: Char): ShortInt;
Var
  B: Integer;
Begin
  B := Ord(C);
  If (B < $30) Or (B > $77) Or ((B > $57) And (B < $60)) Then
    Result := -1
  Else If (B < $60) Then
    Result := (B - $30) And $3F
  Else
    Result := (B - $38) And $3F
End;

(* -----------------------------------------------------------------------
    Initialize a 6-bit datastream structure

    \param State State of 6-bit parser

    This Function initializes the State of the sixbit parser variables
    and 0 terminates the 6-bit string.

    Example:
    \code
    sixbit State;

    init_6bit( &State );
    \endcode
   ----------------------------------------------------------------------- *)
Procedure Init6bit(Var State: TSixBit);
Begin
  State.remainder := 0;
  State.remainder_bits := 0;
  State.p := @State.bits[0];
  FillChar(State.Bits, SizeOf(State.Bits), 0);
End;

(* -----------------------------------------------------------------------
    Return 0-32 bits from a 6-bit ASCII stream

    \param State pointer to a sixbit State structure
    \param numbits number of bits to return

    This Function returns the requested number of bits to the calling
    Function. It pulls the bits from the raw 6-bit ASCII as they are
    needed.

    The full string can be addressed by pointing to State->bits and the
    length can be calculated by strlen(State->bits) * 6, but note that the
    string also includes any final padding, so when checking lengths take
    into account that it will be a multiples of 6 bits.

    Example:
    \code
    sixbit  State;
    unsigned char i;

    init_6bit( &State );
    strcpy( State.bits, "5678901234" );
    i = get_6bit( &State, 6 );

    i == 5
    \endcode
   ----------------------------------------------------------------------- *)
Function Get6bit(Var State: TSixBit; NumBits: Integer): LongWord;
Var
  Fetch_Bits: Integer;
Begin
  Result := 0;
  Fetch_Bits := NumBits;

  While (Fetch_Bits > 0) Do
  Begin
    // Is there anything left over from the last call?
    If State.remainder_bits > 0 Then
    Begin
      If State.remainder_bits <= Fetch_Bits Then
      Begin
        // reminder is less than or equal to what is needed
        Result := (Result Shl 6) + State.remainder;
        Fetch_Bits := Fetch_Bits - State.remainder_bits;
        State.remainder := 0;
        State.remainder_bits := 0;
      End
      Else
      Begin
        { remainder is larger than what is needed Take the bits from
          the top of remainder }
        Result := Result Shl Fetch_Bits;
        Result := Result + State.remainder Shr (State.remainder_bits - Fetch_Bits);

        // Fixup remainder
        State.remainder_bits := State.remainder_bits - Fetch_Bits;
        State.remainder := State.remainder And pow2_mask[State.remainder_bits];

        Break;
      End;
    End;

    // Get the next block of 6 bits from the ASCII string
    If State.p^ <> #0 Then
    Begin
      State.remainder := binfrom6bit(State.p^);
      State.remainder_bits := 6;
      Inc(State.p);
    End
    Else
      // Nothing more to fetch, return what we have 
      Break;
  End;
End;




{===============================================================================
  NMEA Functions

  NMEA 0183 Sentence Parser Module
  Copyright 2006 by Brian C. Lane <bcl@brianlane.com>, All Rights Reserved
  version 1.0

  This module provides utility Functions for handling NMEA 0183 data
  streams. The stream data is 8 bit ASCII with comma separated data
  fields. The Functions in this module can be used to generate and
  verify the checksum, find the start of the sentence, find the next
  field, convert a field to an integer and copy a field to a destination
  buffer.

===============================================================================}

Function IsXDigit(C: Char): Boolean;
Begin
  Result := C In ['0'..'9', 'A'..'F', 'a'..'f'];
End;

(* -----------------------------------------------------------------------
  Return a pointer to the next field in the string

    \param p Pointer to the NMEA 0183 field

    Returns:
      - pointer to next field in string
      - NULL if the end of the string has been reached

    Example:
    \code
    char buf[255];
    char *p;

    strcpy( buf, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    p = nmea_next_field( p );

    *p == '1'
    \endcode
  ----------------------------------------------------------------------- *)
Function NMEANextField(P: PChar): PChar;
Begin
  While Assigned(P) And (P^ <> #0) And (P^ <> ',') And (P^ <> '*') Do
    Inc(P);

  If (P^ <> #0) Then
    Result := P + 1
  Else
    Result := P;
End;

(* -----------------------------------------------------------------------
   Convert the current field to an unsigned integer

    \param p Pointer to the NMEA 0183 field

    Returns:
      - unsigned int

    Example:
    \code
    char buf[255];
    char *p;
    unsigned int i;

    strcpy( buf, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    p = nmea_next_field( p );
    i = nmea_uint( p );

    i == 1
    \endcode

  ----------------------------------------------------------------------- *)
Function NMEAUInt(P: PChar): DWORD;
Var
  I: Integer;
Begin
  I := 0;
  While Assigned(P) And (P^ <> #0) And (P^ <> ',') And (P^ <> '*') Do
  Begin
    I := I * 10;
    I := I + Ord(P^) - Ord('0');
    Inc(P);
  End;
  Result := I;
End;

(* -----------------------------------------------------------------------
   Copy len bytes from a field to a destination buffer

    \param dest pointer to destination buffer
    \param src pointer to 0 terminated source buffer
    \param len maximum number of characters to copy

    Returns:
      - unsigned int

    This routine stops copying when it hits the end of the field, end of
    the string or len.

    Example:
    \code
    char buf1[255];
    char buf2[255];
    char *p;

    strcpy( buf1, "!AIVDM,1,1,,A,19NWoq000Wrt<RJHEuuqiWlN061d,0*5F" );
    buf2[0] = 0;

    p = buf1;
    nmea_copy_field( buf2, p, 254 );

    buf2 == buf1
    \endcode

  ----------------------------------------------------------------------- *)
Procedure NMEACopyField(Dest, Src: PChar; Len: Integer);
Begin
  While (Len > 0) And (Src^ <> #0) And (Src^ <> ',') And (Src^ <> '*') Do
  Begin
    Dest^ := Src^;
    Inc(Dest);
    Inc(Src);
    Dec(Len);
  End;
  Dest^ := #0;
End;

(* -----------------------------------------------------------------------
   Find the start of a NMEA sentence '!' or '$' character

    \param buffer pointer to a 0 terminated string buffer

    Returns:
        - pointer to start character or NULL if not found

    The NMEA sentence may not always start at the beginning of the buffer,
    use this routine to make sure the start of the sentence has been found.

    Example:
    \code
    char buf[255];
    char *p;

    strcpy( (char * ) buf, "678,4343,123,585*FF\n$GPGGA,32,121,676,29394,29234*AA\n" );
    p = find_nmea_start( buf ) );

    *p == "$GPGGA,..."
    \endcode

  ----------------------------------------------------------------------- *)
Function FindNMEAStart(Buffer: PChar): PChar;
Var
  P: PChar;
Begin
  Result := Nil;

  If Not Assigned(Buffer) Then
    Exit;

  P := @Buffer[0];
  While Assigned(P) And (P^ <> #0) And (P^ <> '!') And (P^ <> '$') Do
    Inc(P);

  If (P^ <> #0) Then
    Result := P;
End;

(* -----------------------------------------------------------------------
   Calculate the checksum of a NMEA 0183 sentence

    \param buffer pointer to a 0 terminated buffer
    \param checksum pointer to variable where checksum will be stored

    Returns:
        - 0 if no error
        - 1 if there was an error
        - Fills in *checksum with calculated checksum

    This Function will calculate the checksum of the string by skipping the
    ! or $ character and stopping at the * character or end of string.

    Example:
    \code
    char buf[255];
    unsigned char checksum;

    strcpy( (char * ) buf, "!AIVDM,1,1,,A,15N;<J0P00Jro1<H>bAP0?vL00Rb,0*1B\n" );
    nmea_checksum( buf, &checksum )

    checksum == 0x1B
    \endcode

  ----------------------------------------------------------------------- *)
Function NMEACheckSum(Buffer: PChar; Var CheckSum: Byte): Integer;
Var
  P: PChar;
Begin
  Result := 1;

  If Not Assigned(Buffer) Then
    Exit;

  { Find start of sentence, after a '!' or '$' }
  P := FindNMEAStart(Buffer);
  If Not Assigned(P) Then
    Exit;

  Inc(P);
  CheckSum := 0;
  { Run checksum until end or illegal character }
  While (P^ <> #0) And (P^ <> '*') And (Not ((P^ = '!') Or (P^ = '$'))) Do
  Begin
    Checksum := CheckSum Xor Ord(P^);
    Inc(P);
  End;

  { Check for illegal character }
  If (P^ = '*') Then
    Result := 0;
End;

(* -----------------------------------------------------------------------
  Check and return the checksum of a NMEA 0183 sentence

    \param buffer pointer to a 0 terminated buffer
    \param checksum pointer to variable where checksum will be stored

    Returns:
        - 0 if it does match
        - 1 if it does not match
        - 2 if there was an error

    This Function checks the checksum of a string and returns the result.
    The string is expected to start with a ! or $ and end after the
    2 hex checksum characters. Trailing CR and/or LF are optional. The
    actual checksum is returned in the variable pointed to by checksum.

    Example:
    \code
    char buf[255];
    unsigned char checksum;

    strcpy( (char * ) buf, "!AIVDM,1,1,,A,15N;<J0P00Jro1<H>bAP0?vL00Rb,0*1B\n" );
    if( check_nmea_checksum( buf, &checksum ) != 0 )
    {
        fprintf( stderr, "Checksum failed\n" );
    }

    checksum == 0x1B
    \endcode
  ----------------------------------------------------------------------- *)
Function CheckNMEACheckSum(Buffer: PChar; Var CheckSum: Byte): Integer;
Var
  RV: Integer;
  P: PChar;
  SSum: Byte;
Begin
  If Not Assigned(Buffer) Then
    Result := 2
  Else
  Begin
    RV := NMEACheckSum(Buffer, CheckSum);
    If RV <> 0 Then
      Result := 2
    Else
    Begin
      { Find the checksum in the sentence }
      P := StrScan(Buffer, '*');
      If Not Assigned(P) Then
        Result := 2
      { Make sure it found the end of the sentence }
      Else If (P^ <> '*') Then
        Result := 1
      { Make sure there is a checksum to check }
      Else If Not (IsXDigit((P + 1)^) And IsXDigit((P + 2)^)) Then
        Result := 1
      Else
      Begin
        { Convert the checksum and check it }
        HexToBin(PChar(P + 1), @SSum, SizeOf(SSum));
        If (SSum = CheckSum) Then
          Result := 0
        Else
          Result := 1;
      End;
    End;
  End;
End;




{===============================================================================
  VDM_PARSE Functions

  AIVDM/AIVDO AIS Sentence Parser
  Copyright 2006 by Brian C. Lane <bcl@brianlane.com>, All Rights Reserved
  Version 1.0

  This module contains the Functions to parse incoming Automatic
  Identification System (AIS) serial messages as defined by IEC-61993-2
  Each message type has its own structure, of the form aismsg_XX, based
  on the message id of the message. The parse routine for each message
  is of the form ParseAIS_<msgid>, eg, ParseAIS_5()

  Each of the aismsg_XX parsing Functions expect to be passed a pointer
  to a ais_sate structure containing the raw 6-bit data and a pointer
  to the aismsg_XX structure to place the parsed information into.

  All aismsg_XX structures are cleared by their respecive parsers, except
  for ParseAIS_24() which stores 2 messages into a single structure.

  Data like the ship's name and callsign are converted to ASCII
  before being stored. Positions are converted to signed long values,
  and other fields like course over ground, speed, heading, etc.
  are left for further decoding. 1/1000 minute positions are converted
  to 1/10000 minute positions to make comparisons easier.

  Not all of these parsers have been fully tested with live data.
  Here is a list of the ones that have been tested so far:
    1,3,4,5,7,12,15,20

  If you have valid data for other message ids and would like to
  contribute them please email them to bcl@brianlane.com

  Example:
  \code
  ais_state State;
  aismsg_1  message;
  unsigned int  result;
  char buf[] = "!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27";

  memset( &State, 0, sizeof(State) );
  assemble_vdm( &State, buf )
  State.msgid = (char) get_6bit( &State.six_state, 6 );
  if (State.msgid == 1)
      ParseAIS_1( &State, &message )

  message now contains all of the data from the AIS Message 1
  \endcode

===============================================================================}

(* -----------------------------------------------------------------------
    Convert a AIS 6-bit character to ASCII

    \param value 6-bit value to be converted

    return
      - corresponding ASCII value (0x20-0x5F)

    This function is used to convert binary data to ASCII. This is
    different from the 6-bit ASCII to binary conversion for VDM
    messages; it is used for strings within the datastream itself.
    eg. Ship Name and Destination.

   ----------------------------------------------------------------------- *)
Function AIS2ASCII(C: Byte): Char;
Begin
  Byte(Result) := C And $3f;
  If Byte(Result) < $20 Then
    Byte(Result) := Byte(Result) + $40;
End;

(* -----------------------------------------------------------------------
    Convert 1/10000 minute position to signed DD.DDDDDD format

    \param latitude signed latitude in 1/10000 minute format
    \param longitude signed longitude in 1/10000 minute format
    \param lat_dd pointer to hold signed latitude in DD.DDDDDD format
    \param long_ddd pointer to hold signed longitude in DD.DDDDDD format

    This function converts 1/10000 minute formatted positions to standard
    Degrees and decimal degrees format.

   ----------------------------------------------------------------------- *)
Function Pos2DDD(Latitude, Longitude: LongInt; Var Lat_DD,
  Long_DDD: Double): Integer;
Begin
  // Convert 1/10000 Minute Latitude to DD.DDDDDD
  Lat_DD := Latitude Div 600000;
  Lat_DD := Lat_DD + (Latitude - (Lat_DD * 600000)) / 600000;

  // Convert 1/10000 Minute Longitude to DDD.DDDDDD
  Long_DDD := Longitude Div 600000;
  Long_DDD := Long_DDD + (Longitude - (Long_DDD * 600000)) / 600000;

  Result := 0;
End;

(* -----------------------------------------------------------------------
    Convert 1/10000 minute position to signed DD MM.MMMM format

    \param latitude signed latitude in 1/10000 minute format
    \param longitude signed longitude in 1/10000 minute format
    \param lat_dd pointer to hold signed latitude in DD format
    \param lat_min pointer to hold minutes in mm.mmmm format
    \param long_ddd pointer to hold signed longitude in DDD format
    \param long_min pointer to hold minutes in mm.mmmm format


    This function converts 1/10000 minute formatted positions to standard
    Degrees and decimal minutes format.

   ----------------------------------------------------------------------- *)
Function Pos2DMM(Latitude, Longitude: LongInt; Var Lat_DD: SmallInt;
  Var Lat_Min: Double; Var Long_DDD: SmallInt; Var Long_Min: Double): Integer;
Begin
  // Convert 1/10000 minute Latitude to DD MM.MMMM format
  Lat_DD := (Latitude Div 600000);
  Lat_Min := Abs((Latitude - (Lat_DD * 600000))) / 10000.0;

  // Convert 1/10000 minute Longitude to DDD MM.MMMM format
  Long_DDD := (Longitude Div 600000);
  Long_Min := Abs((Longitude - (Long_DDD * 600000))) / 10000.0;

  Result := 0;
End;

(* -----------------------------------------------------------------------
    Convert unsigned 1/10000 minute position to signed

    \param latitude pointer to an unsigned latitude
    \param longitude pointer to an unsigned longitude

    This function converts the raw 2's complement values to signed long
    1/10000 minute position. It is used internally by all of the messages
    that contain position information.

   ----------------------------------------------------------------------- *)
Function ConvPos(Var Latitude, Longitude: LongInt): Integer;
Begin
  // LATITUDE
  // Convert latitude to signed number
  If (Latitude And $4000000) <> 0 Then           {  61708864 }
  Begin
    Latitude := $8000000 - Latitude;            { 134217728 }
    Latitude := Latitude * -1;
  End;

  // LONGITUDE
  // Convert longitude to signed number
  If (Longitude And $8000000) <> 0 Then          { 134217728 }
  Begin
    Longitude := $10000000 - Longitude;         { 268435456 }
    Longitude := Longitude * -1;
  End;

  Result := 0;
End;

(* -----------------------------------------------------------------------
    Assemble AIVDM/VDO sentences

    This function handles re-assembly and extraction of the 6-bit data
    from AIVDM/AIVDO sentences.

    Because the NMEA standard limits the length of a line to 80 characters
    some AIS messages, such as message 5, are output as a multipart VDM
    messages.
    This routine collects the 6-bit encoded data from these parts and
    returns a 1 when all pieces have been reassembled.

    It expects the sentences to:
      - Be in order, part 1, part 2, etc.
      - Be from a single sequence

    It will return an error if it receives a piece out of order or from
    a new sequence before the previous one is finished.


    Returns
        - 0 Complete packet
        - 1 Incomplete packet
        - 2 NMEA 0183 checksum failed
        - 3 Not an AIS message
        - 4 Error with nmea_next_field
        - 5 Out of sequence packet

    Example:
    \code
    ais_state state;
    char buf[3][255] = { "!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n",
                         "!AIVDM,2,1,9,A,55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H,0*49\r\n",
                         "!AIVDM,2,2,9,A,==40HtI4i@E531H1QDTVH51DSCS0,2*16\r\n"
                       };

    memset( &state, 0, sizeof(state) );
    assemble_vdm( &state, buf[0] )
    state.six_state now has the 6-bit data from the message

    assemble_vdm( &state, buf[1] );
    This returns a 1 because not all the pieces have been received.

    assemble_vdm( &state, buf[2] );
    This returns a 0 and state.six_state has the complete 6-bit data for
    the message 5.

    \endcode

   ----------------------------------------------------------------------- *)
Function AssembleVDM(Var State: TAIS_State; Str: PChar): Integer;
Var
  I, Len: Integer;
  Total, Num, Sequence: DWORD;
  P, D: PChar;
  CheckSum: Byte;
Begin
  Total := 0;
  Num := 0;
  Sequence := 0;

  // Check the strings's checksum
  If CheckNMEAChecksum(Str, CheckSum) <> 0 Then
  Begin
    // Checksum failed
    Result := 2;
    Exit;
  End;

  // Is the string an AIS message?
  P := FindNMEAStart(Str);
  If (StrLComp(P, PChar('!AIVDM'), 6) <> 0) And
     (StrLComp(P, PChar('!AIVDO'), 6) <> 0) Then
  Begin
    // Not an AIS message
    Result := 3;
    Exit;
  End;

  // Get the 3 message info values from the packet
  For I := 0 To 2 Do
  Begin
    P := NMEANextField(P);
    If Not Assigned(P) Then
    Begin
      // Error with the string;
      Result := 4;
      Exit;
    End;

    Case I Of
      0:  Total := NMEAUint(P);
      1:  Num := NMEAUint(P);
      2:  Sequence := NMEAUint(P);
    End;
  End;

  // Are we looking for more parts?
  If State.total > 0 Then
  Begin
    // If the sequence doesn't match, or the number is not in
    //   order: reset and exit
    If (State.sequence <> Sequence) Or (State.num <> (Num - 1)) Then
    Begin
      State.total := 0;
      State.sequence := 0;
      State.num := 0;
      Result := 5;
      Exit;
    End;
    State.num := State.num + 1;
  End
  Else
  Begin
    // Not looking for more parts.  Reset the state.
    State.total := Total;
    State.num := Num;
    State.sequence := Sequence;
    Init6Bit(State.six_state);
  End;

  // Get the channel character
  P := NMEANextField(P);
  If Not Assigned(P) Then
  Begin
    // Error with the string.
    Result := 4;
    Exit;
  End;
  State.channel := P^;

  // Point to the 6-bit data
  P := NMEANextField(P);
  If Not Assigned(P) Then
  Begin
    // Error with the string.
    Result := 4;
    Exit;
  End;

  // Copy the 6-bit ASCII field over to the sixbit_state buffer
  D := @State.six_state.bits;
  Len := 0;

  // Find the end of the current string and count the number of bytes
  While (D^ <> #0) Do
  Begin
    Inc(Len);
    Inc(D);
  End;
  NMEACopyField(D, P, SIXBIT_LEN - Len);

  // Is this the last part of the sequence?
  If (Total = 0) Or (State.total = Num) Then
  Begin
    State.total := 0;
    State.num := 0;
    State.sequence := 0;

    // Found a complete packet
    Result := 0;
  End
  Else
    // Waiting for more packets to complete data
    Result := 1;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 1 into an aismsg_1 structure

    Ship Position report with SOTDMA Communication state

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Note that the latitude and longitude are converted to signed values
    before being returned.

   ----------------------------------------------------------------------- *)
Function ParseAIS_1(Var State: TAIS_State; Var Msg: TAISMsg_1): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 1
    Msg.isrepeat     := Get6Bit(State.six_state, 2  );
    Msg.userid       := Get6Bit(State.six_state, 30 );
    Msg.nav_status   := Get6Bit(State.six_state, 4  );
    Msg.rot          := Get6Bit(State.six_state, 8  );
    Msg.sog          := Get6Bit(State.six_state, 10 );
    Msg.pos_acc      := Get6Bit(State.six_state, 1  );
    Msg.longitude    := Get6Bit(State.six_state, 28 );
    Msg.latitude     := Get6Bit(State.six_state, 27 );
    Msg.cog          := Get6Bit(State.six_state, 12 );
    Msg.true         := Get6Bit(State.six_state, 9  );
    Msg.utc_sec      := Get6Bit(State.six_state, 6  );
    Msg.regional     := Get6Bit(State.six_state, 4  );
    Msg.spare        := Get6Bit(State.six_state, 1  );
    Msg.raim         := Get6Bit(State.six_state, 1  );
    Msg.sync_state   := Get6Bit(State.six_state, 2  );
    Msg.slot_timeout := Get6Bit(State.six_state, 3  );
    Msg.sub_message  := Get6Bit(State.six_state, 14 );

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
   Parse an AIS message 2 into an aismsg_2 structure

   Ship Position report with SOTDMA Communication state

   \param state    pointer to ais_state
   \param result   pointer to parsed message result structure to be filled

   return:
     - 0 if no errors
     - 1 if there is an error
     - 2 if there is a packet length error

    Note that the latitude and longitude are converted to signed values
    before being returned.

   ----------------------------------------------------------------------- *)
Function ParseAIS_2(Var State: TAIS_State; Var Msg: TAISMsg_2): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If Len <> 168 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 2
    Msg.isrepeat     := Get6Bit(State.six_state, 2  );
    Msg.userid       := Get6Bit(State.six_state, 30 );
    Msg.nav_status   := Get6Bit(State.six_state, 4  );
    Msg.rot          := Get6Bit(State.six_state, 8  );
    Msg.sog          := Get6Bit(State.six_state, 10 );
    Msg.pos_acc      := Get6Bit(State.six_state, 1  );
    Msg.longitude    := Get6Bit(State.six_state, 28 );
    Msg.latitude     := Get6Bit(State.six_state, 27 );
    Msg.cog          := Get6Bit(State.six_state, 12 );
    Msg.true         := Get6Bit(State.six_state, 9  );
    Msg.utc_sec      := Get6Bit(State.six_state, 6  );
    Msg.regional     := Get6Bit(State.six_state, 4  );
    Msg.spare        := Get6Bit(State.six_state, 1  );
    Msg.raim         := Get6Bit(State.six_state, 1  );
    Msg.sync_state   := Get6Bit(State.six_state, 2  );
    Msg.slot_timeout := Get6Bit(State.six_state, 3  );
    Msg.sub_message  := Get6Bit(State.six_state, 14 );

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 3 into an aismsg_3 structure

   Ship Position report with ITDMA Communication state

   \param state    pointer to ais_state
   \param result   pointer to parsed message result structure to be filled

   return:
     - 0 if no errors
     - 1 if there is an error
     - 2 if there is a packet length error

    Note that the latitude and longitude are converted to signed values
    before being returned.

   ----------------------------------------------------------------------- *)
Function ParseAIS_3(Var State: TAIS_State; Var Msg: TAISMsg_3): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 3
    Msg.isrepeat       := Get6Bit(State.six_state, 2  );
    Msg.userid         := Get6Bit(State.six_state, 30 );
    Msg.nav_status     := Get6Bit(State.six_state, 4  );
    Msg.rot            := Get6Bit(State.six_state, 8  );
    Msg.sog            := Get6Bit(State.six_state, 10 );
    Msg.pos_acc        := Get6Bit(State.six_state, 1  );
    Msg.longitude      := Get6Bit(State.six_state, 28 );
    Msg.latitude       := Get6Bit(State.six_state, 27 );
    Msg.cog            := Get6Bit(State.six_state, 12 );
    Msg.true           := Get6Bit(State.six_state, 9  );
    Msg.utc_sec        := Get6Bit(State.six_state, 6  );
    Msg.regional       := Get6Bit(State.six_state, 4  );
    Msg.spare          := Get6Bit(State.six_state, 1  );
    Msg.raim           := Get6Bit(State.six_state, 1  );
    Msg.sync_state     := Get6Bit(State.six_state, 2  );
    Msg.slot_increment := Get6Bit(State.six_state, 13 );
    Msg.num_slots      := Get6Bit(State.six_state, 3  );
    Msg.keep           := Get6Bit(State.six_state, 1  );

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 4 into an aismsg_4 structure

    Base Station Report

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

     Note that the latitude and longitude are converted to signed values
     before being returned.

   ----------------------------------------------------------------------- *)
Function ParseAIS_4(Var State: TAIS_State; Var Msg: TAISMsg_4): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 4
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.utc_year     :=  Get6Bit(State.six_state, 14 );
    Msg.utc_month    :=  Get6Bit(State.six_state, 4  );
    Msg.utc_day      :=  Get6Bit(State.six_state, 5  );
    Msg.utc_hour     :=  Get6Bit(State.six_state, 5  );
    Msg.utc_minute   :=  Get6Bit(State.six_state, 6  );
    Msg.utc_second   :=  Get6Bit(State.six_state, 6  );
    Msg.pos_acc      :=  Get6Bit(State.six_state, 1  );
    Msg.longitude    :=  Get6Bit(State.six_state, 28 );
    Msg.latitude     :=  Get6Bit(State.six_state, 27 );
    Msg.pos_type     :=  Get6Bit(State.six_state, 4  );
    Msg.spare        :=  Get6Bit(State.six_state, 10 );
    Msg.raim         :=  Get6Bit(State.six_state, 1  );
    Msg.sync_state   :=  Get6Bit(State.six_state, 2  );
    Msg.slot_timeout :=  Get6Bit(State.six_state, 3  );
    Msg.sub_message  :=  Get6Bit(State.six_state, 14 );

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 5 into an aismsg_5 structure

    Ship Static and Voyage Data

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

     return:
       - 0 if no errors
       - 1 if there is an error
       - 2 if there is a packet length error

     Note that the ship's callsign, name and destination are converted to
     ASCII before storage.

   ----------------------------------------------------------------------- *)
Function ParseAIS_5(Var State: TAIS_State; Var Msg: TAISMsg_5): Integer;
Var
  I: Integer;
Begin
  If StrLen(State.six_state.Bits) <> 71 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 4
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.version      :=  Get6Bit(State.six_state, 2  );
    Msg.imo          :=  Get6Bit(State.six_state, 30 );

    // Get the Callsign, convert to ASCII
    For I := 0 To 6 Do
      Msg.callsign[I] := AIS2ASCII(Get6Bit(State.six_state, 6));
    Msg.callsign[7] := #0;

    // Get the Ship Name, convert to ASCII
    For I := 0 To 19 Do
      Msg.name[I] := AIS2ASCII(Get6Bit(State.six_state, 6));
    Msg.name[20] := #0;

    Msg.ship_type    :=  Get6Bit(State.six_state, 8  );
    Msg.dim_bow      :=  Get6Bit(State.six_state, 9  );
    Msg.dim_stern    :=  Get6Bit(State.six_state, 9  );
    Msg.dim_port     :=  Get6Bit(State.six_state, 6  );
    Msg.dim_starboard:=  Get6Bit(State.six_state, 6  );
    Msg.pos_type     :=  Get6Bit(State.six_state, 4  );
    Msg.eta          :=  Get6Bit(State.six_state, 20 );
    Msg.draught      :=  Get6Bit(State.six_state, 8  );

    // Get the Ship Destination, convert to ASCII
    For I := 0 To 19 Do
      Msg.dest[I] := AIS2ASCII(Get6Bit(State.six_state, 6));
    Msg.name[20] := #0;

    Msg.dte          := Get6Bit(State.six_state, 1 );
    Msg.spare        := Get6Bit(State.six_state, 1 );

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 6 into an aismsg_6 structure

    Addressed Binary Message

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Note: the binary payload of the message is left in its 6-bit ASCII
          form when stored into result->data. This allows the data to
          be passed to the sixbit module for parsing.

   ----------------------------------------------------------------------- *)
Function ParseAIS_6(Var State: TAIS_State; Var Msg: TAISMsg_6): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 88) Or (Len > 1008) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 6
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.sequence     :=  Get6Bit(State.six_state, 2 );
    Msg.destination  :=  Get6Bit(State.six_state, 30 );
    Msg.retransmit   :=  Get6Bit(State.six_state, 1 );
    Msg.spare        :=  Get6Bit(State.six_state, 1 );
    Msg.app_id       :=  Get6Bit(State.six_state, 16 );

    // Store the remaining payload of the packet as 6-bit data
    StrLCopy(@Msg.data, State.six_state.p, 154);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 7 into an aismsg_7 structure

    Binary acknowledge

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Depending on the length of the message some of the fields may be 0.
    result->num_acks has the number of acks received.

   ----------------------------------------------------------------------- *)
Function ParseAIS_7(Var State: TAIS_State; Var Msg: TAISMsg_7): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 72) Or (Len > 168) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 7
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.spare        :=  Get6Bit(State.six_state, 2  );
    Msg.destid_1     :=  Get6Bit(State.six_state, 30 );
    Msg.sequence_1   :=  Get6Bit(State.six_state, 2  );
    Msg.num_acks := 1;

    if (Len > 72) Then
    Begin
      Msg.destid_2     :=  Get6Bit(State.six_state, 30 );
      Msg.sequence_2   :=  Get6Bit(State.six_state, 2  );
      Inc(Msg.num_acks);
    End;

    If (Len > 104) Then
    Begin
      Msg.destid_3     :=  Get6Bit(State.six_state, 30 );
      Msg.sequence_3   :=  Get6Bit(State.six_state, 2  );
      Inc(Msg.num_acks);
    End;

    If (Len > 136) Then
    Begin
      Msg.destid_4     :=  Get6Bit(State.six_state, 30 );
      Msg.sequence_4   :=  Get6Bit(State.six_state, 2  );
      Inc(Msg.num_acks);
    End;

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 8 into an aismsg_8 structure

   Binary Broadcast Message

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

   return:
     - 0 if no errors
     - 1 if there is an error
     - 2 if there is a packet length error

    Note: the binary payload of the message is left in its 6-bit ASCII
          form when stored into result->data. This allows the data to
          be passed to the sixbit module for parsing.

   ----------------------------------------------------------------------- *)
Function ParseAIS_8(Var State: TAIS_State; Var Msg: TAISMsg_8): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 56) Or (Len > 1008) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 8
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.spare        :=  Get6Bit(State.six_state, 1 );
    Msg.app_id       :=  Get6Bit(State.six_state, 16 );

    // Store the remaining payload of the packet as 6-bit data
    StrLCopy(@Msg.data, State.six_state.p, 160);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 9 into an aismsg_9 structure

    Search And Rescue(SAR) position report

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

     Note that the latitude and longitude are converted to signed values
     before being returned.

   ----------------------------------------------------------------------- *)
Function ParseAIS_9(Var State: TAIS_State; Var Msg: TAISMsg_9): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 9
    Msg.isrepeat   :=  Get6Bit(State.six_state, 2  );
    Msg.userid     :=  Get6Bit(State.six_state, 30 );
    Msg.altitude   :=  Get6Bit(State.six_state, 12 );
    Msg.sog        :=  Get6Bit(State.six_state, 10 );
    Msg.pos_acc    :=  Get6Bit(State.six_state, 1  );
    Msg.longitude  :=  Get6Bit(State.six_state, 28 );
    Msg.latitude   :=  Get6Bit(State.six_state, 27 );
    Msg.cog        :=  Get6Bit(State.six_state, 12 );
    Msg.utc_sec    :=  Get6Bit(State.six_state, 6  );
    Msg.regional   :=  Get6Bit(State.six_state, 8  );
    Msg.dte        :=  Get6Bit(State.six_state, 1  );
    Msg.spare      :=  Get6Bit(State.six_state, 3  );
    Msg.assigned   :=  Get6Bit(State.six_state, 1  );
    Msg.raim       :=  Get6Bit(State.six_state, 1  );
    Msg.comm_state :=  Get6Bit(State.six_state, 1  );

    If (Msg.comm_state = 0) Then
    Begin
      Msg.sotdma.sync_state   := Get6Bit(State.six_state, 2  );
      Msg.sotdma.slot_timeout := Get6Bit(State.six_state, 3  );
      Msg.sotdma.sub_message  := Get6Bit(State.six_state, 14 );
    End
    Else
    Begin
      Msg.itdma.sync_state    := Get6Bit(State.six_state, 2  );
      Msg.itdma.slot_inc      := Get6Bit(State.six_state, 13 );
      Msg.itdma.num_slots     := Get6Bit(State.six_state, 3  );
      Msg.itdma.keep_flag     := Get6Bit(State.six_state, 1  );
    End;

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 10 into an aismsg_10 structure

    UTC and Date Inquiry

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

   ----------------------------------------------------------------------- *)
Function ParseAIS_10(Var State: TAIS_State; Var Msg: TAISMsg_10): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 10
    Msg.isrepeat    :=  Get6Bit(State.six_state, 2  );
    Msg.userid      :=  Get6Bit(State.six_state, 30 );
    Msg.spare1      :=  Get6Bit(State.six_state, 2  );
    Msg.destination :=  Get6Bit(State.six_state, 30 );
    Msg.spare2      :=  Get6Bit(State.six_state, 2  );

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 11 into an aismsg_11 structure

    UTC and Date Response

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

   ----------------------------------------------------------------------- *)
Function ParseAIS_11(Var State: TAIS_State; Var Msg: TAISMsg_11): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 11
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.utc_year     :=  Get6Bit(State.six_state, 14 );
    Msg.utc_month    :=  Get6Bit(State.six_state, 4  );
    Msg.utc_day      :=  Get6Bit(State.six_state, 5  );
    Msg.utc_hour     :=  Get6Bit(State.six_state, 5  );
    Msg.utc_minute   :=  Get6Bit(State.six_state, 6  );
    Msg.utc_second   :=  Get6Bit(State.six_state, 6  );
    Msg.pos_acc      :=  Get6Bit(State.six_state, 1  );
    Msg.longitude    :=  Get6Bit(State.six_state, 28 );
    Msg.latitude     :=  Get6Bit(State.six_state, 27 );
    Msg.pos_type     :=  Get6Bit(State.six_state, 4  );
    Msg.spare        :=  Get6Bit(State.six_state, 10 );
    Msg.raim         :=  Get6Bit(State.six_state, 1  );
    Msg.sync_state   :=  Get6Bit(State.six_state, 2  );
    Msg.slot_timeout :=  Get6Bit(State.six_state, 3  );
    Msg.sub_message  :=  Get6Bit(State.six_state, 14 );

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 12 into an aismsg_12 structure

    Addressed Safety Related Message

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    The safety message is converted to ASCII before storage in the
    result->message field.

   ----------------------------------------------------------------------- *)
Function ParseAIS_12(Var State: TAIS_State; Var Msg: TAISMsg_12): Integer;
Var
  I,
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 72) Or (Len > 1008) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 12
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.sequence     :=  Get6Bit(State.six_state, 2  );
    Msg.destination  :=  Get6Bit(State.six_state, 30 );
    Msg.retransmit   :=  Get6Bit(State.six_state, 1  );
    Msg.spare        :=  Get6Bit(State.six_state, 1  );

    // Get the message, convert to ASCII
    I := 0;
    While (I <> ((Len - 72) Div 6)) Do
    Begin
      Msg.msg[I] := AIS2ASCII(Get6Bit(State.six_state, 6));
      Inc(I);
    End;
    Msg.msg[I] := #0;

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 13 into an aismsg_13 structure

    Safety Related Acknowledge

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Depending on the length of the message some of the fields may be 0.
    result->num_acks has the number of acks received.

   ----------------------------------------------------------------------- *)
Function ParseAIS_13(Var State: TAIS_State; Var Msg: TAISMsg_13): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 72) Or (Len > 168) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 13
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.spare        :=  Get6Bit(State.six_state, 2  );
    Msg.destid_1     :=  Get6Bit(State.six_state, 30 );
    Msg.sequence_1   :=  Get6Bit(State.six_state, 2  );
    Msg.num_acks     := 1;

    If (Len > 72) Then
    Begin
      Msg.destid_2     :=  Get6Bit(State.six_state, 30 );
      Msg.sequence_2   :=  Get6Bit(State.six_state, 2  );
      Inc(Msg.num_acks);
    End;

    If (Len > 104) Then
    Begin
      Msg.destid_3     :=  Get6Bit(State.six_state, 30 );
      Msg.sequence_3   :=  Get6Bit(State.six_state, 2  );
      Inc(Msg.num_acks);
    End;

    If (Len > 136) Then
    Begin
      Msg.destid_4     :=  Get6Bit(State.six_state, 30 );
      Msg.sequence_4   :=  Get6Bit(State.six_state, 2  );
      Inc(Msg.num_acks);
    End;

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 14 into an aismsg_14 structure

    Safety Related Broadcast

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is a parameter error
      - 2 if there is a packet length error

    The safety message is converted to ASCII before storage in the
    result->message field.

   ----------------------------------------------------------------------- *)
Function ParseAIS_14(Var State: TAIS_State; Var Msg: TAISMsg_14): Integer;
Var
  I,
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 40) Or (Len > 1008) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 14
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.spare        :=  Get6Bit(State.six_state, 2 );

    // Get the message, convert to ASCII
    I := 0;
    While (I <> ((Len - 40) Div 6)) Do
    Begin
      Msg.msg[I] := AIS2ASCII(Get6Bit(State.six_state, 6));
      Inc(I);
    End;
    Msg.msg[I] := #0;

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 15 into an aismsg_15 structure

    Interrogation

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Depending on the length of the message some if the fields may be 0.
    result->num_reqs has the number of interrogation requests.

   ----------------------------------------------------------------------- *)
Function ParseAIS_15(Var State: TAIS_State; Var Msg: TAISMsg_15): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 88) Or (Len > 162) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 15
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.spare1       :=  Get6Bit(State.six_state, 2  );
    Msg.destid1      :=  Get6Bit(State.six_state, 30 );
    Msg.msgid1_1     :=  Get6Bit(State.six_state, 6  );
    Msg.offset1_1    :=  Get6Bit(State.six_state, 12 );
    Msg.num_reqs     := 1;

    If (Len > 88) Then
    Begin
      Msg.spare2    := Get6Bit(State.six_state, 2 );
      Msg.msgid1_2  := Get6Bit(State.six_state, 6  );
      Msg.offset1_2 := Get6Bit(State.six_state, 12 );
      Msg.num_reqs  := 2;
    End;

    If (Len = 160) Then
    Begin
      Msg.spare3    := Get6Bit(State.six_state, 2  );
      Msg.destid2   := Get6Bit(State.six_state, 30 );
      Msg.msgid2_1  := Get6Bit(State.six_state, 6  );
      Msg.offset2_1 := Get6Bit(State.six_state, 12 );
      Msg.spare4    := Get6Bit(State.six_state, 2  );
      Msg.num_reqs  := 3;
    End;

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 16 into an aismsg_16 structure

    Assigned mode command

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Depending on the length of the message some of the fields may be 0.
    result->num_cmds has the number of commands received.

   ----------------------------------------------------------------------- *)
Function ParseAIS_16(Var State: TAIS_State; Var Msg: TAISMsg_16): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  <> 96) And (Len <> 144) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 16
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.spare1       :=  Get6Bit(State.six_state, 2  );
    Msg.destid_a     :=  Get6Bit(State.six_state, 30 );
    Msg.offset_a     :=  Get6Bit(State.six_state, 12 );
    Msg.increment_a  :=  Get6Bit(State.six_state, 10 );
    Msg.num_cmds     := 1;

    If (Len = 144) Then
    Begin
      Msg.destid_b     :=  Get6Bit(State.six_state, 30 );
      Msg.offset_b     :=  Get6Bit(State.six_state, 12 );
      Msg.increment_b  :=  Get6Bit(State.six_state, 10 );
      Msg.spare2       :=  Get6Bit(State.six_state, 4 );
      Msg.num_cmds     := 2;
    End;

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 17 into an aismsg_17 structure

    GNSS Binary Broadcast Message

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Note: the binary payload of the message is left in its 6-bit ASCII
          form when stored into result->data. This allows the data to
          be passed to the sixbit module for parsing.

   ----------------------------------------------------------------------- *)
Function ParseAIS_17(Var State: TAIS_State; Var Msg: TAISMsg_17): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 80) Or (Len > 816) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 17
    Msg.isrepeat     :=  Get6Bit(State.six_state, 2  );
    Msg.userid       :=  Get6Bit(State.six_state, 30 );
    Msg.spare1       :=  Get6Bit(State.six_state, 2  );
    Msg.longitude    :=  Get6Bit(State.six_state, 18 );
    Msg.latitude     :=  Get6Bit(State.six_state, 17 );
    Msg.spare2       :=  Get6Bit(State.six_state, 5  );
    Msg.msg_type     :=  Get6Bit(State.six_state, 6  );
    Msg.station_id   :=  Get6Bit(State.six_state, 10 );
    Msg.z_count      :=  Get6Bit(State.six_state, 13 );
    Msg.seq_num      :=  Get6Bit(State.six_state, 3  );
    Msg.num_words    :=  Get6Bit(State.six_state, 5  );
    Msg.health       :=  Get6Bit(State.six_state, 3  );

    // Store the remaining payload of the packet as 6-bit data
    StrLCopy(@Msg.data, State.six_state.p, 117);

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 18 into an aismsg_18 structure

    Class B Position Report

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

     Note that the latitude and longitude are converted to signed values
     before being returned.

   ----------------------------------------------------------------------- *)
Function ParseAIS_18(Var State: TAIS_State; Var Msg: TAISMsg_18): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 18
    Msg.isrepeat       := Get6Bit(State.six_state, 2  );
    Msg.userid         := Get6Bit(State.six_state, 30 );
    Msg.regional1      := Get6Bit(State.six_state, 8  );
    Msg.sog            := Get6Bit(State.six_state, 10 );
    Msg.pos_acc        := Get6Bit(State.six_state, 1  );
    Msg.longitude      := Get6Bit(State.six_state, 28 );
    Msg.latitude       := Get6Bit(State.six_state, 27 );
    Msg.cog            := Get6Bit(State.six_state, 12 );
    Msg.true           := Get6Bit(State.six_state, 9  );
    Msg.utc_sec        := Get6Bit(State.six_state, 6  );
    Msg.regional2      := Get6Bit(State.six_state, 2  );
    Msg.unit_flag      := Get6Bit(State.six_state, 1  );
    Msg.display_flag   := Get6Bit(State.six_state, 1  );
    Msg.dsc_flag       := Get6Bit(State.six_state, 1  );
    Msg.band_flag      := Get6Bit(State.six_state, 1  );
    Msg.msg22_flag     := Get6Bit(State.six_state, 1  );
    Msg.mode_flag      := Get6Bit(State.six_state, 1  );
    Msg.raim           := Get6Bit(State.six_state, 1  );
    Msg.comm_state     := Get6Bit(State.six_state, 1  );

    If (Msg.comm_state = 0) Then
    Begin
      Msg.sotdma.sync_state   := Get6Bit(State.six_state, 2  );
      Msg.sotdma.slot_timeout := Get6Bit(State.six_state, 3  );
      Msg.sotdma.sub_message  := Get6Bit(State.six_state, 14 );
    End
    Else
    Begin
      Msg.itdma.sync_state    := Get6Bit(State.six_state, 2  );
      Msg.itdma.slot_inc      := Get6Bit(State.six_state, 13 );
      Msg.itdma.num_slots     := Get6Bit(State.six_state, 3  );
      Msg.itdma.keep_flag     := Get6Bit(State.six_state, 1  );
    End;

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 19 into an aismsg_19 structure

    Extended Class B Position Report

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Note that the latitude and longitude are converted to signed values
    before being returned. And the ship's name is converted to ASCII
    before being stored.

   ----------------------------------------------------------------------- *)
Function ParseAIS_19(Var State: TAIS_State; Var Msg: TAISMsg_19): Integer;
Var
  I: Integer;
Begin
  If StrLen(State.six_state.Bits) <> 52 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 19
    Msg.isrepeat       := Get6Bit(State.six_state, 2  );
    Msg.userid         := Get6Bit(State.six_state, 30 );
    Msg.regional1      := Get6Bit(State.six_state, 8  );
    Msg.sog            := Get6Bit(State.six_state, 10 );
    Msg.pos_acc        := Get6Bit(State.six_state, 1  );
    Msg.longitude      := Get6Bit(State.six_state, 28 );
    Msg.latitude       := Get6Bit(State.six_state, 27 );
    Msg.cog            := Get6Bit(State.six_state, 12 );
    Msg.true           := Get6Bit(State.six_state, 9  );
    Msg.utc_sec        := Get6Bit(State.six_state, 6  );
    Msg.regional2      := Get6Bit(State.six_state, 4  );

    // Get the Ship Name, convert to ASCII
    For I := 0 To 19 Do
      Msg.name[I] := AIS2ASCII(Get6Bit(State.six_state, 6 ));
    Msg.name[20] := #0;

    Msg.ship_type    :=  Get6Bit(State.six_state, 8  );
    Msg.dim_bow      :=  Get6Bit(State.six_state, 9  );
    Msg.dim_stern    :=  Get6Bit(State.six_state, 9  );
    Msg.dim_port     :=  Get6Bit(State.six_state, 6  );
    Msg.dim_starboard:=  Get6Bit(State.six_state, 6  );
    Msg.pos_type     :=  Get6Bit(State.six_state, 4  );
    Msg.raim         :=  Get6Bit(State.six_state, 1  );
    Msg.dte          :=  Get6Bit(State.six_state, 1  );
    Msg.spare        :=  Get6Bit(State.six_state, 5  );

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 20 into an aismsg_20 structure

    Data link management message

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Depending on the length of the message some fields may be 0.
    result->num_cmds will have the number of commands received.

   ----------------------------------------------------------------------- *)
Function ParseAIS_20(Var State: TAIS_State; Var Msg: TAISMsg_20): Integer;
Var
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 72) Or (Len > 162) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 20
    Msg.isrepeat       := Get6Bit(State.six_state, 2  );
    Msg.userid         := Get6Bit(State.six_state, 30 );
    Msg.spare1         := Get6Bit(State.six_state, 2  );
    Msg.offset1        := Get6Bit(State.six_state, 12 );
    Msg.slots1         := Get6Bit(State.six_state, 4  );
    Msg.timeout1       := Get6Bit(State.six_state, 3  );
    Msg.increment1     := Get6Bit(State.six_state, 11 );
    Msg.num_cmds       := 1;

    If (Len > 72) Then
    Begin
      Msg.offset2    := Get6Bit(State.six_state, 12 );
      Msg.slots2     := Get6Bit(State.six_state, 4  );
      Msg.timeout2   := Get6Bit(State.six_state, 3  );
      Msg.increment2 := Get6Bit(State.six_state, 11 );
      Msg.num_cmds   := 2;
    End;

    If (Len > 104) Then
    Begin
      Msg.offset3    := Get6Bit(State.six_state, 12 );
      Msg.slots3     := Get6Bit(State.six_state, 4  );
      Msg.timeout3   := Get6Bit(State.six_state, 3  );
      Msg.increment3 := Get6Bit(State.six_state, 11 );
      Msg.num_cmds   := 3;
    End;

    If (Len > 136) Then
    Begin
      Msg.offset4    := Get6Bit(State.six_state, 12 );
      Msg.slots4     := Get6Bit(State.six_state, 4  );
      Msg.timeout4   := Get6Bit(State.six_state, 3  );
      Msg.increment4 := Get6Bit(State.six_state, 11 );
      Msg.num_cmds   := 4;
    End;

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 21 into an aismsg_21 structure

    Aids To Navigation position report (AtoN)

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Note that the latitude and longitude are converted to signed values
    before being returned. And the AtoN name and extended name are converted
    to ASCII before storage.

   ----------------------------------------------------------------------- *)
Function ParseAIS_21(Var State: TAIS_State; Var Msg: TAISMsg_21): Integer;
Var
  I,
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len  < 272) Or (Len > 360) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 21
    Msg.isrepeat       := Get6Bit(State.six_state, 2  );
    Msg.userid         := Get6Bit(State.six_state, 30 );
    Msg.aton_type      := Get6Bit(State.six_state, 5  );

    // Get the AtoN name, convert to ASCII
    For I := 0 To 19 Do
      Msg.name[I] := AIS2ASCII(Get6Bit(State.six_state, 6 ));
    Msg.name[20] := #0;

    Msg.pos_acc       := Get6Bit(State.six_state, 1  );
    Msg.longitude     := Get6Bit(State.six_state, 28 );
    Msg.latitude      := Get6Bit(State.six_state, 27 );
    Msg.dim_bow       := Get6Bit(State.six_state, 9  );
    Msg.dim_stern     := Get6Bit(State.six_state, 9  );
    Msg.dim_port      := Get6Bit(State.six_state, 6  );
    Msg.dim_starboard := Get6Bit(State.six_state, 6  );
    Msg.pos_type      := Get6Bit(State.six_state, 4  );
    Msg.utc_sec       := Get6Bit(State.six_state, 6  );
    Msg.off_position  := Get6Bit(State.six_state, 1  );
    Msg.regional      := Get6Bit(State.six_state, 8  );
    Msg.raim          := Get6Bit(State.six_state, 1  );
    Msg.virtual       := Get6Bit(State.six_state, 1  );
    Msg.assigned      := Get6Bit(State.six_state, 1  );
    Msg.spare1        := Get6Bit(State.six_state, 1  );

    If (Len > 272) Then
    Begin
      // Get the extended name 1-14 characters
      I := 0;
      While (I < ((Len - 272) / 6)) Do
      Begin
        Msg.name_ext[I] := AIS2ASCII(Get6Bit(State.six_state, 6));
        Inc(I);
      End;
      Msg.name_ext[I] := #0;
    End;

    // Convert the position to signed value
    ConvPos(Msg.latitude, Msg.longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 22 into an aismsg_22 structure

    Channel Management

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Check the addressed flag to decide whether to use the positions in
    NE and SW or the addresses in addressed_1 and addressed_2

    Note that the latitudes and longitudes are converted to signed values
    before being returned and that they are converted to 1/10000 minute
    format from 1/1000 minute as received.

   ----------------------------------------------------------------------- *)
Function ParseAIS_22(Var State: TAIS_State; Var Msg: TAISMsg_22): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 22
    Msg.isrepeat       := Get6Bit(State.six_state, 2  );
    Msg.userid         := Get6Bit(State.six_state, 30 );
    Msg.spare1         := Get6Bit(State.six_state, 1  );
    Msg.channel_a      := Get6Bit(State.six_state, 12 );
    Msg.channel_b      := Get6Bit(State.six_state, 12 );
    Msg.txrx_mode      := Get6Bit(State.six_state, 4  );
    Msg.power          := Get6Bit(State.six_state, 1  );
    Msg.NE_longitude   := Get6Bit(State.six_state, 18 );
    Msg.NE_latitude    := Get6Bit(State.six_state, 17 );
    Msg.SW_longitude   := Get6Bit(State.six_state, 18 );
    Msg.SW_latitude    := Get6Bit(State.six_state, 17 );
    Msg.addressed      := Get6Bit(State.six_state, 1  );
    Msg.bw_a           := Get6Bit(State.six_state, 1  );
    Msg.bw_b           := Get6Bit(State.six_state, 1  );
    Msg.tz_size        := Get6Bit(State.six_state, 3  );

    // Is the position actually an address?
    If (Msg.addressed <> 0) Then
    Begin
      // Convert the positions to addresses
      Msg.addressed_1 := (Msg.NE_longitude Shl 12) + (Msg.NE_latitude Shr 5);
      Msg.addressed_2 := (Msg.SW_longitude Shl 12) + (Msg.SW_latitude Shr 5);
    End
    Else
    Begin
      // Convert 1/10 to 1/10000 before conversion
      Msg.NE_longitude := Msg.NE_longitude * 1000;
      Msg.NE_latitude  := Msg.NE_latitude * 1000;

      Msg.SW_longitude := Msg.SW_longitude * 1000;
      Msg.SW_latitude  := Msg.SW_latitude * 1000;
    End;

    // Convert the position to signed value
    ConvPos(Msg.NE_latitude, Msg.NE_longitude);
    ConvPos(Msg.SW_latitude, Msg.SW_longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 23 into an aismsg_23 structure

    Group Assignment Command

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error

    Note that the latitudes and longitudes are converted to signed values
    before being returned and that they are converted to 1/10000 minute
    format from 1/1000 minute as received.

   ----------------------------------------------------------------------- *)
Function ParseAIS_23(Var State: TAIS_State; Var Msg: TAISMsg_23): Integer;
Begin
  If StrLen(State.six_state.Bits) <> 28 Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 23
    Msg.isrepeat       := Get6Bit(State.six_state, 2  );
    Msg.userid         := Get6Bit(State.six_state, 30 );
    Msg.spare1         := Get6Bit(State.six_state, 2  );
    Msg.NE_longitude   := Get6Bit(State.six_state, 18 );
    Msg.NE_latitude    := Get6Bit(State.six_state, 17 );
    Msg.SW_longitude   := Get6Bit(State.six_state, 18 );
    Msg.SW_latitude    := Get6Bit(State.six_state, 17 );
    Msg.station_type   := Get6Bit(State.six_state, 4  );
    Msg.ship_type      := Get6Bit(State.six_state, 8  );
    Msg.spare2         := Get6Bit(State.six_state, 22 );
    Msg.txrx_mode      := Get6Bit(State.six_state, 2  );
    Msg.report_interval:= Get6Bit(State.six_state, 4  );
    Msg.quiet_time     := Get6Bit(State.six_state, 4  );
    Msg.spare3         := Get6Bit(State.six_state, 6  );

    Msg.NE_longitude := Msg.NE_longitude * 1000;
    Msg.NE_latitude  := Msg.NE_latitude * 1000;
    Msg.SW_longitude := Msg.SW_longitude * 1000;
    Msg.SW_latitude  := Msg.SW_latitude * 1000;

    // Convert the position to signed value
    ConvPos(Msg.NE_latitude, Msg.NE_longitude);
    ConvPos(Msg.SW_latitude, Msg.SW_longitude);

    Result := 0;
  End;
End;

(* -----------------------------------------------------------------------
    Parse an AIS message 24 into an aismsg_24 structure

    Class B"CS" Static Data Report

    \param state    pointer to ais_state
    \param result   pointer to parsed message result structure to be filled

    return:
      - 0 if no errors
      - 1 if there is an error
      - 2 if there is a packet length error
      - 3 if unknown part number

     Message 24 is a 2 part message. The first part only contains the MMSI
     and the ship name. The second message contains the ship dimensions,
     etc.

     Check the result->part_number field to determine which message this
     is. The same structure is used for both messages and the result->flags
     field will have a 0x03 in it when both messages have been parsed.

     The ship name, vendor id and callsign are all converted to ASCII before
     storage.

     Note that the latitude and longitude are converted to signed values
     before being returned

   ----------------------------------------------------------------------- *)
Function ParseAIS_24(Var State: TAIS_State; Var Msg: TAISMsg_24): Integer;
Var
  I,
  Len: Integer;
Begin
  Len := StrLen(State.six_state.Bits) * 6;
  If (Len <> 162) And (Len <> 168) Then
    Result := 2
  Else
  Begin
    // Clear out the structure first
    FillChar(Msg, SizeOf(Msg), 0);

    Msg.msgid := State.msgid;

    // Parse the Message 24
    Msg.isrepeat       := Get6Bit(State.six_state, 2  );
    Msg.userid         := Get6Bit(State.six_state, 30 );
    Msg.part_number    := Get6Bit(State.six_state, 2  );

    If (Msg.part_number = 0) Then
    Begin
      // Parse 24A
      // Get the Ship Name, convert to ASCII
      For I := 0 To 19 Do
        Msg.name[I] := AIS2ASCII(Get6Bit(State.six_state, 6 ));
      Msg.name[20] := #0;

      // Indicate reception of part A
      Msg.flags := Msg.flags Or $01;
    End
    Else If (Msg.part_number = 1) Then
    Begin
      // Parse 24B
      Msg.ship_type := Get6Bit(State.six_state, 8  );

      For I := 0 To 6 Do
        Msg.vendor_id[I] := AIS2ASCII(Get6Bit(State.six_state, 6 ));
      Msg.name[7] := #0;

      For I := 0 To 6 Do
        Msg.callsign[I] := AIS2ASCII(Get6Bit(State.six_state, 6 ));
      Msg.name[7] := #0;

      Msg.dim_bow      := Get6Bit(State.six_state, 9  );
      Msg.dim_stern    := Get6Bit(State.six_state, 9  );
      Msg.dim_port     := Get6Bit(State.six_state, 6  );
      Msg.dim_starboard:= Get6Bit(State.six_state, 6  );
      Msg.spare        := Get6Bit(State.six_state, 6  );

      // Indicate reception of part A
      Msg.flags := Msg.flags Or $02;
    End
    Else
    Begin
      Result := 3;
      Exit;
    End;

    Result := 0;
  End;
End;

End.
