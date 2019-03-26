package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Ignore;

/** Tests for {@link Message6}. */
public class Message6Test {

    @Ignore("Need test data")
    public void testParse() {
        Message6 msg = new Message6();

        fail("Not yet implemented - Need test data");

        try {
            Vdm vdm_message = new Vdm();
            int result = vdm_message.add("");
            assertEquals("vdm add failed", 0, result);

            msg.parse(vdm_message.sixbit());
        } catch (Exception e) {
            fail(e.getMessage());
        }

        assertEquals("msgid", 5, msg.msgid());
        assertEquals("repeat", 0, msg.repeat());
        assertEquals("userid", 366710810, msg.userid());
    }
}
