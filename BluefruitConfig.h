// COMMON SETTINGS
// ----------------------------------------------------------------------------------------------
// These settings are used in both SW UART, HW UART and SPI mode
// ----------------------------------------------------------------------------------------------
#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   false  // If set to 'true' enables debug output


// SOFTWARE UART SETTINGS
// ----------------------------------------------------------------------------------------------
// The following macros declare the pins that will be used for 'SW' serial.
// You should use this option if you are connecting the UART Friend to an UNO
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_SWUART_RXD_PIN       1//9    // Required for software serial!
#define BLUEFRUIT_SWUART_TXD_PIN       2//10   // Required for software serial!
#define BLUEFRUIT_UART_CTS_PIN         3//11   // Required for software serial!
#define BLUEFRUIT_UART_RTS_PIN         -1   // Optional, set to -1 if unused


// HARDWARE UART SETTINGS
// ----------------------------------------------------------------------------------------------
// The following macros declare the HW serial port you are using. Uncomment
// this line if you are connecting the BLE to Leonardo/Micro or Flora
// ----------------------------------------------------------------------------------------------
#ifdef Serial1    // this makes it not complain on compilation if there's no Serial1
  #define BLUEFRUIT_HWSERIAL_NAME      Serial1
#endif


// SHARED UART SETTINGS
// ----------------------------------------------------------------------------------------------
// The following sets the optional Mode pin, its recommended but not required
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_UART_MODE_PIN        -1    // Set to -1 if unused


// HARDWARE SPI SETTINGS
// ----------------------------------------------------------------------------------------------
// The following macros declare the pins to use for HW SPI communication.
// SCK, MISO and MOSI should be connected to the HW SPI pins on the Uno, etc.
// This should be used with nRF51822 based Bluefruit LE modules that use SPI.
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended, set to -1 if unused

// PINS AND DELAY SETTINGS
#define delayTime 30
int bounceTime = 45; // Antibounce timing adjust, ms
int holdTime = 250;  // Hold timing adjust, ms
int doubleTime = 500; // Min timing for double press, ms
int GREENPin = 10;  // GREEN pin of the LED to PWM pin 10
int REDPin = 11;    // RED pin of the LED to PWM pin 11
int BLUEPin = 12;   // BLUE pin of the LED to PWM pin 12

// 'NEXT' BUTTON PIN AND ACTION CODES
const int forwardButtonPin = 6;
String forwardButtonCode = "00-00-4e"; // page down key
String forwardHoldButtonCode = "00-00-51"; // arrow down key
String forwardDoubleButtonCode = "00-00-4D"; // end key

// 'PREVIOUS' BUTTON PIN AND ACTION CODES
const int backButtonPin = 5;
String backButtonCode = "00-00-4b";  // page up key
String backHoldButtonCode = "00-00-52"; // arrow upkey
String backDoubleButtonCode = "00-00-4A"; // home key

// 
const float chargedVoltage = 4.21;
const float dischargedVoltage = 3.8;
