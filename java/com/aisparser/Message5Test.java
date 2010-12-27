package com.aisparser;

import junit.framework.TestCase;

public class Message5Test extends TestCase {

	Vdm vdm_message;
	Message5 msg;
	int result;
	
	public void testParse()
	{
		vdm_message = new Vdm();
		 msg = new Message5();

		 try {
			 result = vdm_message.add("!AIVDM,2,1,9,A,55Mf@6P00001MUS;7GQL4hh61L4hh6222222220t41H,0*49\r\n");
			 assertEquals( "vdm add failed", 1, result );

			 result = vdm_message.add("!AIVDM,2,2,9,A,==40HtI4i@E531H1QDTVH51DSCS0,2*16\r\n");
			 assertEquals( "vdm add failed", 0, result );
			 
			 msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			fail(e.getMessage());
		}
		
		assertEquals( "msgid", 5, msg.msgid());
		assertEquals( "repeat", 0, msg.repeat());
		assertEquals( "userid", 366710810, msg.userid());
		assertEquals( "version", 0, msg.version() );
		assertEquals( "imo", 0, msg.imo() );
		assertEquals( "callsign", "WYX2158", msg.callsign() );
		assertEquals( "name", "WALLA WALLA         ", msg.name() );
		assertEquals( "ship_type", 60, msg.ship_type() );
		assertEquals( "dim_bow", 32, msg.dim_bow() );
		assertEquals( "dim_stern", 88, msg.dim_stern() );
		assertEquals( "dim_starboard", 13, msg.dim_starboard() );
		assertEquals( "dim_port", 13, msg.dim_port() );
		assertEquals( "pos_type", 1, msg.pos_type() );
		assertEquals( "eta", 1596, msg.eta() );
		assertEquals( "draught", 100, msg.draught() );
		assertEquals( "dest", "SEATTLE FERRY TERMNL", msg.dest() );
		assertEquals( "dte", 0, msg.dte());
		assertEquals( "spare", 0, msg.spare() );
	}
}
