VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command1 
      Caption         =   "Test AIS Functions"
      Height          =   495
      Left            =   1320
      TabIndex        =   0
      Top             =   240
      Width           =   2055
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()

' AIS Parser SDK Visual Basic DLL Test Functions
' Copyright 2006-2020 by Brian C. Lane
' All Rights Reserved
' http://www.aisparser.com
'
' The DLL exports all of the SDK functions and also provides a couple of
' additional routines to make life easier for the VB developer
'
' When first using the SDK a call needs to be made to InitAisparser()
' This allocates memory for the state of the parser and returns a pointer
' (as a Long) to the memory allocated.
'
' When finished using the parser (at program close) a call should be made
' to CloseAisparser( ptr ) to free up the allocated memory
'
' The flow of the parser is very similar to the 'C' examples, with the
' exception of storage of the msgid in the ais_state structure. It cannot
' be directly accessed from VB so the msgid needs to be stored locally.
'
' Please note that some message parsing functions have not been tested
' with live data yet. If you have examples of live data for untested
' messages please email them to bcl@brianlane.com for inclusion in future
' releases of the SDK
'
' Currently parsing of the payload of binary messages is not working. I have added
' a number of helper functions but have reached the limits of my Visual Basic
' skills.
'
' The plan was to use a function named GetMsg8Data() to get a pointer to the
' payload and then pass this to the ParseSeaway1_3() function. But for some reason
' when I include the call to GetMsg8Data() VB6 complains that it can't find the entry
' point.
'
' So for the forseeable future, this code doesn't support St. Lawrence Seaway or IMO
' binary messages. You will need to use the 'C' code if you need the data from those.

Dim ais_state As AISSTATE
Dim s As String
Dim r As Integer
Dim msgid As Long
Dim ErrorFlags As Long
Dim lat_ddd As Double
Dim long_ddd As Double
Dim lat_d As Integer
Dim lat_mmmm As Double
Dim long_d As Integer
Dim long_mmmm As Double
Dim msg1 As aismsg_1
Dim msg4 As aismsg_4
Dim msg5 As aismsg_5
Dim msg6 As aismsg_6
Dim msg7 As aismsg_7
Dim msg8 As aismsg_8
Dim msg9 As aismsg_9
Dim msg10 As aismsg_10
Dim msg11 As aismsg_11
Dim msg12 As aismsg_12
Dim msg13 As aismsg_13
Dim msg14 As aismsg_14
Dim msg15 As aismsg_15
Dim msg16 As aismsg_16
Dim msg17 As aismsg_17
Dim msg18 As aismsg_18
Dim msg19 As aismsg_19
Dim msg20 As aismsg_20
Dim msg21 As aismsg_21
Dim msg22 As aismsg_22
Dim msg23 As aismsg_23
Dim msg24 As aismsg_24
Dim utc_time As timetag
Dim WindReport As wind_report
Dim WeatherReport As weather_report
Dim WaterLevelReport As water_level_report
Dim WaterFlowReport As water_flow_report
Dim LockSchedule As lock_schedule
Dim CurrentReport As current_report
Dim SalinityReport As salinity_report
Dim ProcessionOrder As procession_order
Dim msg1_1 As seaway1_1
Dim msg1_2 As seaway1_2
Dim msg1_3 As seaway1_3
Dim msg1_6 As seaway1_6
Dim msg2_1 As seaway2_1
Dim msg2_2 As seaway2_2
Dim msg32_1 As seaway32_1
Dim msg1_4 As pawss1_4
Dim msg1_5 As pawss1_5
Dim msg2_3 As pawss2_3
Dim msg1_11 As imo1_11
Dim msg1_12 As imo1_12
Dim msg1_13 As imo1_13
Dim msg1_14 As imo1_14
Dim msg1_15 As imo1_15
Dim msg1_16 As imo1_17


ais_state = InitAisparser()

s = "!AIVDM,1,1,,B,15MqvC0Oh9G?qinK?VlPhA480@2n,0*1F"
r = AssembleVdm(ais_state, s)
msgid = Get6Bit(ais_state, 6)
r = ParseMsg1(ais_state, msg1)

ErrorFlags = 0


If msgid <> 1 Then
    ErrorFlags = ErrorFlags + 1
End If
If msg1.repeat <> 0 Then
    ErrorFlags = ErrorFlags + 2
End If
If msg1.userid <> 366902860 Then
    ErrorFlags = ErrorFlags + 4
End If
If msg1.nav_status <> 0 Then
    ErrorFlags = ErrorFlags + 8
End If
If msg1.rot <> 127 Then
    ErrorFlags = ErrorFlags + 16
End If
If msg1.sog <> 9 Then
    ErrorFlags = ErrorFlags + 32
End If
If msg1.pos_acc <> 0 Then
    ErrorFlags = ErrorFlags + 64
End If
If msg1.longitude <> -73413061 Then
    ErrorFlags = ErrorFlags + 128
End If
If msg1.latitude <> 28567250 Then
    ErrorFlags = ErrorFlags + 256
End If
If msg1.cog <> 193 Then
    ErrorFlags = ErrorFlags + 512
End If
If msg1.true <> 34 Then
    ErrorFlags = ErrorFlags + 1024
End If
If msg1.utc_sec <> 4 Then
    ErrorFlags = ErrorFlags + 2048
End If
If msg1.regional <> 0 Then
    ErrorFlags = ErrorFlags + 4096
End If
If msg1.raim <> 0 Then
    ErrorFlags = ErrorFlags + 8192
End If
If msg1.sync_state <> 0 Then
    ErrorFlags = ErrorFlags + 16384
End If
If msg1.slot_timeout <> 4 Then
    ErrorFlags = ErrorFlags + 32768
End If
If msg1.sub_message <> 182 Then
    ErrorFlags = ErrorFlags + 65536
End If

' Convert the AIS 1/10000 degree position to DD.DDDDDD format
r = pos2ddd(msg1.latitude, msg1.longitude, lat_ddd, long_ddd)

MsgBox (CStr(r) + " : " + CStr(msgid) + " : 0x" + Hex(ErrorFlags) + "  " + CStr(lat_ddd) + " / " + CStr(long_ddd))


' Message 4 test
s = "!AIVDM,1,1,,A,403OwpiuIKl:Ro=sbvK=CG700<3b,0*5E"
r = AssembleVdm(ais_state, s)
msgid = Get6Bit(ais_state, 6)
r = ParseMsg4(ais_state, msg4)

ErrorFlags = 0

If msgid <> 4 Then
    ErrorFlags = ErrorFlags + 1
End If
If msg4.repeat <> 0 Then
    ErrorFlags = ErrorFlags + 2
End If
If msg4.userid <> 3669987 Then
    ErrorFlags = ErrorFlags + 4
End If
If msg4.utc_year <> 2006 Then
    ErrorFlags = ErrorFlags + 8
End If
If msg4.utc_month <> 5 Then
    ErrorFlags = ErrorFlags + 16
End If
If msg4.utc_day <> 23 Then
    ErrorFlags = ErrorFlags + 32
End If
If msg4.utc_hour <> 20 Then
    ErrorFlags = ErrorFlags + 64
End If
If msg4.utc_minute <> 10 Then
    ErrorFlags = ErrorFlags + 128
End If
If msg4.utc_second <> 34 Then
    ErrorFlags = ErrorFlags + 256
End If
If msg4.pos_acc <> 1 Then
    ErrorFlags = ErrorFlags + 512
End If
If msg4.longitude <> -73671329 Then
    ErrorFlags = ErrorFlags + 1024
End If
If msg4.latitude <> 28529500 Then
    ErrorFlags = ErrorFlags + 2048
End If
If msg4.pos_type <> 7 Then
    ErrorFlags = ErrorFlags + 4096
End If
If msg4.spare <> 0 Then
    ErrorFlags = ErrorFlags + 8192
End If
If msg4.raim <> 0 Then
    ErrorFlags = ErrorFlags + 16384
End If
If msg4.sync_state <> 0 Then
    ErrorFlags = ErrorFlags + 32768
End If
If msg4.slot_timeout <> 3 Then
    ErrorFlags = ErrorFlags + 65536
End If
If msg4.sub_message <> 234 Then
    ErrorFlags = ErrorFlags + 131072
End If

' Convert the AIS 1/10000 degree position to DD MM.MMMM format
r = pos2dmm(msg4.latitude, msg4.longitude, lat_d, lat_mmmm, long_d, long_mmmm)

MsgBox (CStr(r) + " : " + CStr(msgid) + " : 0x" + Hex(ErrorFlags) + "  " + CStr(lat_d) + " " + CStr(lat_mmmm) + " / " + CStr(long_d) + " " + CStr(long_mmmm))



s = "!AIVDM,2,1,9,A,55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H,0*49"
r = AssembleVdm(ais_state, s)

If r = 0 Then
    MsgBox ("Error assembling 1st part of Msg 5")
    Return
End If

s = "!AIVDM,2,2,9,A,==40HtI4i@E531H1QDTVH51DSCS0,2*16"
r = AssembleVdm(ais_state, s)

If r <> 0 Then
    MsgBox ("Error assembling 2nd part of Msg 5")
    Return
End If

msgid = Get6Bit(ais_state, 6)
r = ParseMsg5(ais_state, msg5)

ErrorFlags = 0

If msgid <> 5 Then
    ErrorFlags = ErrorFlags + 1
End If
If msg5.repeat <> 0 Then
    ErrorFlags = ErrorFlags + 2
End If
If msg5.userid <> 366710810 Then
    ErrorFlags = ErrorFlags + 4
End If
If msg5.version <> 0 Then
    ErrorFlags = ErrorFlags + 8
End If
If msg5.imo <> 0 Then
    ErrorFlags = ErrorFlags + 16
End If
If msg5.imo <> 0 Then
    ErrorFlags = ErrorFlags + 32
End If
If StrComp(StrConv(msg5.callsign, vbUnicode), "WYX2158") <> 1 Then
    ErrorFlags = ErrorFlags + 64
End If
If StrComp(StrConv(msg5.Name, vbUnicode), "WALLA WALLA         ") <> 1 Then
    ErrorFlags = ErrorFlags + 128
End If
If msg5.ship_type <> 60 Then
    ErrorFlags = ErrorFlags + 256
End If
If msg5.dim_bow <> 32 Then
    ErrorFlags = ErrorFlags + 512
End If
If msg5.dim_stern <> 88 Then
    ErrorFlags = ErrorFlags + 1024
End If
If msg5.dim_port <> 13 Then
    ErrorFlags = ErrorFlags + 2048
End If
If msg5.dim_starboard <> 13 Then
    ErrorFlags = ErrorFlags + 4096
End If
If msg5.pos_type <> 1 Then
    ErrorFlags = ErrorFlags + 8192
End If
If msg5.eta <> 1596 Then
    ErrorFlags = ErrorFlags + 16384
End If
If msg5.draught <> 100 Then
    ErrorFlags = ErrorFlags + 32768
End If
If StrComp(StrConv(msg5.dest, vbUnicode), "SEATTLE FERRY TERMNL") <> 1 Then
    ErrorFlags = ErrorFlags + 65536
End If
If msg5.dte <> 0 Then
    ErrorFlags = ErrorFlags + 131072
End If
If msg5.spare <> 0 Then
    ErrorFlags = ErrorFlags + 262144
End If

MsgBox (CStr(r) + " : " + CStr(msgid) + " : 0x" + Hex(ErrorFlags))


' Message 7 test
s = "!AIVDM,1,1,,A,703Owpi9lmaQ,0*3B"
r = AssembleVdm(ais_state, s)
msgid = Get6Bit(ais_state, 6)
r = ParseMsg7(ais_state, msg7)

ErrorFlags = 0

If msgid <> 7 Then
    ErrorFlags = ErrorFlags + 1
End If
If msg7.repeat <> 0 Then
    ErrorFlags = ErrorFlags + 2
End If
If msg7.userid <> 3669987 Then
    ErrorFlags = ErrorFlags + 4
End If
If msg7.spare <> 0 Then
    ErrorFlags = ErrorFlags + 8
End If
If msg7.destid_1 <> 309647000 Then
    ErrorFlags = ErrorFlags + 16
End If
If msg7.sequence_1 <> 1 Then
    ErrorFlags = ErrorFlags + 32
End If
If msg7.num_acks <> 1 Then
    ErrorFlags = ErrorFlags + 64
End If
    
MsgBox (CStr(r) + " : " + CStr(msgid) + " : 0x" + Hex(ErrorFlags))


' Message 8 / St. Lawrence Seaway Test
        
s = "!AIVDM,2,1,2,B,8030ojA?0@=DE3@?BDPA3onQiUFttP1Wh01DE3<1EJ?>0onlkUG0e01I,0*3D"
r = AssembleVdm(ais_state, s)

If r = 0 Then
    MsgBox ("Error assembling 1st part of Msg 8")
    Return
End If

s = "!AIVDM,2,2,2,B,h00,2*7D"
r = AssembleVdm(ais_state, s)

If r <> 0 Then
    MsgBox ("Error assembling 2nd part of Msg 8")
    Return
End If

msgid = Get6Bit(ais_state, 6)
r = ParseMsg8(ais_state, msg8)

ErrorFlags = 0

If msgid <> 5 Then
    ErrorFlags = ErrorFlags + 1
End If

' Which binary message is this?
dac = Int(msg8.app_id / 64)
fi = msg8.app_id And 63

MsgBox (CStr(r) + " : " + CStr(msgid) + " : dac " + CStr(dac) + " : fi " + CStr(fi))


' Message 12 test
s = "!AIVDM,1,1,,A,<03Owph00002QG51D85BP1<5BDQP,0*7D"
r = AssembleVdm(ais_state, s)
msgid = Get6Bit(ais_state, 6)
r = ParseMsg12(ais_state, msg12)

ErrorFlags = 0

If msgid <> 12 Then
    ErrorFlags = ErrorFlags + 1
End If
If msg12.repeat <> 0 Then
    ErrorFlags = ErrorFlags + 2
End If
If msg12.userid <> 3669987 Then
    ErrorFlags = ErrorFlags + 4
End If
If msg12.sequence <> 0 Then
    ErrorFlags = ErrorFlags + 8
End If
If msg12.destination <> 0 Then
    ErrorFlags = ErrorFlags + 16
End If
If msg12.retransmit <> 1 Then
    ErrorFlags = ErrorFlags + 32
End If
If msg12.spare <> 0 Then
    ErrorFlags = ErrorFlags + 64
End If
If StrComp(StrConv(msg12.message, vbUnicode), "!WEATHER ALERT! ") <> 1 Then
    ErrorFlags = ErrorFlags + 128
End If

MsgBox (CStr(r) + " : " + CStr(msgid) + " : 0x" + Hex(ErrorFlags))

    
' Message 13 test
s = "!AIVDM,1,1,,A,=03Owpi;Eo7`,0*7F"
r = AssembleVdm(ais_state, s)
msgid = Get6Bit(ais_state, 6)
r = ParseMsg13(ais_state, msg13)

ErrorFlags = 0

If msgid <> 13 Then
    ErrorFlags = ErrorFlags + 1
End If
If msg13.repeat <> 0 Then
    ErrorFlags = ErrorFlags + 2
End If
If msg13.userid <> 3669987 Then
    ErrorFlags = ErrorFlags + 4
End If
If msg13.spare <> 0 Then
    ErrorFlags = ErrorFlags + 8
End If
If msg13.destid_1 <> 316005498 Then
    ErrorFlags = ErrorFlags + 16
End If
If msg13.sequence_1 <> 0 Then
    ErrorFlags = ErrorFlags + 32
End If
If msg13.num_acks <> 1 Then
    ErrorFlags = ErrorFlags + 64
End If

MsgBox (CStr(r) + " : " + CStr(msgid) + " : 0x" + Hex(ErrorFlags))


' Message 15 test
s = "!AIVDM,1,1,,A,?03OwpiGPmD0000,2*07"
r = AssembleVdm(ais_state, s)
msgid = Get6Bit(ais_state, 6)
r = ParseMsg15(ais_state, msg15)

ErrorFlags = 0

If msgid <> 15 Then
    ErrorFlags = ErrorFlags + 1
End If
If msg15.repeat <> 0 Then
    ErrorFlags = ErrorFlags + 2
End If
If msg15.userid <> 3669987 Then
    ErrorFlags = ErrorFlags + 4
End If
If msg15.spare1 <> 0 Then
    ErrorFlags = ErrorFlags + 8
End If
If msg15.destid1 <> 367056192 Then
    ErrorFlags = ErrorFlags + 16
End If
If msg15.msgid1_1 <> 0 Then
    ErrorFlags = ErrorFlags + 32
End If
If msg15.offset1_1 <> 0 Then
    ErrorFlags = ErrorFlags + 64
End If
If msg15.spare2 <> 0 Then
    ErrorFlags = ErrorFlags + 128
End If
If msg15.msgid1_2 <> 0 Then
    ErrorFlags = ErrorFlags + 256
End If
If msg15.offset1_2 <> 0 Then
    ErrorFlags = ErrorFlags + 512
End If
If msg15.spare3 <> 0 Then
    ErrorFlags = ErrorFlags + 1024
End If
If msg15.destid2 <> 0 Then
    ErrorFlags = ErrorFlags + 2048
End If
If msg15.msgid2_1 <> 0 Then
    ErrorFlags = ErrorFlags + 4096
End If
If msg15.offset2_1 <> 0 Then
    ErrorFlags = ErrorFlags + 8192
End If
If msg15.spare4 <> 0 Then
    ErrorFlags = ErrorFlags + 16384
End If

MsgBox (CStr(r) + " : " + CStr(msgid) + " : 0x" + Hex(ErrorFlags))


' Message 15 test
s = "!AIVDM,1,1,,A,D03OwphiIN>4,0*25"
r = AssembleVdm(ais_state, s)
msgid = Get6Bit(ais_state, 6)
r = ParseMsg20(ais_state, msg20)

ErrorFlags = 0

If msgid <> 20 Then
    ErrorFlags = ErrorFlags + 1
End If
If msg20.repeat <> 0 Then
    ErrorFlags = ErrorFlags + 2
End If
If msg20.userid <> 3669987 Then
    ErrorFlags = ErrorFlags + 4
End If
If msg20.spare1 <> 0 Then
    ErrorFlags = ErrorFlags + 8
End If
If msg20.offset1 <> 790 Then
    ErrorFlags = ErrorFlags + 16
End If
If msg20.slots1 <> 5 Then
    ErrorFlags = ErrorFlags + 32
End If
If msg20.timeout1 <> 7 Then
    ErrorFlags = ErrorFlags + 64
End If
If msg20.increment1 <> 225 Then
    ErrorFlags = ErrorFlags + 128
End If
If msg20.offset2 <> 0 Then
    ErrorFlags = ErrorFlags + 256
End If
If msg20.slots2 <> 0 Then
    ErrorFlags = ErrorFlags + 512
End If
If msg20.timeout2 <> 0 Then
    ErrorFlags = ErrorFlags + 1024
End If
If msg20.increment2 <> 0 Then
    ErrorFlags = ErrorFlags + 2048
End If
If msg20.offset3 <> 0 Then
    ErrorFlags = ErrorFlags + 4096
End If
If msg20.slots3 <> 0 Then
    ErrorFlags = ErrorFlags + 8192
End If
If msg20.timeout3 <> 0 Then
    ErrorFlags = ErrorFlags + 16384
End If
If msg20.increment3 <> 0 Then
    ErrorFlags = ErrorFlags + 32768
End If
If msg20.offset4 <> 0 Then
    ErrorFlags = ErrorFlags + 65536
End If
If msg20.slots4 <> 0 Then
    ErrorFlags = ErrorFlags + 131072
End If
If msg20.timeout4 <> 0 Then
    ErrorFlags = ErrorFlags + 262144
End If
If msg20.increment4 <> 0 Then
    ErrorFlags = ErrorFlags + 524288
End If
If msg20.spare2 <> 0 Then
    ErrorFlags = ErrorFlags + 1048576
End If

MsgBox (CStr(r) + " : " + CStr(msgid) + " : 0x" + Hex(ErrorFlags))


CloseAisparser (ais_state)

End Sub
