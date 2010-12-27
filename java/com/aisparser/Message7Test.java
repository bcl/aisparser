package com.aisparser;

import junit.framework.TestCase;

public class Message7Test extends TestCase {

	Vdm vdm_message;
	Message7 msg;
	int result;

	public void testParse() {
		vdm_message = new Vdm();
		msg = new Message7();

		try {
			result = vdm_message.add("!AIVDM,1,1,,A,703Owpi9lmaQ,0*3B");
			assertEquals( "vdm add failed", 0, result );
			 
			msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}

		assertEquals( "num_acks", 1, msg.num_acks());
		assertEquals( "msgid", 7, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 3669987, msg.userid());
		assertEquals( "spare", 0, msg.spare());
		assertEquals( "destid_1", 309647000, msg.destid_1());
		assertEquals( "sequence_1", 1, msg.sequence_1());
	}

}
