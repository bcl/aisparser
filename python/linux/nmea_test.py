import nmea

def test_check_checksum():
    """Test correct check_checksum behavior"""
    result = nmea.check_checksum("!AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*0F")
    assert result == (True, 0x0F, 0x0F)

    result = nmea.check_checksum("$AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*0F")
    assert result == (True, 0x0F, 0x0F)

def test_check_checksum_wrong():
    result = nmea.check_checksum("!AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*11")
    assert result == (False, 0x0F, 0x11)

def test_check_checksum_errors():
    """Test error conditions in check_checksum"""

    # No start character
    result = nmea.check_checksum("AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*0F")
    assert result == (False, None, None)

    # Invalid start character
    result = nmea.check_checksum("@AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*0F")
    assert result == (False, None, None)

    # No checksum at the end
    result = nmea.check_checksum("!AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0")
    assert result == (False, 0x0F, -1)

    # Missing checksum character
    result = nmea.check_checksum("!AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*0")
    assert result == (False, 0x0F, 0)

def test_make_checksum():
    """Test correct make_checksum behavior"""
    result = nmea.make_checksum("!AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*00")
    assert result == 0x0F

    result = nmea.make_checksum("!AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*")
    assert result == 0x0F

    # No * at the end
    result = nmea.make_checksum("!AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0")
    assert result == 0x0F

def test_make_checksum_errors():
    # Invalid start character
    result = nmea.make_checksum("@AIVDM,1,1,,B,35Mj3MPOj@o?FVFK<5w3r3@L00di,0*")
    assert result == None
