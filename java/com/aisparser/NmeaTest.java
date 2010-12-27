package com.aisparser;

import junit.framework.TestCase;

public class NmeaTest extends TestCase {

	Nmea	nmea_message;
	
	protected void setUp() {
		nmea_message = new Nmea();
	}
	
	protected void tearDown() {
		nmea_message = null;
	}
	
	public void testFind_start() {
		nmea_message.init("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
		try {
			assertEquals("Start found in the wrong place", 0, nmea_message.find_start());
		} catch (StartNotFoundException e) {
			fail("Failed to find the start");
		}

		nmea_message.init("here is some junk before!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
		try {
			assertEquals("Start found in the wrong place", 24, nmea_message.find_start());
		} catch (StartNotFoundException e) {
			fail("Failed to find the start");
		}

		nmea_message.init(",1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
		try {
			nmea_message.find_start();
			fail("Start found when it doesn't exist");
		} catch (StartNotFoundException e) {
			
		}
	}

	public void testCalculateChecksum() {
		nmea_message.init("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
		try {
			nmea_message.calculateChecksum();
		} catch (StartNotFoundException e) {
			fail("Failed to find the start");
		} catch (IllegalNMEACharacterException e) {
			fail("Found illegal NMEA character");
		}
	}

	public void testCheckChecksum() {
		nmea_message.init("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
		assertEquals("Checksum failed", 0, nmea_message.checkChecksum());

		nmea_message.init("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*55\r\n");
		assertEquals("Checksum failed to fail", 1, nmea_message.checkChecksum());
	}
}
