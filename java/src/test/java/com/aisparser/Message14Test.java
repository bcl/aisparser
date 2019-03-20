package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for {@link Message14}. */
@RunWith(JUnit4.class)
public class Message14Test {

    Vdm vdm_message;
    Message14 msg;
    int result;

    @Test
    @Ignore("Need test data")
    public void testParse() {
        vdm_message = new Vdm();
        msg = new Message14();

        fail("Not yet implemented - Need test data");

        try {
            result = vdm_message.add("");
            assertEquals("vdm add failed", 0, result);

            msg.parse(vdm_message.sixbit());
        } catch (Exception e) {
            fail(e.getMessage());
        }

        assertEquals("msgid", 14, msg.msgid());
        assertEquals("repeat", 0, msg.repeat());
        assertEquals("userid", 0, msg.userid());
    }
}
