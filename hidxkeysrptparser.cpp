// XkeysRptParser - A project to read an Xkeys keypad on an Arduino
// Copyright (C) 2013 Rick Russell

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2
// of the License.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "hidxkeysrptparser.h"

XkeysReportParser::XkeysReportParser()
: Hid(&pUsb)
{
	oldButtons = 0;


  	for (uint8_t i=0; i<RPT_XKEYS_LEN; i++)
		oldRpt[i]	= 0xD; 

}

void XkeysReportParser::init()
{
        if (pUsb.Init() == -1)
            Serial.println("OSC did not start.");

        delay( 200 );

        if (!Hid.SetReportParser(0, (HIDReportParser*)this))
            ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  ); 
}

void XkeysReportParser::Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
	bool match = true;

	// Checking if there are changes in report since the method was last called
	for (uint8_t i=0; i<RPT_XKEYS_LEN; i++)
		if (buf[i] != oldRpt[i])
		{
			match = false;
			break;
		}

	// Calling xkeys event handler
	if (!match)
	{
		for (uint8_t i=0; i<RPT_XKEYS_LEN; i++) oldRpt[i] = buf[i];
	}
	

	uint32_t buttons = (0x00000000 | buf[5]);
	buttons <<= 24;
	buttons |= ((uint32_t)buf[4] << 16);
	buttons |= ((uint32_t)buf[3] << 8);
        buttons |= ((uint32_t)buf[2] << 0);
	uint32_t changes = (buttons ^ oldButtons);
 

	// Calling Button Event Handler for every button changed
	if (changes)
	{
		for (uint8_t i=0; i<0x1e; i++)
		{
			uint32_t mask = ((uint32_t)0x01 << i);

			if (((mask & changes) > 0))
				if ((buttons & mask) > 0) {
					OnKeyDown(i);
                                } else {
					OnKeyUp(i);
                                }
		}
		oldButtons = buttons;
	}
}

void XkeysReportParser::setLED(uint8_t keyId, uint8_t color, uint8_t mode)
{
  uint8_t rpt[LED_RPT_LEN];
  
  for (uint8_t i=0; i<RPT_XKEYS_LEN; i++)
		rpt[i]	= 0x0; 
  
  rpt[0] = 0;
  rpt[1] = 181;
  rpt[2] = keyId;
  rpt[3] = mode;
  
  //uint8_t SetReport( uint8_t ep, uint8_t iface, uint8_t report_type, uint8_t report_id, uint16_t nbytes, uint8_t* dataptr );
  uint8_t ret = Hid.SetReport(4,0,2,0,LED_RPT_LEN,rpt);
  Serial.print("SetReport Return: ");
  Serial.println(ret, HEX);
  
}

void XkeysReportParser::runLoop()
{
      pUsb.Task();
}
