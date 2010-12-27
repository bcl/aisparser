package com.aisparser;

import junit.framework.TestCase;

public class Message3Test extends TestCase {

	Vdm vdm_message;
	Message3 msg;
	int result;
	
	public void testParse()
	{
		vdm_message = new Vdm();
		 msg = new Message3();

		 try {
			 result = vdm_message.add("!AIVDM,1,1,,B,35Mk33gOkSG?bLtK?;B2dRO`00`A,0*30");
			 assertEquals( "vdm add failed", 0, result );
			 msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}
		
		assertEquals( "msgid", 3, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 366789390, msg.userid());
		assertEquals( "nav_status", 15, msg.nav_status() );
		assertEquals( "rot", 127, msg.rot() );
		assertEquals( "sog", 227, msg.sog() );
		assertEquals( "pos_acc", 0, msg.pos_acc() );
		assertEquals( "longitude", -73444450, msg.longitude() );
		assertEquals( "latitude", 28560200, msg.latitude() );
		assertEquals( "cog", 690, msg.cog() );
		assertEquals( "true_heading", 79, msg.true_heading() );
		assertEquals( "utc_sec", 52, msg.utc_sec() );
		assertEquals( "regional", 0, msg.regional() );
		assertEquals( "spare", 0, msg.spare() );
		assertEquals( "raim", 0, msg.raim() );
		assertEquals( "sync_state", 0, msg.sync_state() );
		assertEquals( "slot_increment", 161, msg.slot_increment() );
		assertEquals( "num_slots", 0, msg.num_slots() );
		assertEquals( "keep", 1, msg.keep() );
	}

}
