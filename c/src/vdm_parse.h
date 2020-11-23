/* -----------------------------------------------------------------------
   libuais vdm/vdo sentence parsing
   Copyright 2006-2020 by Brian C. Lane <bcl@brianlane.com>
   All rights Reserved
   ----------------------------------------------------------------------- */

/*! \file
    \brief Header file for vdm_parse.c

    Structures for each AIS message type
    and a structure to hold position information

    For exact definitions of each field you should see the ITU M.1371,
    IEC 62287 and the IALA Clarifications on M.1371
    The format of the !AIVDM strings are covered in IEC 61993-2
*/

/* Length of 6-bit data buffer */
#define SIXBUF_LEN  255


/** AIS Message 1 - Position Report with SOTDMA
*/
typedef struct {
    char            msgid;             //!< 6 bits  : Message ID (1)
    char            repeat;            //!< 2 bits  : Repeated
    unsigned long   userid;            //!< 30 bits : UserID / MMSI
    char            nav_status;        //!< 4 bits  : Navigational Status
    unsigned char   rot;               //!< 8 bits  : Rate of Turn
    int             sog;               //!< 10 bits : Speed Over Ground
    char            pos_acc;           //!< 1 bit   : Position Accuracy
    long            longitude;         //!< 28 bits : Longitude in 1/10000 minute
    long            latitude;          //!< 27 bits : Latitude in 1/10000 minute
    int             cog;               //!< 12 bits : Course over Ground
    int             true;              //!< 9 bits  : True heading
    char            utc_sec;           //!< 6 bits  : UTC Seconds
    char            regional;          //!< 4 bits  : Regional bits
    char            spare;             //!< 1 bit   : Spare
    char            raim;              //!< 1 bit   : RAIM flag
    char            sync_state;        //!< 2 bits  : SOTDMA sync state
    char            slot_timeout;      //!< 3 bits  : SOTDMA Slot Timeout
    int             sub_message;       //!< 14 bits : SOTDMA sub-message
} aismsg_1;


/** AIS Message 2 - Position Report with SOTDMA
*/
typedef struct {
    char            msgid;             //!< 6 bits  : Message ID (2)
    char            repeat;            //!< 2 bits  : Repeated
    unsigned long   userid;            //!< 30 bits : UserID / MMSI
    char            nav_status;        //!< 4 bits  : Navigational Status
    unsigned char   rot;               //!< 8 bits  : Rate of Turn
    int             sog;               //!< 10 bits : Speed Over ground
    char            pos_acc;           //!< 1 bit   : Position Accuracy
    long            longitude;         //!< 28 bits : Longitude in 1/10000 minute
    long            latitude;          //!< 27 bits : Latitude in 1/10000 minute
    int             cog;               //!< 12 bits : Course over Ground
    int             true;              //!< 9 bits  : True Heading
    char            utc_sec;           //!< 6 bits  : UTC Seconds
    char            regional;          //!< 4 bits  : Regional bits
    char            spare;             //!< 1 bit   : Spare
    char            raim;              //!< 1 bit   : RAIM flag
    char            sync_state;        //!< 2 bits  : SOTDMA sync state
    char            slot_timeout;      //!< 3 bits  : SOTDMA Slot Timeout
    int             sub_message;       //!< 14 bits : SOTDMA sub-message
} aismsg_2 ;


/** AIS Message 3 - Position Report with ITDMA
*/
typedef struct {
    char            msgid;             //!< 6 bits  : MessageID (3)
    char            repeat;            //!< 2 bits  : Repeated
    unsigned long   userid;            //!< 30 bits : UserID / MMSI
    char            nav_status;        //!< 4 bits  : Navigational Status
    unsigned char   rot;               //!< 8 bits  : Rate of Turn
    int             sog;               //!< 10 bits : Speed Over Ground
    char            pos_acc;           //!< 1 bit   : Position Accuracy
    long            longitude;         //!< 28 bits : Longitude in 1/10000 minute
    long            latitude;          //!< 27 bits : Latitude in 1/10000 minute
    int             cog;               //!< 12 bits : Course over Ground
    int             true;              //!< 9 bits  : True Heading
    char            utc_sec;           //!< 6 bits  : UTC Seconds
    char            regional;          //!< 4 bits  : Regional bits
    char            spare;             //!< 1 bit   : Spare
    char            raim;              //!< 1 bit   : RAIM Flag
    char            sync_state;        //!< 2 bits  : ITDMA sync state
    int             slot_increment;    //!< 13 bits : ITDMA Slot Increment
    char            num_slots;         //!< 3 bits  : ITDMA Number of Slots
    char            keep;              //!< 1 bit   : ITDMA Keep Flag
} aismsg_3 ;


/** AIS Message 4 - Base Station Report
*/
typedef struct {
    char            msgid;             //!< 6 bits  : MessageID (4)
    char            repeat;            //!< 2 bits  : Repeated
    unsigned long   userid;            //!< 30 bits : UserID / MMSI
    int             utc_year;          //!< 14 bits : UTC Year
    char            utc_month;         //!< 4 bits  : UTC Month
    char            utc_day;           //!< 5 bits  : UTC Day
    char            utc_hour;          //!< 5 bits  : UTC Hour
    char            utc_minute;        //!< 6 bits  : UTC Minute
    char            utc_second;        //!< 6 bits  : UTC Second
    char            pos_acc;           //!< 1 bit   : Position Accuracy
    long            longitude;         //!< 28 bits : Longitude in 1/10000 minute
    long            latitude;          //!< 27 bits : Latitude in 1/10000 minute
    char            pos_type;          //!< 4 bits  : Type of position fixing device
    int             spare;             //!< 10 bits : Spare
    char            raim;              //!< 1 bit   : RAIM flag
    char            sync_state;        //!< 2 bits  : SOTDMA sync state
    char            slot_timeout;      //!< 3 bits  : SOTDMA Slot Timeout
    int             sub_message;       //!< 14 bits : SOTDMA sub-message
} aismsg_4 ;


/** AIS Message 5 - Ship static and voyage related data
*/
typedef struct {
    char            msgid;             //!< 6 bits  : MessageID (5)
    char            repeat;            //!< 2 bits  : Repeated
    unsigned long   userid;            //!< 30 bits : UserID / MMSI
    char            version;           //!< 2 bits  : AIS Version
    unsigned long   imo;               //!< 30 bits : IMO Number
    char            callsign[8];       //!< 7x6 (42) bits : Callsign
    char            name[21];          //!< 20x6 (120) bits : Ship Name
    unsigned char   ship_type;         //!< 8 bits  : Type of Ship and Cargo
    int             dim_bow;           //!< 9 bits  : GPS Ant. Distance from Bow
    int             dim_stern;         //!< 9 bits  : GPS Ant. Distance from stern
    char            dim_port;          //!< 6 bits  : GPS Ant. Distance from port
    char            dim_starboard;     //!< 6 bits  : GPS Ant. Distance from starboard
    char            pos_type;          //!< 4 bits  : Type of position fixing device
    unsigned long   eta;               //!< 20 bits : Estimated Time of Arrival MMDDHHMM
    unsigned char   draught;           //!< 8 bits  : Maximum present static draught
    char            dest[21];          //!< 6x20 (120) bits : Ship Destination
    char            dte;               //!< 1 bit   : DTE flag
    char            spare;             //!< 1 bit   : spare
} aismsg_5;


/** AIS Message 6 - Addressed Binary Message
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (6)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            sequence;          //!< 2 bits   : Sequence number
    unsigned long   destination;       //!< 30 bits  : Destination MMSI
    char            retransmit;        //!< 1 bit    : Retransmit
    char            spare;             //!< 1 bit    : Spare
    unsigned int    app_id;            //!< 16 bits  : Application ID
    sixbit          data;              //!< 960 bits : Data payload
} aismsg_6 ;


/** AIS Message 7 - Binary Acknowledge
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (7)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare;             //!< 2 bits   : Spare
    unsigned long   destid_1;          //!< 30 bits  : Destination MMSI 1
    char            sequence_1;        //!< 2 bits   : Sequence Number 1
    unsigned long   destid_2;          //!< 30 bits  : Destination MMSI 2
    char            sequence_2;        //!< 2 bits   : Sequence Number 2
    unsigned long   destid_3;          //!< 30 bits  : Destination MMSI 3
    char            sequence_3;        //!< 2 bits   : Sequence Number 3
    unsigned long   destid_4;          //!< 30 bits  : Destination MMSI 4
    char            sequence_4;        //!< 2 bits   : Sequence Number 4
    char            num_acks;          //!< Number of acks
} aismsg_7;


/** AIS Message 8 - Binary Broadcast Message
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (8)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare;             //!< 2 bits   : Spare
    unsigned int    app_id;            //!< 16 bits  : Application ID
    sixbit          data;              //!< 952 bits : Data payload
} aismsg_8;


/** AIS Message 9 - Standard SAR Aircraft position report
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (9)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    int             altitude;          //!< 12 bits  : Altitude
    int             sog;               //!< 10 bits  : Speed Over Ground
    char            pos_acc;           //!< 1 bit    : Position Accuracy
    long            longitude;         //!< 28 bits  : Longitude in 1/10000 minute
    long            latitude;          //!< 27 bits  : Latitude in 1/10000 minute
    int             cog;               //!< 12 bits  : Course Over Ground
    char            utc_sec;           //!< 6 bits   : UTC Seconds
    unsigned char   regional;          //!< 8 bits   : Regional bits
    char            dte;               //!< 1 bit    : DTE flag
    char            spare;             //!< 3 bits   : Spare
    char            assigned;          //!< 1 bit    : Assigned mode flag
    char            raim;              //!< 1 bit    : RAIM flag
    char            comm_state;        //!< 1 bit    : Comm State Flag
    struct {
        char          sync_state;      //!< 2 bits   : SOTDMA Sync State
        char          slot_timeout;    //!< 3 bits   : SOTDMA Slot Timeout
        int           sub_message;     //!< 14 bits  : SOTDMA Sub-Messsage
    } sotdma;
    struct {
        char          sync_state;      //!< 2 bits   : ITDMA Sync State
        int           slot_inc;        //!< 13 bits  : ITDMA Slot Increment
        char          num_slots;       //!< 3 bits   : ITDMA Number of Slots
        char          keep_flag;       //!< 1 bit    : ITDMA Keep Flag
    } itdma;
} aismsg_9;


/** AIS Message 10 - UTC and Date inquiry
*/
typedef struct  {
    char            msgid;             //!< 6 bits   : MessageID (10)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare1;            //!< 2 bits   : Spare
    unsigned long   destination;       //!< 30 bits  : Destination MMSI
    char            spare2;            //!< 2 bits   : Spare
} aismsg_10;


/** AIS Message 11 - UTC and Date response
*/
typedef struct {
    char            msgid;             //!< 6 bits  : MessageID (4)
    char            repeat;            //!< 2 bits  : Repeated
    unsigned long   userid;            //!< 30 bits : UserID / MMSI
    int             utc_year;          //!< 14 bits : UTC Year
    char            utc_month;         //!< 4 bits  : UTC Month
    char            utc_day;           //!< 5 bits  : UTC Day
    char            utc_hour;          //!< 5 bits  : UTC Hour
    char            utc_minute;        //!< 6 bits  : UTC Minute
    char            utc_second;        //!< 6 bits  : UTC Second
    char            pos_acc;           //!< 1 bit   : Position Accuracy
    long            longitude;         //!< 28 bits : Longitude in 1/10000 minute
    long            latitude;          //!< 27 bits : Latitude in 1/10000 minute
    char            pos_type;          //!< 4 bits  : Type of position fixing device
    int             spare;             //!< 10 bits : Spare
    char            raim;              //!< 1 bit   : RAIM flag
    char            sync_state;        //!< 2 bits  : SOTDMA sync state
    char            slot_timeout;      //!< 3 bits  : SOTDMA Slot Timeout
    int             sub_message;       //!< 14 bits : SOTDMA sub-message
} aismsg_11;


/** AIS Message 12 - Addressed safety related message
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (12)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            sequence;          //!< 2 bits   : Sequence
    unsigned long   destination;       //!< 30 bits  : Destination MMSI
    char            retransmit;        //!< 1 bit    : Retransmit
    char            spare;             //!< 1 bit    : Spare
    char            message[158];      //!< 936 bits : Message in ASCII
} aismsg_12;


/** AIS Message 13 - Safety related Acknowledge
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (13)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare;             //!< 2 bits   : Spare
    unsigned long   destid_1;          //!< 30 bits  : Destination MMSI 1
    char            sequence_1;        //!< 2 bits   : Sequence Number 1
    unsigned long   destid_2;          //!< 30 bits  : Destination MMSI 2
    char            sequence_2;        //!< 2 bits   : Sequence Number 2
    unsigned long   destid_3;          //!< 30 bits  : Destination MMSI 3
    char            sequence_3;        //!< 2 bits   : Sequence Number 3
    unsigned long   destid_4;          //!< 30 bits  : Destination MMSI 4
    char            sequence_4;        //!< 2 bits   : Sequence Number 4
    char            num_acks;          //!< Number of acks
} aismsg_13;


/** AIS Message 14 - Safety related Broadcast Message
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (14)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare;             //!< 2 bits   : Spare
    char            message[164];      //!< 968 bits : Message in ASCII
} aismsg_14;


/** AIS Message 15 - Interrogation
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (15)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare1;            //!< 2 bits   : Spare
    unsigned long   destid1;           //!< 30 bits  : Destination MMSI 1
    char            msgid1_1;          //!< 6 bits   : MessageID 1.1
    int             offset1_1;         //!< 12 bits  : Slot Offset 1.1
    char            spare2;            //!< 2 bits   : Spare
    char            msgid1_2;          //!< 6 bits   : MessageID 1.2
    int             offset1_2;         //!< 12 bits  : Slot Offset 1.2
    char            spare3;            //!< 2 bits   : Spare
    unsigned long   destid2;           //!< 30 bits  : Destination MMSI 2
    char            msgid2_1;          //!< 6 bits   : MessageID 2.1
    int             offset2_1;         //!< 12 bits  : Slot Offset 2.1
    char            spare4;            //!< 2 bits   : Spare
    char            num_reqs;          //!< Number of interrogation requests
} aismsg_15;


/** AIS Message 16 - Assigned Mode Command
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (16)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare1;            //!< 2 bits   : Spare
    unsigned long   destid_a;          //!< 30 bits  : Destination MMSI A
    int             offset_a;          //!< 12 bits  : Slot Offset A
    int             increment_a;       //!< 10 bits  : Increment A
    unsigned long   destid_b;          //!< 30 bits  : Destination MMSI B
    int             offset_b;          //!< 12 bits  : Slot Offset B
    int             increment_b;       //!< 10 bits  : Increment B
    char            spare2;            //!< 4 bits   : Spare
    char            num_cmds;          //!< Number of commands received
} aismsg_16;


/** AIS Message 17 - GNSS Binary Broadcast Message
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (17)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare1;            //!< 2 bits   : Spare
    long            longitude;         //!< 18 bits  : Longitude in 1/1000 minute
    long            latitude;          //!< 17 bits  : Latitude in 1/1000 minute
    char            spare2;            //!< 5 bits   : Spare
    char            msg_type;          //!< 6 bits   : Mesage Type from M.823
    int             station_id;        //!< 10 bits  : Station ID from M.823
    int             z_count;           //!< 13 bits  : Z Count
    char            seq_num;           //!< 3 bits   : Sequence Number
    char            num_words;         //!< 5 bits   : Number of Data Words
    char            health;            //!< 3 bits   : Reference Station Health from M.823
    sixbit          data;              //!< 0-696 bits  : Data payload
} aismsg_17;


/** AIS Message 18 - Standard Class B CS Position Report

    From IEC 62287. This differs slightly from the original
    message, some of the regional bits were changed to flags.
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (18)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    unsigned char   regional1;         //!< 8 bits   : Regional Bits
    int             sog;               //!< 10 bits  : Speed Over Ground
    char            pos_acc;           //!< 1 bit    : Position Accuracy
    long            longitude;         //!< 28 bits  : Longitude in 1/10000 minute
    long            latitude;          //!< 27 bits  : Latitude in 1/10000 minute
    int             cog;               //!< 12 bits  : Course Over Ground
    int             true;              //!< 9 bits   : True Heading
    char            utc_sec;           //!< 6 bits   : UTC Seconds
    char            regional2;         //!< 2 bits   : Regional Bits
    char            unit_flag;         //!< 1 bit    : Class B CS Flag
    char            display_flag;      //!< 1 bit    : Integrated msg14 Display Flag
    char            dsc_flag;          //!< 1 bit    : DSC Capability flag
    char            band_flag;         //!< 1 bit    : Marine Band Operation Flag
    char            msg22_flag;        //!< 1 bit    : Msg22 Frequency Management Flag
    char            mode_flag;         //!< 1 bit    : Autonomous Mode Flag
    char            raim;              //!< 1 bit    : RAIM Flag
    char            comm_state;        //!< 1 bit    : Comm State Flag
    struct {
        char          sync_state;      //!< 2 bits   : SOTDMA Sync State
        char          slot_timeout;    //!< 3 bits   : SOTDMA Slot Timeout
        int           sub_message;     //!< 14 bits  : SOTDMA Sub-Message
    } sotdma;
    struct {
        char          sync_state;      //!< 2 bits   : ITDMA Sync State
        int           slot_inc;        //!< 13 bits  : ITDMA Slot Increment
        char          num_slots;       //!< 3 bits   : ITDMA Number of Slots
        char          keep_flag;       //!< 1 bit    : ITDMA Keep Flag
    } itdma;
} aismsg_18;


/** AIS Message 19 - Extended Class B Equipment Position Report
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (19)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            regional1;         //!< 8 bits   : Regional Bits
    int             sog;               //!< 10 bits  : Speed Over Ground
    char            pos_acc;           //!< 1 bit    : Position Accuracy
    long            longitude;         //!< 28 bits  : Longitude in 1/10000 minute
    long            latitude;          //!< 27 bits  : Latitude in 1/10000 minute
    int             cog;               //!< 12 bits  : Course Over Ground
    int             true;              //!< 9 bits   : True Heading
    char            utc_sec;           //!< 6 bits   : UTC Seconds
    char            regional2;         //!< 4 bits   : Regional Bits
    char            name[21];          //!< 120 bits : Ship Name in ASCII
    unsigned char   ship_type;         //!< 8 bits   : Type of Ship and Cargo
    int             dim_bow;           //!< 9 bits   : GPS Ant. Distance from Bow
    int             dim_stern;         //!< 9 bits   : GPS Ant. Distance from Stern
    char            dim_port;          //!< 6 bits   : GPS Ant. Distance from Port
    char            dim_starboard;     //!< 6 bits   : GPS Ant. Distance from Starboard
    char            pos_type;          //!< 4 bits   : Type of Position Fixing Device
    char            raim;              //!< 1 bit    : RAIM Flag
    char            dte;               //!< 1 bit    : DTE Flag
    char            spare;             //!< 5 bits   : Spare
} aismsg_19;


/** AIS Message 20 - Data Link Management
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (20)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare1;            //!< 2 bits   : Spare
    int             offset1;           //!< 12 bits  : Slot Offset 1
    char            slots1;            //!< 4 bits   : Number of Slots 1
    char            timeout1;          //!< 3 bits   : Timeout in Minutes 2
    int             increment1;        //!< 11 bits  : Slot Increment 1
    int             offset2;           //!< 12 bits  : Slot Offset 2
    char            slots2;            //!< 4 bits   : Number of Slots 2
    char            timeout2;          //!< 3 bits   : Timeout in Minutes 2
    int             increment2;        //!< 11 bits  : Slot Increment 2
    int             offset3;           //!< 12 bits  : Slot Offset 3
    char            slots3;            //!< 4 bits   : Number of Slots 3
    char            timeout3;          //!< 3 bits   : Timeout in Minutes 3
    int             increment3;        //!< 11 bits  : Slot Increment 3
    int             offset4;           //!< 12 bits  : Slot Offset 4
    char            slots4;            //!< 4 bits   : Number of Slots 4
    char            timeout4;          //!< 3 bits   : Timeout in Minutes 4
    int             increment4;        //!< 11 bits  : Slot Increment 4
    char            spare2;            //!< 0-6 bits : Spare
    char            num_cmds;          //!< Number of commands received
} aismsg_20;


/** AIS Message 21 - Aids-to-navigation Report
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (21)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            aton_type;         //!< 5 bits   : Type of AtoN
    char            name[21];          //!< 120 bits : Name of AtoN in ASCII
    char            pos_acc;           //!< 1 bit    : Position Accuracy
    long            longitude;         //!< 28 bits  : Longitude in 1/10000 minutes
    long            latitude;          //!< 27 bits  : Latitude in 1/10000 minutes
    int             dim_bow;           //!< 9 bits   : GPS Ant. Distance from Bow
    int             dim_stern;         //!< 9 bits   : GPS Ant. Distance from Stern
    char            dim_port;          //!< 6 bits   : GPS Ant. Distance from Port
    char            dim_starboard;     //!< 6 bits   : GPS Ant. Distance from Starboard
    char            pos_type;          //!< 4 bits   : Type of Position Fixing Device
    char            utc_sec;           //!< 6 bits   : UTC Seconds
    char            off_position;      //!< 1 bit    : Off Position Flag
    unsigned char   regional;          //!< 8 bits   : Regional Bits
    char            raim;              //!< 1 bit    : RAIM Flag
    char            virtual;           //!< 1 bit    : Virtual/Pseudo AtoN Flag
    char            assigned;          //!< 1 bit    : Assigned Mode Flag
    char            spare1;            //!< 1 bit    : Spare
    char            name_ext[16];      //!< 0-84 bits : Extended name in ASCII
    char            spare2;            //!< 0-6 bits : Spare
} aismsg_21;


/** AIS Message 22 - Channel Management

    If the message is for a geographic area its 2 corners are
    defined by the NE and SW positions. If it is an assigned
    message the destination MMSI will be in addressed_1 and
    addressed_2.
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (22)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare1;            //!< 2 bits   : Spare
    int             channel_a;         //!< 12 bits  : M.1084 Channel A Frequency
    int             channel_b;         //!< 12 bits  : M.1084 Channel B Frequency
    char            txrx_mode;         //!< 4 bits   : TX/RX Mode
    char            power;             //!< 1 bit    : Power Level
    long            NE_longitude;      //!< 18 bits  : Longitude in 1/1000 minutes
    long            NE_latitude;       //!< 17 bits  : Latitude in 1/1000 minutes
    unsigned long   addressed_1;       //!< 30 bits  : Destination MMSI 1
    long            SW_longitude;      //!< 18 bits  : Longitude in 1/1000 minutes
    long            SW_latitude;       //!< 17 bits  : Latitude in 1/1000 minutes
    unsigned long   addressed_2;       //!< 30 bits  : Destination MMSI 2
    char            addressed;         //!< 1 bit    : Addressed flag
    char            bw_a;              //!< 1 bit    : Channel A Bandwidth
    char            bw_b;              //!< 1 bit    : Channel B Bandwidth
    char            tz_size;           //!< 3 bits   : Transitional Zone size
    unsigned long   spare2;            //!< 23 bits  : Spare
} aismsg_22 ;


/** AIS Message 23 - Group Assignment Command

    The geographic area is defined by 2 corners, stored in
    the NE and SW positions.
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (23)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            spare1;            //!< 2 bits   : Spare
    long            NE_longitude;      //!< 18 bits  : Longitude in 1/1000 minutes
    long            NE_latitude;       //!< 17 bits  : Latitude in 1/1000 minutes
    long            SW_longitude;      //!< 18 bits  : Longitude in 1/1000 minutes
    long            SW_latitude;       //!< 17 bits  : Latitude in 1/1000 minutes
    char            station_type;      //!< 4 bits   : Station Type
    unsigned char   ship_type;         //!< 8 bits   : Type of Ship and Cargo
    unsigned long   spare2;            //!< 22 bits  : Spare
    char            txrx_mode;         //!< 2 bits   : TX/RX Mode
    char            report_interval;   //!< 4 bits   : Reporting Interval from IEC 62287 Table 17
    char            quiet_time;        //!< 4 bits   : Quiet Time in Minutes
    char            spare3;            //!< 6 bits   : Spare
} aismsg_23 ;


/** AIS Message 24 - Class B CS Static Data Report

    Message 24 is a 2 part message, 24A with the name and
    24B with the dimensions, etc. This structure is used
    to hold the results.
*/
typedef struct {
    char            msgid;             //!< 6 bits   : MessageID (24)
    char            repeat;            //!< 2 bits   : Repeated
    unsigned long   userid;            //!< 30 bits  : UserID / MMSI
    char            part_number;       //!< 2 bits   : Part Number

    //!< Message 24A
    char            name[21];          //!< 120 bits : Ship Name in ASCII

    //!< Message 24B
    unsigned char   ship_type;         //!< 8 bits   : Type of Ship and Cargo
    char            vendor_id[8];      //!< 42 bits  : Vendor ID in ASCII
    char            callsign[8];       //!< 42 bits  : Callsign in ASCII
    int             dim_bow;           //!< 9 bits   : GPS Ant. Distance from Bow
    int             dim_stern;         //!< 9 bits   : GPS Ant. Distance from Stern
    char            dim_port;          //!< 6 bits   : GPS Ant. Distance from Port
    char            dim_starboard;     //!< 6 bits   : GPS Ant. Distance from Starboard
    char            spare;             //!< 6 bits   : Spare

    char            flags;             //!< A/B flags - A = 1  B = 2  Both = 3
} aismsg_24;


/* AIS Message 27 - Long Range AIS Broadcast message
*/
typedef struct {
    char            msgid;             //!< 6 bits  : Message ID (1)
    char            repeat;            //!< 2 bits  : Repeated
    unsigned long   userid;            //!< 30 bits : UserID / MMSI
    char            pos_acc;           //!< 1 bit   : Position Accuracy
    char            raim;              //!< 1 bit   : RAIM flag
    char            nav_status;        //!< 4 bits  : Navigational Status
    long            longitude;         //!< 18 bits : Longitude in minutes/10,181000 = N/A (default)
    long            latitude;          //!< 17 bits : Latitude in minutes/10,91000 = N/A (default)
    int             sog;               //!< 6 bits  : Speed Over Ground, Knots (0-62); 63 = N/A (default)
    int             cog;               //!< 9 bits  : Course over Ground,0 to 359 degrees, 511 = not available.
    char            gnss;             //!< 1 bit   : GNSS Position status,0 = current GNSS position 1 = not GNSS position (default)
    char            spare;             //!< 1 bit   : Spare
} aismsg_27;


/** ETA, Seaway and IMO UTC Timetag
*/
typedef struct {
    char            month;             //!< 4 bits   : Month
    char            day;               //!< 5 bits   : Day
    char            hours;             //!< 5 bits   : UTC Hours
    char            minutes;           //!< 6 bits   : UTC Minutes
} timetag;


/* ------------------------------------------------------------------------ */
/** ais_state the state for the Message Parser

    It keeps track partial messages until a complete message has been
    received and it hols the sixbit state for exteacting bits from the
    message.
*/
/* ------------------------------------------------------------------------ */
typedef struct {
    unsigned char msgid;               //!< Message ID 0-31
    unsigned int  sequence;            //!< VDM message sequence number
    unsigned int  total;               //!< Total # of parts for the message
    unsigned int  num;                 //!< Number of the last part stored
    char          channel;             //!< AIS Channel character
    sixbit        six_state;           //!< sixbit parser state
} ais_state;


/* ------------------------------------------------------------------------ */
/** binary_state the state for the Seaway/IMO Message Parser

	Holds the DAC, FI, msgid and the message bits in six_state
	Used for processing the data portion of a binary message
*/
/* ------------------------------------------------------------------------ */
typedef struct {
    int dac;                           //!< DAC
    char fi;                           //!< FI
    char spare;                        //!< 2 bits Spare
    char msgid;                        //!< 6 bits Message ID 0-31
    sixbit        six_state;           //!< sixbit parser state
} binary_state;


/* Prototypes */
int __stdcall get_timetag( sixbit *state, timetag *datetime );
void __stdcall conv_sign( unsigned int sign_bit, int *value );
char __stdcall ais2ascii( char value );
int __stdcall pos2ddd( long latitude, long longitude, double *lat_dd, double *long_ddd );
int __stdcall pos2dmm( long latitude, long longitude, short *lat_dd, double *lat_min, short *long_ddd, double *long_min );
int __stdcall conv_pos( long *latitude, long *longitude );
int __stdcall conv_pos27( long *latitude, long *longitude );
int __stdcall assemble_vdm( ais_state *state, char *str );
int __stdcall parse_ais_1( ais_state *state, aismsg_1 *result );
int __stdcall parse_ais_2( ais_state *state, aismsg_2 *result );
int __stdcall parse_ais_3( ais_state *state, aismsg_3 *result );
int __stdcall parse_ais_4( ais_state *state, aismsg_4 *result );
int __stdcall parse_ais_5( ais_state *state, aismsg_5 *result );
int __stdcall parse_ais_6( ais_state *state, aismsg_6 *result );
int __stdcall parse_ais_7( ais_state *state, aismsg_7 *result );
int __stdcall parse_ais_8( ais_state *state, aismsg_8 *result );
int __stdcall parse_ais_9( ais_state *state, aismsg_9 *result );
int __stdcall parse_ais_10( ais_state *state, aismsg_10 *result );
int __stdcall parse_ais_11( ais_state *state, aismsg_11 *result );
int __stdcall parse_ais_12( ais_state *state, aismsg_12 *result );
int __stdcall parse_ais_13( ais_state *state, aismsg_13 *result );
int __stdcall parse_ais_14( ais_state *state, aismsg_14 *result );
int __stdcall parse_ais_15( ais_state *state, aismsg_15 *result );
int __stdcall parse_ais_16( ais_state *state, aismsg_16 *result );
int __stdcall parse_ais_17( ais_state *state, aismsg_17 *result );
int __stdcall parse_ais_18( ais_state *state, aismsg_18 *result );
int __stdcall parse_ais_19( ais_state *state, aismsg_19 *result );
int __stdcall parse_ais_20( ais_state *state, aismsg_20 *result );
int __stdcall parse_ais_21( ais_state *state, aismsg_21 *result );
int __stdcall parse_ais_22( ais_state *state, aismsg_22 *result );
int __stdcall parse_ais_23( ais_state *state, aismsg_23 *result );
int __stdcall parse_ais_24( ais_state *state, aismsg_24 *result );
int __stdcall parse_ais_27( ais_state *state, aismsg_27 *result );
