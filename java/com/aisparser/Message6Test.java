package com.aisparser;

import junit.framework.TestCase;

public class Message6Test extends TestCase {

	Vdm vdm_message;
	Message6 msg;
	int result;

	public void testParse() {
		vdm_message = new Vdm();
		msg = new Message6();

		fail("Not yet implemented - Need test data");
		
		try {
			result = vdm_message.add("");
			assertEquals( "vdm add failed", 0, result );
			 
			msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}

		assertEquals( "msgid", 5, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 366710810, msg.userid());

	}

}
