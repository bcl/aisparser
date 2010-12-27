package com.aisparser;

import junit.framework.TestCase;

public class Message12Test extends TestCase {

	Vdm vdm_message;
	Message12 msg;
	int result;

	public void testParse() {
		vdm_message = new Vdm();
		msg = new Message12();

		try {
			result = vdm_message.add("!AIVDM,1,1,,A,<03Owph00002QG51D85BP1<5BDQP,0*7D");
			assertEquals( "vdm add failed", 0, result );
			 
			msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}
		
		assertEquals( "msgid", 12, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 3669987, msg.userid());
		assertEquals( "sequence", 0, msg.sequence());
		assertEquals( "destination", 0, msg.destination());
		assertEquals( "retransmit", 1, msg.retransmit());
		assertEquals( "message", "!WEATHER ALERT! ", msg.message());
		
	}

}
