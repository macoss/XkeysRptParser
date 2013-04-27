
#include "avrpins.h"
#include "max3421e.h"
#include "usbhost.h"
#include "usb_ch9.h"
#include "Usb.h"
#include "hid.h"
#include "hiduniversal.h"

#include <SPI.h>
#include <hidxkeysrptparser.h>

/******************************************************************************
  Create a subclass of the XkeysReportParser and override the OnKey methods 
  to handle the key reports when they are received.
******************************************************************************/
class XKeys : public XkeysReportParser
{

public:
        XKeys();
protected:
  virtual void OnKeyUp(uint8_t keyId); 
  virtual void OnKeyDown(uint8_t keyId);
};

XKeys::XKeys() : XkeysReportParser() {}

void XKeys::OnKeyDown(uint8_t keyId)
{
    Serial.print("Key down: ");
    Serial.println(keyId, DEC);
}

void XKeys::OnKeyUp(uint8_t keyId)
{
    Serial.print("Key Up: ");
    Serial.println(keyId, DEC);
    // TODO: add support for updating the 
    //setLED(0,LED_BLUE, LED_FLASH);
}

XKeys XKs;

void setup()
{
  
  Serial.begin( 115200 );
/******************************************************************************
    If you are running on boards like the Ethernet you might need 
    set the pin modes and right them to high to get the SPI bus to
    allow the USB Host Shield to work.
******************************************************************************/
  //pinMode(10, OUTPUT);
  //pinMode(4, OUTPUT);
  //digitalWrite(10, HIGH);
  //digitalWrite(4, HIGH);
  
  Serial.println("Start");
      
  //Setup the XKs report parser
  XKs.init();

  
}

void loop()
{
    // Excutes the USB.init() function to receive reports
    XKs.runLoop();
}

