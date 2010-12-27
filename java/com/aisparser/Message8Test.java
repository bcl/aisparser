package com.aisparser;

import junit.framework.TestCase;

public class Message8Test extends TestCase {

	Vdm vdm_message;
	Message8 msg;
	int result;
	
	public void testParse() {
		vdm_message = new Vdm();
		 msg = new Message8();

		 try {
			 result = vdm_message.add("!AIVDM,3,1,1,A,85MwqciKf@nWshjR1VfGGDssdvT>hncBfTwcsgGKo?t,0*2E\r\n");
			 assertEquals( "vdm add failed", 1, result );

			 result = vdm_message.add("!AIVDM,3,2,1,A,u1uBo`7b`1Oa>@cO0f2wr1mwb0=kf<tI2MwS;sVKU07,0*67\r\n");
			 assertEquals( "vdm add failed", 1, result );

			 result = vdm_message.add("!AIVDM,3,3,1,A,8fDSaOKeP,2*0C\r\n");
			 assertEquals( "vdm add failed", 0, result );
			 
			 msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}

		assertEquals( "msgid", 8, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 366999983, msg.userid());
		assertEquals("spare", 0, msg.spare() );
		assertEquals("app_id", 23481, msg.app_id());
		
		Sixbit data = msg.data();
		assertEquals("data length", 568, data.bit_length());
		
		// Here is where the payload would be parsed, if we knew how...	
	}

}
