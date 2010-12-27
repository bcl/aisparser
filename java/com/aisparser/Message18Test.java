package com.aisparser;

import junit.framework.TestCase;

public class Message18Test extends TestCase {

	Vdm vdm_message;
	Message18 msg;
	int result;

	public void testParse() {
		vdm_message = new Vdm();
		msg = new Message18();

		try {
			result = vdm_message.add("!AIVDM,1,1,,A,B52IRsP005=abWRnlQP03w`UkP06,0*2A\r\n");
			assertEquals( "vdm add failed", 0, result );
			 
			msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}
		
		assertEquals( "msgid", 18, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 338060014, msg.userid());
		assertEquals("regional1", 0, msg.regional1());
		assertEquals("sog", 0, msg.sog());
		assertEquals("pos_acc", 0, msg.pos_acc());
		assertEquals("longitude", -93506225, msg.longitude());
		assertEquals("latitude", 11981336, msg.latitude());
		assertEquals("cog", 0, msg.cog());
		assertEquals("true_heading", 511, msg.true_heading());
		assertEquals("utc_sec", 17, msg.utc_sec());
		assertEquals("regional2", 0, msg.regional2());
		assertEquals("unit_flag", 1, msg.unit_flag());
		assertEquals("display_flag", 0, msg.display_flag());
		assertEquals("dsc_flag", 1, msg.dsc_flag());
		assertEquals("band_flag", 1, msg.band_flag());
		assertEquals("msg22_flag", 1, msg.msg22_flag());
		assertEquals("mode_flag", 0, msg.mode_flag());
		assertEquals("raim", 0, msg.raim());
		assertEquals("comm_state", 1, msg.comm_state());
		if (msg.comm_state() == 0)
		{
			fail("sotdma state");
		} else {
			assertEquals("itdma.sync_state", 3, msg.itdma_state().sync_state());
			assertEquals("itdma.slot_inc", 0, msg.itdma_state().slot_inc());
			assertEquals("itdma.num_slots", 3, msg.itdma_state().num_slots());
			assertEquals("itdma.keep_flag", 0, msg.itdma_state().keep_flag());
		}
	}
}
