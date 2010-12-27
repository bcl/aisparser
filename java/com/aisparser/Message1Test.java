package com.aisparser;

import static java.lang.System.out;
import junit.framework.TestCase;

public class Message1Test extends TestCase {

	Vdm vdm_message;
	Message1 msg;
	int result;
	
	public void testParse()
	{
		vdm_message = new Vdm();
		 msg = new Message1();

		 try {
			 result = vdm_message.add("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
			 assertEquals( "vdm add failed", 0, result );
			 msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}
		
		assertEquals( "msgid", 1, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 636012431, msg.userid());
		assertEquals( "nav_status", 8, msg.nav_status() );
		assertEquals( "rot", 0, msg.rot() );
		assertEquals( "sog", 191, msg.sog() );
		assertEquals( "pos_acc", 1, msg.pos_acc() );
		assertEquals( "longitude", -73481550, msg.longitude() );
		assertEquals( "latitude", 28590700, msg.latitude() );
		assertEquals( "cog", 1750, msg.cog() );
		assertEquals( "true_heading", 174, msg.true_heading() );
		assertEquals( "utc_sec", 33, msg.utc_sec() );
		assertEquals( "regional", 0, msg.regional() );
		assertEquals( "spare", 0, msg.spare() );
		assertEquals( "raim", 0, msg.raim() );
		assertEquals( "sync_state", 0, msg.sync_state() );
		assertEquals( "slot_timeout", 3, msg.slot_timeout() );
		assertEquals( "sub_message", 1805, msg.sub_message() );
	}

}
