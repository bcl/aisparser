package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for {@link Message14}. */
@RunWith(JUnit4.class)
public class Message14Test {

    @Test
    public void testParse() {
        Message14 msg = new Message14();

        try {
            Vdm vdm_message = new Vdm();
            int result = vdm_message.add("!AIVDM,1,1,,A,>>M@rl1<59B1@E=@0000000,2*0D");
            assertEquals("vdm add failed", 0, result);

            msg.parse(vdm_message.sixbit());
        } catch (Exception e) {
            fail(e.getMessage());
        }

        assertEquals("msgid", 14, msg.msgid());
        assertEquals("repeat", 0, msg.repeat());
        assertEquals("userid", 970210000, msg.userid());
        assertEquals("spare", 0, msg.spare());
        assertEquals("message", "SART TEST@@@@@@@", msg.message());
    }
}
