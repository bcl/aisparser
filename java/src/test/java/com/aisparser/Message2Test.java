package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for {@link Message2}. */
@RunWith(JUnit4.class)
public class Message2Test {

    // Verifies decode of Msg 2 for a ship in a turn.
    @Test
    public void testParse() {
        Message2 msg = new Message2();

        try {
            Vdm vdm_message = new Vdm();
            int result = vdm_message.add("!AIVDM,1,1,,B,284;UGTdP4301>3L;B@Wk3TnU@A1,0*7C\r\n");
            assertEquals("vdm add failed", 0, result);
            msg.parse(vdm_message.sixbit());
        } catch (Exception e) {
            fail(e.getMessage());
        }

        assertEquals("msgid", 2, msg.msgid());
        assertEquals("repeat", 0, msg.repeat());
        assertEquals("userid", 541255006, msg.userid());
        assertEquals("nav_status", 4, msg.nav_status());
        assertEquals("rot", 178, msg.rot());
        assertEquals("sog", 4, msg.sog());
        assertEquals("pos_acc", 0, msg.pos_acc());
        assertEquals("longitude", 25168321, msg.longitude()); // 41.947201666666665
        assertEquals("latitude", -37563838, msg.latitude()); // -62.606396666666669
        assertEquals("cog", 1996, msg.cog());
        assertEquals("true_heading", 114, msg.true_heading());
        assertEquals("utc_sec", 27, msg.utc_sec());
        assertEquals("regional", 4, msg.regional());
        assertEquals("spare", 1, msg.spare());
        assertEquals("raim", 0, msg.raim());
        assertEquals("sync_state", 2, msg.sync_state());
        assertEquals("slot_timeout", 4, msg.slot_timeout());
        assertEquals("sub_message", 1089, msg.sub_message());
    }
}
