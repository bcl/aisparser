package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for {@link Message12}. */
@RunWith(JUnit4.class)
public class Message12Test {

    @Test
    public void testParse() {
        Message12 msg = new Message12();

        try {
            Vdm vdm_message = new Vdm();
            int result = vdm_message.add("!AIVDM,1,1,,A,<03Owph00002QG51D85BP1<5BDQP,0*7D");
            assertEquals("vdm add failed", 0, result);

            msg.parse(vdm_message.sixbit());
        } catch (Exception e) {
            fail(e.getMessage());
        }

        assertEquals("msgid", 12, msg.msgid());
        assertEquals("repeat", 0, msg.repeat());
        assertEquals("userid", 3669987, msg.userid());
        assertEquals("sequence", 0, msg.sequence());
        assertEquals("destination", 0, msg.destination());
        assertEquals("retransmit", 1, msg.retransmit());
        assertEquals("message", "!WEATHER ALERT! ", msg.message());
    }
}
