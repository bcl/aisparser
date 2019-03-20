package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for {@link Nmea}. */
@RunWith(JUnit4.class)
public class NmeaTest {

    Nmea nmea_message;

    @Before
    public void setUp() {
        nmea_message = new Nmea();
    }

    @After
    public void tearDown() {
        nmea_message = null;
    }

    @Test
    public void testFind_start() {
        nmea_message.init("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
        try {
            assertEquals("Start found in the wrong place", 0, nmea_message.find_start());
        } catch (StartNotFoundException e) {
            fail("Failed to find the start");
        }

        nmea_message.init(
                "here is some junk before!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
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

    @Test
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

    @Test
    public void testCheckChecksum() {
        nmea_message.init("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
        assertEquals("Checksum failed", 0, nmea_message.checkChecksum());

        nmea_message.init("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*55\r\n");
        assertEquals("Checksum failed to fail", 1, nmea_message.checkChecksum());
    }
}
