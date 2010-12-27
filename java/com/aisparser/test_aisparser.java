package com.aisparser;
import static java.lang.System.out;

/*
 * My skeleton test class
 */
public class test_aisparser {

	static void test_sixbit( Sixbit ais_sixbit )
	{
		int	msgid;
		long mmsi;
		
		out.println("Testing sixbit...");
		try {
			msgid = (int) ais_sixbit.get(6);
			ais_sixbit.get(2);
			mmsi = ais_sixbit.get(30);
			out.printf("Length = %d\n", ais_sixbit.length());
			out.printf("msgid = %d\n", msgid);
			out.printf("mmsi = %d\n", mmsi);
		} catch ( SixbitsExhaustedException e ) {
			out.println("Ran out of bits!");
		}
	}
	
	
	static void test_nmea()
	{
		Nmea nmea_message = new Nmea();
		nmea_message.init("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
		
		if (nmea_message.checkChecksum() == 0)
			out.println("Checksum is OK");
		else
			out.println("Checksum is BAD");
	}

	static void test_vdm()
	{
		Vdm vdm_message = new Vdm();
		int result;
		
		try {
			result = vdm_message.add("!AIVDM,1,1,,B,19NS7Sp02wo?HETKA2K6mUM20<L=,0*27\r\n");
			out.printf("result = %d\n", result);
			
			Message1 msg = new Message1();
			msg.parse( vdm_message.sixbit() );
		} catch (Exception e) {
			out.printf("Error: %s\n", e.getMessage());
		}

		try {
			result = vdm_message.add("!AIVDM,2,1,6,B,55ArUT02:nkG<I8GB20nuJ0p5HTu>0hT9860TV16000006420BDi@E53,0*33");
			out.printf("result = %d\n", result);
			result = vdm_message.add("!AIVDM,2,2,6,B,1KUDhH888888880,2*6A");
			out.printf("result = %d\n", result);
			
			Message5 msg = new Message5();
			msg.parse( vdm_message.sixbit() );
			
		} catch (Exception e) {
			out.printf("Error: %s\n", e.getMessage());
		}
	}
	
	public static void main( String args[] )
	{
		//test_sixbit();
		//test_nmea();
		test_vdm();
	}
}
