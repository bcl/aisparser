#!/usr/bin/python

"""
    AIS Browser
    Copyright 2007-2008 by Brian C. Lane
    All Rights Reserved
    
    This is a wxPython GUI application for browsing AIS message logs,
    it will display detailed information about the selected message.

"""
import os,sys
import aisparser
import wx
import copy
from msg_order import msg_order
from wx.lib.mixins.listctrl import ListCtrlAutoWidthMixin
import ConfigParser
import stat


import wx.lib.inspection

class BrowserConfig:
    """
    Keep track of the configuration of the AIS Browser application

    Last opened file
    Last selection in last opened file
    Main window geometry
    Splitter position
    Column width settings for SourceListCtrl
    Column width settings for DetailListCtrl
    """
    def __init__(self):
        self.config = ConfigParser.ConfigParser()

        # The location of the config file will depend on the system being used
        # For Linux it will be in $HOME
        # For Windows it should be in the current directory?
        # For Mac it should be ?
        # sys.platform, os.name and os.path.expanduser("~") may all be useful
        self.config_file = os.getenv("HOME") + os.sep + ".aisbrowserrc" 

        if not os.path.isfile( self.config_file ): 
            # Create config file
            self.config.add_section("main")
            self.config.add_section("source_columns")
            self.config.add_section("detail_columns")
        else:
            width = int(self.config.get("main", "width"))

    def Save(self):
        """
        Save the config file
        """
        cf = open( config_file, "w" )
        self.config.write(cf) 
        cf.close()

    def SetLastFile(self, filename, selection):
        """
        Set the last opened filename
        """
        self.config.set("main","lastfile",filename)
        self.config.set("main","selection",selection)

    def SetGeometry(self, width, height, splitter ):
        """
        Save the current window geometry
        """
        self.config.set("main","width",width)
        self.config.set("main","height",height) 
        self.config.set("main","splitter",splitter)

    def SetSourceColumns(self, columns):
        """
        Save the SourceListCtrl column widths
        """
        for i,c in enumerate(columns):
            self.config.set("source_columns",i,c)

    def SetDetailColumns(self, columns):
        """
        Save the DetailListCtrl column widths
        """
        for i,c in enumerate(columns):
            self.config.set("detail_columns",i,c)



class AisMessage:
	"""
	Parse the AIS 6-bit data into message fields in self.data
	This maps the msgid to the aismsg_X structure and parse_ais_X function
	"""
	def __init__( self, ais_state ):
		# Find the corresponding message struct and function
		try:
			msg_struct = getattr( aisparser, "aismsg_" + str(ais_state.msgid) )
			msg_func = getattr( aisparser, "parse_ais_" + str(ais_state.msgid) )
			self.data = msg_struct()

			# Parse the message
			msg_func( ais_state, self.data )

			# If this is a message 6 or 8 try to parse the binary message
			if ais_state.msgid in [6,8]:
				ais_msg = self.data
				self.binary_state = aisparser.binary_state()
				self.binary_state.dac = ais_msg.app_id >> 6
				self.binary_state.fi = ais_msg.app_id & 0x3F 
				self.binary_state.six_state = ais_msg.data

#				print "dac = %d" % (self.binary_state.dac)
#				print "fi  = %d" % (ord(self.binary_state.fi))
				if self.binary_state.dac in [366,316] and ord(self.binary_state.fi) in [1,2,32,]:
					# Looks like a St. Lawrence Seaway Message
#					print "St. Lawrence Seaway"
					self.seaway_spare = aisparser.get_6bit( self.binary_state.six_state, 2 )
					self.seaway_msgid = aisparser.get_6bit( self.binary_state.six_state, 6 )
					
					if (ord(self.binary_state.fi),self.seaway_msgid) in [(1,1),(1,2),(1,3),(1,6),(2,1),(2,2),(32,1)]:
						msg_struct = getattr( aisparser, "seaway%d_%d" % (ord(self.binary_state.fi),self.seaway_msgid))
						msg_func = getattr( aisparser, "parse_seaway%d_%d" % (ord(self.binary_state.fi),self.seaway_msgid))
						self.binary_data = msg_struct()
						msg_func( self.binary_state, self.binary_data)
						self.binary_type = "seaway"
					elif (ord(self.binary_state.fi),self.seaway_msgid) in [(1,4),(1,5),(2,3)]:
						msg_struct = getattr( aisparser, "pawss%d_%d" % (ord(self.binary_state.fi),self.seaway_msgid))
						msg_func = getattr( aisparser, "parse_pawss%d_%d" % (ord(self.binary_state.fi),self.seaway_msgid))
						self.binary_data = msg_struct()
						msg_func( self.binary_state, self.binary_data)
						self.binary_type = "seaway"
				elif self.binary_state.dac == 1 and ord(self.binary_state.fi) in [11,12,13,14,15,16,17]:
					# Looks like an IMO message
					msg_struct = getattr( aisparser, "imo%d_%d" % (self.binary_state.dac,ord(self.binary_state.fi)))
					msg_func = getattr( aisparser, "parse_seaway%d_%d" % (self.binary_state.dac,ord(self.binary_state.fi)))
					self.binary_data = msg_struct()
					msg_func( self.binary_state, self.binary_data)
					self.binary_type = "imo"
		except AttributeError:
			self.data = None
			raise


class DataSource:
    """
    Source of AIVDM messages, used by the SourceListCtrl to load items into
    the ListCtrl on demand.
    The current implementation loads the whole file into memory, this makes it
    easier to handle parsing multi-part messages.
    """
    filename = ""
    ais_info = []

    
    def GetColumnHeaders(self):
        return ['mmsi','msgid','channel','sentence']

        
    def GetCount(self):
        return len(self.ais_info)

        
    def GetItem(self, index):
        # Parse AIVDM line # index into mmsi, msgid and sentence
        mmsi = "%d" % (self.ais_info[index].msg.data.userid)
        channel = self.ais_info[index].channel
        msgid = "%d" % (self.ais_info[index].msgid)
        sixbit = self.ais_info[index].six_state.bits
        
        return [ mmsi, msgid, channel, sixbit ]


    def GetDetails(self, index):
        return self.ais_info[index]

        
    def UpdateCache(self, start, end):
#        print start, end
        pass

        
    def LoadFile(self):
        """
        Re-Assemble the data into full packets
        """
        # Initialize the AIS Parser class
        ais_state = aisparser.ais_state()

       
        # Process the whole file
        info = os.stat( self.filename )

        dialog = wx.ProgressDialog("Loading %s" % (self.filename),
                               "Time remaining",
                               info[stat.ST_SIZE],
                               style=wx.PD_ELAPSED_TIME | wx.PD_REMAINING_TIME)
 

        ais_file = open( self.filename, "r" )
        old_pos = 0
        while 1:
            line = ais_file.readline().strip()
            if not line:
                break
            pos = ais_file.tell() 
            if pos > old_pos + 4e3:
                keepGoing = dialog.Update(pos)
                old_pos = pos
 
            result = aisparser.assemble_vdm( ais_state, line )
            if result == 0:
                ais_state.msgid = aisparser.get_6bit( ais_state.six_state, 6 )
                ais_state.msg = AisMessage( ais_state )
                if ais_state.msg.data: 
                    self.ais_info.append( ais_state )
                # Re-start the parser, creating a new ais_state object
                ais_state = aisparser.ais_state()
        ais_file.close()
        dialog.Destroy()


class SourceListCtrl(wx.ListCtrl,ListCtrlAutoWidthMixin):
    """
    Display the AIVDM messages, including the mmsi, msgid and raw 6-bit data
    """
    def __init__(self, parent, dataSource):
        wx.ListCtrl.__init__(self, parent, style=wx.LC_REPORT|wx.LC_SINGLE_SEL|wx.LC_VIRTUAL)
        ListCtrlAutoWidthMixin.__init__(self)
        self.dataSource = dataSource
        self.Bind( wx.EVT_LIST_CACHE_HINT, self.DoCacheItems )
        self.SetItemCount( dataSource.GetCount() )
        columns = dataSource.GetColumnHeaders()
        for col, text in enumerate(columns):
            self.InsertColumn(col, text)

        # If the user has resized the columns, the widths should be
        # set based on the last user settings.

        # Set column 1,2 to center
 
    def DoCacheItems(self, evt):
        self.dataSource.UpdateCache( evt.GetCacheFrom(), evt.GetCacheTo())

        
    def OnGetItemText( self, item, col ):
        data = self.dataSource.GetItem(item)
        return data[col]

        
    def OnGetItemAttr( self, item ):
        return None


    def OnGetItemImage( self, item ):
        return -1        


class DetailListCtrl(wx.ListCtrl,ListCtrlAutoWidthMixin):
    """
    Display the details about a specific AIVDM message
    """
    def __init__(self, parent):
        wx.ListCtrl.__init__(self, parent, style=wx.LC_REPORT|wx.LC_SINGLE_SEL)
	ListCtrlAutoWidthMixin.__init__(self)
        self.InsertColumn(0, 'Field')
        self.InsertColumn(1, 'Value')
        self.InsertColumn(2, 'Details')


class BrowserFrame(wx.Frame):
    """
    Main program frame, containing the SourceListCtrl and DetailListCtrl
    """
    def __init__(self, parent, id):
        self.initpos = 50
        wx.Frame.__init__(self, parent, id, 'AIS Browser', size=(600, 600))

        # Setup the menubar
        menuBar = wx.MenuBar()
        menu1 = wx.Menu()
        menuItem = menu1.Append(-1, "&Open Logfile...")
        self.Bind(wx.EVT_MENU, self.OnOpenFile, menuItem)

        menuItem = menu1.Append(-1, "&Exit")
        self.Bind(wx.EVT_MENU, self.OnCloseMe, menuItem)

        menuBar.Append(menu1, "&File")
        self.SetMenuBar(menuBar)

        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetFieldsCount(2)

        # Setup the splitter window
        self.sp1 = wx.SplitterWindow(self)
        self.sp1.SetMinimumPaneSize(1)
        self.sourcelist = SourceListCtrl( self.sp1, DataSource() )
        self.sourcelist.Bind( wx.EVT_LIST_ITEM_SELECTED, self.UpdateDetails, self.sourcelist )
        self.detaillist = DetailListCtrl( self.sp1 )

        # If the user has previously adjusted the positions, the 
        # initial settings should come from the user's last selections
        self.sp1.SplitHorizontally(self.sourcelist, self.detaillist, 200)


    def UpdateDetails(self, event):
        item = event.GetItem()
#        print "Item: ", event.GetIndex()
        details = self.sourcelist.dataSource.GetDetails( event.GetIndex() )
#        print details.six_state.bits

#        print dir(details.msg.data)
#        for a in dir(details.msg.data):
#            if a[:2] != "__":
#                attr = getattr(details.msg.data, a)
#                if type(attr) == str and len(attr) == 1:
#                    print "%s = %d" % (a, ord(attr))
#                else:
#                    print "%s = %s" % (a, attr)

        self.detaillist.DeleteAllItems()
        if details.msgid in msg_order:
            for field in msg_order[details.msgid]:
                index = self.detaillist.InsertStringItem(sys.maxint, field)
                # Special case for sotdma and itdma
                if field == 'sotdma' or field == 'itdma':
                    print "%s :" % field
                    data = getattr(details.msg.data, field)
                    for subfield in msg_order[field]:
                        attr = getattr(data, subfield)
                        if type(attr) == str and len(attr) == 1:
#                            print "%s = %d" % (subfield, ord(attr))
                            self.detaillist.SetStringItem(index, 1, "%d" % ord(attr))
                        else:
#                            print "%s = %s" % (subfield, attr)
                            self.detaillist.SetStringItem(index, 1, "%s" % attr)
                elif details.msgid == 8 and field == 'data':
					index = self.detaillist.InsertStringItem(sys.maxint, 'dac')
					self.detaillist.SetStringItem(index, 1, "%d" % (details.msg.binary_state.dac))
					index = self.detaillist.InsertStringItem(sys.maxint, 'fi')
					self.detaillist.SetStringItem(index, 1, "%d" % ord(details.msg.binary_state.fi))

					binary_msg = details.msg.binary_data
					if details.msg.binary_type == 'seaway':
						index = self.detaillist.InsertStringItem(sys.maxint, 'msgid')
						self.detaillist.SetStringItem(index, 1, "%d" % (details.msg.seaway_msgid))
						seaway_msg = details.msg.binary_data
						print dir(seaway_msg)
						print type(seaway_msg.report)
						
					elif details.msg.binary_type == 'imo':
						pass

                elif details.msgid == 6 and field == 'data':
					index = self.detaillist.InsertStringItem(sys.maxint, 'dac')
					self.detaillist.SetStringItem(index, 1, "%d" % (details.msg.binary_state.dac))
					index = self.detaillist.InsertStringItem(sys.maxint, 'fi')
					self.detaillist.SetStringItem(index, 1, "%d" % ord(details.msg.binary_state.fi))

                else:
                    attr = getattr(details.msg.data, field)
                    if type(attr) == str and len(attr) == 1:
#                        print "%s = %d" % (field, ord(attr))
                        self.detaillist.SetStringItem(index, 1, "%d" % ord(attr))
                    else:
#                        print "%s = %s" % (field, attr)
                        self.detaillist.SetStringItem(index, 1, "%s" % attr)


    def OnOpenFile(self, event):
        """
        Display a file dialog for opening the AIVDM message logfile
        """
        wildcard = "Logfile (*.log)|*.log|" \
                   "All files (*.*)|*.*"
        dialog = wx.FileDialog(None, "Choose a AIS Logfile", os.getcwd(), "", wildcard, wx.OPEN)
        if dialog.ShowModal() == wx.ID_OK:
            self.sourcelist.dataSource.filename = dialog.GetPath()
            self.sourcelist.dataSource.LoadFile()
            self.sourcelist.SetItemCount( len(self.sourcelist.dataSource.ais_info) )
        dialog.Destroy()


    def OnCloseMe(self, event):
        self.Close(True)


if __name__ == '__main__':

# Read user settings at some point:
# listctrl column sizes
# Window geometry
# Open file
# Current selection



    app = wx.PySimpleApp()
    frame = BrowserFrame(parent=None, id=-1)
    frame.Show()

#    wx.lib.inspection.InspectionTool().Show()
    
    app.MainLoop()

