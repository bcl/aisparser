package com.aisparser;

import junit.framework.TestCase;

public class Message9Test extends TestCase {

	Vdm vdm_message;
	Message9 msg;
	int result;

	public void testParse() {
		vdm_message = new Vdm();
		 msg = new Message9();

		 try {
			 result = vdm_message.add("!AIVDM,1,1,,B,900048wwTcJb0mpF16IobRP2086Q,0*48\r\n");
			 assertEquals( "vdm add failed", 0, result );
			 
			 msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}

		assertEquals( "msgid", 9, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 1059, msg.userid());
		assertEquals("altitude", 4094, msg.altitiude());
		assertEquals("sog", 299, msg.sog());
		assertEquals("pos_acc", 0, msg.pos_acc());
		assertEquals("longitude", -44824900, msg.longitude());
		assertEquals("latitude", 23086695, msg.latitude());
		assertEquals("cog", 1962, msg.cog());
		assertEquals("utc_sec", 10, msg.utc_sec());
		assertEquals("regional", 0, msg.regional());
		assertEquals("dte", 1, msg.dte());
		assertEquals("spare", 0, msg.spare());
		assertEquals("assigned", 0, msg.assigned());
		assertEquals("raim", 0, msg.raim());
		assertEquals("comm_state", 0, msg.comm_state());
		if ( msg.comm_state() == 0 )
		{
			assertEquals("sotdma.sync_state", 0, msg.sotdma_state().sync_state());
			assertEquals("sotdma.slot_timeout", 2, msg.sotdma_state().slot_timeout());
			assertEquals("sotdma.sub_message", 417, msg.sotdma_state().sub_message());
		} else {
			fail("itdma state");
		}
		
	}

}
