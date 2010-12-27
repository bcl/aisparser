package com.aisparser;

import junit.framework.TestCase;

public class SixbitTest extends TestCase {

	private Sixbit six_state;
	
	protected void setUp() {
		six_state = new Sixbit();
		six_state.init("");
		six_state.add("19NS7Sp02wo?HETKA2K6mUM20<L=");
	}
	
	protected void tearDown() {
		six_state = null;
	}
	
	public void testBit_length() {
		assertEquals("Length should be 168", 168, six_state.bit_length());
	}

	public void testLength() {
		
		assertEquals("Length should be 28", 28, six_state.length());
	}

	public void testBinfrom6bit() {
		int v = 0x30;
		int r = 0x00;
		
		// Pass all possible input values
		for ( ; v<0x58; v++, r++ )
		{
			assertEquals("binbrom6bit failed to convert "+v, r, six_state.binfrom6bit(v));
		}
		for ( v=0x60; v<0x78; v++, r++)
		{
			assertEquals("binbrom6bit failed to convert "+v, r, six_state.binfrom6bit(v));
		}
		
		// Test catching the IllegalArgumentException
		for ( v=0x00; v<0x30; v++)
		{
			try {
				six_state.binfrom6bit(v);
			} catch (IllegalArgumentException e) {
				continue;
			}
			fail("Failed to catch IllegalArgumentException for " + v);
		}
	}

	public void testBinto6bit() {
		int v = 0x00;
		int r = 0x30;
		
		// Pass all possible input values
		for ( ; v<0x28; v++, r++)
		{
			assertEquals("binto6bit failed to convert "+v, r, six_state.binto6bit(v));
		}
		r = 0x60;
		for ( ; v<0x40; v++, r++)
		{
			assertEquals("binto6bit failed to convert "+v, r, six_state.binto6bit(v));
		}
		
		// Test catching IllegalArgumentException
		for ( v=0x40; v<0x100; v++ )
		{
			try {
				six_state.binto6bit(v);
			} catch (IllegalArgumentException e) {
				continue;
			}
			fail("Failed to catch IllegalArgumentException for " + v);
		}
	}

	public void testAis2ascii() {
		int v = 0x00;
		int r = 0x40;
		
		// Pass all available inputs
		for ( ; v<0x20; v++, r++ )
		{
			assertEquals("ais2ascii failed to convert " + v, r, six_state.ais2ascii(v));
		}
		r = 0x20;
		for ( ; v<0x3F; v++, r++ )
		{
			assertEquals("ais2ascii failed to convert " + v, r, six_state.ais2ascii(v));
		}
		
		// Test catching IllegalArgumentException
		for ( v=0x40; v<0x100; v++ )
		{
			try {
				six_state.ais2ascii(v);
			} catch (IllegalArgumentException e) {
				continue;
			}
			fail("Failed to catch IllegalArgumentException for " + v);
		}
	}

	public void testGet() {
		int msgid = 0;
		
		try {
			msgid = (int) six_state.get(6);
		} catch (SixbitsExhaustedException e) {
			fail("Ran out of bits to get!");
		}
		assertEquals("msgid should be a 1", 1, msgid);
		
		// Need more tests
	}

	public void testGet_string() {
		String s;
		
		six_state.init("D5CD");
		s = six_state.get_string(4);
		assertEquals("Failed to get AIS string ", "TEST", s);
	}

}
