# Page changer for PDF/Scores

> Based on
>
> Adafruit Feather Bluefruit M0
>
> Bluetooth HID keyboard
>
> https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/hidkeyboard

## Circuit and hardware setup

### LED Indicator

Hardware used: RGB LED (common cathode), [L1]

Wiring: http://wiring.org.co/learning/basics/rgbled.html

* attach Green pin to pin 10 through a 180ohm resistor
* attach Red pin to pin 11 through a 100ohm resistor
* attach Blue pin to pin 12 through a 100ohm resistor
* attach Cathode to GND

### Previous/next switches

Hardware used: Any 2 standard SPST switches, [S1, S2]

Wiring: http://www.studentcompanion.co.za/wp-content/uploads/2015/12/Arduino_Pushbutton-cct.jpg

* attach S1's left pole to Feather's 3.3V
* attach S1's right pole to Feather's pin 5
* attach S1's right pole to GND through a 10K resistor
* attach S2's left pole to Feather's 3.3V
* attach S2's right pole to Feather's pin 6
* attach S2's right pole to GND through a 10K resistor

### On/Off switch

Hardware used: Any standard SPST or SPDT switch, [S3]

Wiring: 

* attach S3's left pole to Feather's En pin
* attach S3's right pole to GND