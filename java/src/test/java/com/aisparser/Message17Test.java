package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for {@link Message17}. */
@RunWith(JUnit4.class)
public class Message17Test {

    Vdm vdm_message;
    Message17 msg;
    int result;

    @Test
    @Ignore("Need test data")
    public void testParse() {
        vdm_message = new Vdm();
        msg = new Message17();

        fail("Not yet implemented - Need test data");

        try {
            result = vdm_message.add("");
            assertEquals("vdm add failed", 0, result);

            msg.parse(vdm_message.sixbit());
        } catch (Exception e) {
            fail(e.getMessage());
        }

        assertEquals("msgid", 17, msg.msgid());
        assertEquals("repeat", 0, msg.repeat());
        assertEquals("userid", 0, msg.userid());
    }
}
