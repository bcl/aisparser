package com.aisparser;

import junit.framework.TestCase;

public class Message17Test extends TestCase {

	Vdm vdm_message;
	Message17 msg;
	int result;

	public void testParse() {
		vdm_message = new Vdm();
		msg = new Message17();

		fail("Not yet implemented - Need test data");
		
		try {
			result = vdm_message.add("");
			assertEquals( "vdm add failed", 0, result );
			 
			msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}

		assertEquals( "msgid", 17, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 0, msg.userid());

	}

}
