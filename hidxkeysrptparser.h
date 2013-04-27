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

#if !defined(__HIDXKEYSRPTPARSER_H__)
#define __HIDXKEYSPARSER_H__

#include <inttypes.h>
#include <avr/pgmspace.h>
#include "avrpins.h"
#include "max3421e.h"
#include "usbhost.h"
#include "usb_ch9.h"
#include "Usb.h"
#include "hid.h"
#include "hiduniversal.h"

#if defined(ARDUINO) && ARDUINO >=100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif

#include "printhex.h"
#include "hexdump.h"
#include "message.h"
#include "confdescparser.h"


#define RPT_XKEYS_LEN		6
#define LED_BLUE                0
#define LED_RED                 32
#define LED_RPT_LEN             36
#define LED_OFF                 0
#define LED_ON                  1
#define LED_FLASH               2


class XkeysReportParser : public HIDReportParser
{
  
	uint8_t				oldRpt[RPT_XKEYS_LEN];
	uint32_t			oldButtons;
       
private:
        USB                             pUsb;
        HIDUniversal                    Hid;

public:
	XkeysReportParser();
	virtual void Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
        virtual void setLED(uint8_t keyId, uint8_t color, uint8_t mode);
        virtual void runLoop();
        virtual void init();
protected:
        // Subclass XkeysReportParser and define these methods to receive events
	virtual void OnKeyUp(uint8_t keyId) {}; 
	virtual void OnKeyDown(uint8_t keyId) {};

};

#endif // __HIDXKEYSRPTPARSER_H__
