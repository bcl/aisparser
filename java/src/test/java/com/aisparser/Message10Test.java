package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for {@link Message3}. */
@RunWith(JUnit4.class)
public class Message10Test {

    Vdm vdm_message;
    Message10 msg;
    int result;

    @Test
    public void testParse() {
        vdm_message = new Vdm();
        msg = new Message10();

        try {
            result = vdm_message.add("!AIVDM,1,1,,A,:5D2Lp1Ghfe0,0*4E\r\n");
            assertEquals("vdm add failed", 0, result);

            msg.parse(vdm_message.sixbit());
        } catch (Exception e) {
            fail(e.getMessage());
        }

        assertEquals("msgid", 10, msg.msgid());
        assertEquals("repeat", 0, msg.repeat());
        assertEquals("userid", 356556000, msg.userid());
        assertEquals("spare1", 0, msg.spare1());
        assertEquals("destination", 368098000, msg.destination());
        assertEquals("spare2", 0, msg.spare2());
    }
}
