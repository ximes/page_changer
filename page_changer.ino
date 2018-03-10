/*
Page changer for PDF/Scores

Based on
Adafruit Feather Bluefruit M0
Bluetooth HID keyboard
https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/hidkeyboard

Circuit and hardware setup

LED Indicator
Hardware used: RGB LED (common cathode), [L1]
Wiring: http://wiring.org.co/learning/basics/rgbled.html

* attach Green pin to pin 10 through a 180ohm resistor
* attach Red pin to pin 11 through a 100ohm resistor
* attach Blue pin to pin 12 through a 100ohm resistor
* attach Cathode to GND

Previous/next switches
Hardware used: Any 2 standard SPST switches, [S1, S2]
Wiring: http://www.studentcompanion.co.za/wp-content/uploads/2015/12/Arduino_Pushbutton-cct.jpg

* attach S1's left pole to Feather's 3.3V
* attach S1's right pole to Feather's pin 5
* attach S1's right pole to GND through a 10K resistor
* attach S2's left pole to Feather's 3.3V
* attach S2's right pole to Feather's pin 6
* attach S2's right pole to GND through a 10K resistor

On/Off switch
Previous/next switches
Hardware used: Any standard SPST or SPDT switch, [S3]
Wiring: 
* attach S3's left pole to Feather's En pin
* attach S3's right pole to GND

*/

#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined(ARDUINO_ARCH_SAMD)
#include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE         0 //prevent factory reset/debug
#define MINIMUM_FIRMWARE_VERSION    "0.7.0"

// Set forward button pin and action codes
int forwardButtonState;
int lastForwardButtonState = LOW;   // the previous reading from the input pin
int lastForwardBounceTime = 0;

// Set back button pin and action codes
int backButtonState;
int lastBackButtonState = LOW;   // the previous reading from the input pin
int lastBackBounceTime = 0;

// Set bounce and delay timings
int forwardHold = 0;
int forwardSingle = 0;
long forwardOnTime = 0;
long lastForwardSwitchTime = 0;
int backHold = 0;
int backSingle = 0;
long backOnTime = 0;
long lastBackSwitchTime = 0;
int redVal;
int blueVal;
int greenVal;

// Battery check pin
#define VBATPIN A7 //Feather's specific

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup(void)
{
  // Disabling serial check for operating this without usb (e.g. bluetooth only)
  //while (!Serial);  // required for Flora & Micro

  Serial.begin(115200);
  Serial.println(F("Sdc's Page Changer"));
  Serial.println(F("---------------------------------------"));

  Serial.println(F("Booting"));

  blinkGreen();

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  /* Change the device name to make it easier to find */
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Sdc's Score Page Changer" )) ) {
    error(F("Could not set device name?"));
  }

  /* Enable HID Service */
  Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=1" ))) {
    error(F("Could not enable Keyboard"));
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }

  Serial.println();

  // Setting up pins
  pinMode(forwardButtonPin, INPUT);
  pinMode(backButtonPin, INPUT);

  // Led pin test
  pinMode(REDPin, OUTPUT);
  pinMode(GREENPin, OUTPUT);
  pinMode(BLUEPin, OUTPUT);
  digitalWrite(GREENPin, LOW);
  digitalWrite(BLUEPin, LOW);
  digitalWrite(REDPin, LOW);

  // Set basic led colour to blue
  digitalWrite(BLUEPin, HIGH);

  // Startup notification
  blinkGreen();
  blinkGreen();
  blinkGreen();

  // Check and print battery level
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  Serial.print("VBat: " ); Serial.println(measuredvbat);

  delay(500);

  Serial.println(F("Ready."));
}

void loop(void)
{
  // Check and print battery level
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage

  // If battery becomes full, send a green visual alert
  if (measuredvbat >= chargedVoltage) {
    digitalWrite(GREENPin, HIGH);
    // else battery is low, send a red visual alert
  } else if (measuredvbat < dischargedVoltage) {
    digitalWrite(GREENPin, LOW);
    for ( int i = 0 ; i < 255 ; i += 1 ) {
      redVal += 1;
      blueVal -= 1;
      analogWrite( REDPin, 255 - redVal );
      analogWrite( BLUEPin, 50 - blueVal );
      delay(0.5);
    }
    delay(100);
    for ( int i = 255 ; i > 0 ; i -= 1 ) {
      redVal -= 1;
      blueVal += 1;
      analogWrite( REDPin, 255 - redVal );
      analogWrite( BLUEPin, 150 - blueVal );
      delay( 1 );
    }

  }

  // Read button press and filter button bounces
  int forwardButtonReading = digitalRead(forwardButtonPin);
  int backButtonReading = digitalRead(backButtonPin);

  if (forwardButtonReading != lastForwardButtonState ) {
    lastForwardBounceTime = millis();
  }
  if (backButtonReading != lastBackButtonState ) {
    lastBackBounceTime = millis();
  }

  forwardButtonState = forwardButtonReading;
  backButtonState = backButtonReading;

  // Buttons are first pressed
  if (forwardButtonState == HIGH && lastForwardButtonState == LOW) {
    forwardOnTime = millis();
  }
  if (backButtonState == HIGH && lastBackButtonState == LOW) {
    backOnTime = millis();
  }

  // Buttons are held in place
  if (forwardButtonState == HIGH && lastForwardButtonState == HIGH) {
    if ((millis() - forwardOnTime) > holdTime) {
      Serial.println("held");
      pressButton(forwardHoldButtonCode);
      forwardHold = 1;
    }
  }
  if (backButtonState == HIGH && lastBackButtonState == HIGH) {
    if ((millis() - backOnTime) > holdTime) {
      Serial.println("held");
      pressButton(backHoldButtonCode);
      backHold = 1;
    }
  }

  // Buttons are released
  if (forwardButtonState == LOW && lastForwardButtonState == HIGH) {
    if (((millis() - forwardOnTime) > bounceTime) && forwardHold  != 1) {
      onForwardButtonRelease();
    }
    if (forwardHold == 1) {
      forwardHold  = 0;
    }
  }

  if (forwardSingle == 1 && (millis() - lastForwardSwitchTime) > doubleTime) {
    forwardSingle = 0;
  }

  if (backButtonState == LOW && lastBackButtonState == HIGH) {
    if (((millis() - backOnTime) > bounceTime) && backHold  != 1) {
      onBackButtonRelease();
    }
    if (backHold == 1) {
      backHold  = 0;
    }
  }

  if (backSingle == 1 && (millis() - lastBackSwitchTime) > doubleTime) {
    backSingle = 0;
  }

  lastForwardButtonState = forwardButtonReading;
  lastBackButtonState = backButtonReading;
}

void onForwardButtonRelease() {
  if ((millis() - lastForwardSwitchTime) >= doubleTime) {
    // Serial.println(forwardButtonCode);
    Serial.println("single press");
    forwardSingle = 1;
    pressButton(forwardButtonCode);
    lastForwardSwitchTime = millis();
    return;
  }
  if ((millis() - lastForwardSwitchTime) < doubleTime) {
    // Serial.println(forwardDoubleButtonCode);
    Serial.println("double press");
    pressButton(forwardDoubleButtonCode);
    forwardSingle = 0;
    lastForwardSwitchTime = millis();
  }
}

void onBackButtonRelease() {
  if ((millis() - lastBackSwitchTime) >= doubleTime) {
    // Serial.println(backButtonCode);
    Serial.println("single press");
    backSingle  = 1;
    pressButton(backButtonCode);
    lastBackSwitchTime = millis();
    return;
  }
  if ((millis() - lastBackSwitchTime) < doubleTime) {
    // Serial.println(backDoubleButtonCode);
    Serial.println("double press");
    pressButton(backDoubleButtonCode);
    backSingle = 0;
    lastBackSwitchTime = millis();
  }
}

// Routing for blinking a green led once
void blinkGreen() {
  digitalWrite(GREENPin, HIGH);
  digitalWrite(BLUEPin, LOW);
  digitalWrite(REDPin, LOW);
  delay(delayTime);
  digitalWrite(GREENPin, LOW);
  digitalWrite(BLUEPin, HIGH);
  digitalWrite(REDPin, LOW);
  delay(100);
}

/**************************************************************************/
/*!
    @brief  Wrap the press button calls
*/
/**************************************************************************/
void pressButton(String code) {
  // Send button press code
  ble.print("AT+BleKeyboardCode=");
  ble.println(code);
  delay(1);
  // Send keyboard button release code
  ble.print("AT+BLEKEYBOARDCODE=");
  ble.println("00-00");

  //Visual feedback (green flash) on button press and release
  blinkGreen();

  if ( ble.waitForOK() )
  {
    Serial.println( F("OK!") );
  } else
  {
    Serial.println( F("FAILED!") );
  }
}
