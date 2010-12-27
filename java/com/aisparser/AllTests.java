package com.aisparser;

import junit.framework.Test;
import junit.framework.TestSuite;

public class AllTests {

	public static Test suite() {
		TestSuite suite = new TestSuite("Test for com.aisparser");
		//$JUnit-BEGIN$
		suite.addTestSuite(Message19Test.class);
		suite.addTestSuite(Message14Test.class);
		suite.addTestSuite(Message17Test.class);
		suite.addTestSuite(Message23Test.class);
		suite.addTestSuite(Message1Test.class);
		suite.addTestSuite(Message12Test.class);
		suite.addTestSuite(Message4Test.class);
		suite.addTestSuite(Message3Test.class);
		suite.addTestSuite(Message15Test.class);
		suite.addTestSuite(VdmTest.class);
		suite.addTestSuite(Message2Test.class);
		suite.addTestSuite(Message16Test.class);
		suite.addTestSuite(Message8Test.class);
		suite.addTestSuite(Message5Test.class);
		suite.addTestSuite(Message11Test.class);
		suite.addTestSuite(Message6Test.class);
		suite.addTestSuite(Message9Test.class);
		suite.addTestSuite(Message24Test.class);
		suite.addTestSuite(Message10Test.class);
		suite.addTestSuite(SotdmaTest.class);
		suite.addTestSuite(Message21Test.class);
		suite.addTestSuite(SixbitTest.class);
		suite.addTestSuite(Message13Test.class);
		suite.addTestSuite(MessagesTest.class);
		suite.addTestSuite(Message18Test.class);
		suite.addTestSuite(ItdmaTest.class);
		suite.addTestSuite(Message7Test.class);
		suite.addTestSuite(NmeaTest.class);
		suite.addTestSuite(Message20Test.class);
		suite.addTestSuite(Message22Test.class);
		//$JUnit-END$
		return suite;
	}

}
