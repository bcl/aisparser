package com.aisparser;

import static java.lang.System.out;
import junit.framework.TestCase;

public class VdmTest extends TestCase {

	Vdm vdm_message;
	
	protected void setUp() {
		vdm_message = new Vdm();
	}
	
	protected void tearDown() {
		vdm_message = null;
	}
	
	public void testMsgid() {
		int result;
		
		try {
			result = vdm_message.add("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
			assertEquals("Adding message 1 failed", 0, result);
			assertEquals("Message ID wrong", 1, vdm_message.msgid());
		} catch (Exception e) {
			fail(e.getMessage());
		}
	}

	public void testAdd() {
		int result;
		
		try {
			result = vdm_message.add("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
			assertEquals("Adding message 1 failed", 0, result);
			
			vdm_message = new Vdm();
			result = vdm_message.add("!AIVDM,2,1,6,B,55ArUT02:nkG<I8GB20nuJ0p5HTu>0hT9860TV16000006420BDi@E53,0*33\r\n");
			assertEquals("Adding message 5 part 1 failed", 1, result);
			
			result = vdm_message.add("!AIVDM,2,2,6,B,1KUDhH888888880,2*6A");
			assertEquals("Adding message 5 part 1 failed", 0, result);
		} catch (Exception e) {
			fail(e.getMessage());
		}
	}
}
