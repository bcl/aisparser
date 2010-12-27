package com.aisparser;

import junit.framework.TestCase;

public class Message20Test extends TestCase {

	Vdm vdm_message;
	Message20 msg;
	int result;

	public void testParse() {
		vdm_message = new Vdm();
		msg = new Message20();

		try {
			result = vdm_message.add("!AIVDM,1,1,,A,D03OwphiIN>4,0*25");
			assertEquals( "vdm add failed", 0, result );
			 
			msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}

		assertEquals("num_cmds", 1, msg.num_cmds());
		assertEquals( "msgid", 20, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 3669987, msg.userid());
		assertEquals("spare1", 0, msg.spare1());
		assertEquals("offset1", 790, msg.offset1());
		assertEquals("slots1", 5, msg.slots1());
		assertEquals("timeout1", 7, msg.timeout1());
		assertEquals("increment1", 225, msg.increment1());
	}
}
