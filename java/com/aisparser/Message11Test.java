package com.aisparser;

import junit.framework.TestCase;

public class Message11Test extends TestCase {

	Vdm vdm_message;
	Message11 msg;
	int result;
	
	public void testParse() {
		vdm_message = new Vdm();
		 msg = new Message11();

		 try {
			 result = vdm_message.add("!AIVDM,1,1,,A,;4WOL21uM<jCroP`g8B=NFQ00000,0*37\r\n");
			 assertEquals( "vdm add failed", 0, result );
			 
			 msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}

		assertEquals( "msgid", 11, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 309845000, msg.userid());
		assertEquals("utc_year", 2007, msg.utc_year());
		assertEquals("utc_month", 4, msg.utc_month());
		assertEquals("utc_day", 25, msg.utc_day());
		assertEquals("utc_hour", 18, msg.utc_hour());
		assertEquals("utc_minute", 19, msg.utc_minute());
		assertEquals("utc_second", 58, msg.utc_second());
		assertEquals("pos_acc", 1, msg.pos_acc());
		assertEquals("longitude", -71219740, msg.longitude());
		assertEquals("latitude", 19095130, msg.latitude());
		assertEquals("pos_type", 1, msg.pos_type());
		assertEquals("spare", 0, msg.spare());
		assertEquals("raim", 0, msg.raim());
		assertEquals("sotdma.sync_state", 0, msg.sotdma_state().sync_state());
		assertEquals("sotdma.slot_timeout", 0, msg.sotdma_state().slot_timeout());
		assertEquals("sotdma.sub_message", 0, msg.sotdma_state().sub_message());	
	}

}
