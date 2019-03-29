package com.aisparser;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/** Tests for {@link Message1}. */
@RunWith(JUnit4.class)
public class Message1Test {

    @Test
    public void testParse() {
        Message1 msg = new Message1();

        try {
            Vdm vdm_message = new Vdm();
            int result = vdm_message.add("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
            assertEquals("vdm add failed", 0, result);
            msg.parse(vdm_message.sixbit());
        } catch (Exception e) {
            fail(e.getMessage());
        }

        assertEquals("msgid", 1, msg.msgid());
        assertEquals("repeat", 0, msg.repeat());
        assertEquals("userid", 636012431, msg.userid());
        assertEquals("nav_status", 8, msg.nav_status());
        assertEquals("rot", 0, msg.rot());
        assertEquals("sog", 191, msg.sog());
        assertEquals("pos_acc", 1, msg.pos_acc());
        assertEquals("longitude", -73481550, msg.longitude());
        assertEquals("latitude", 28590700, msg.latitude());
        assertEquals("cog", 1750, msg.cog());
        assertEquals("true_heading", 174, msg.true_heading());
        assertEquals("utc_sec", 33, msg.utc_sec());
        assertEquals("regional", 0, msg.regional());
        assertEquals("spare", 0, msg.spare());
        assertEquals("raim", 0, msg.raim());
        assertEquals("sync_state", 0, msg.sync_state());
        assertEquals("slot_timeout", 3, msg.slot_timeout());
        assertEquals("sub_message", 1805, msg.sub_message());
    }

    @Test(expected = AISMessageException.class)
    public void testWrongBitLength() throws SixbitsExhaustedException, AISMessageException {
        Sixbit six_state = new Sixbit();
        six_state.init("19NS7Sp02wo?HETKA2K6mUM20<L"); // Short
        Message1 msg = new Message1();
        msg.parse(six_state);
    }
}
