/*
   Using the nRF24L01 radio module to communicate
   between two Arduinos with much increased reliability following
   various tutorials, conversations, and studying the nRF24L01 datasheet
   and the library reference.

   Transmitter is
   https://github.com/michaelshiloh/resourcesForClasses/tree/master/kicad/Arduino_Shield_RC_Controller

  Receiver is
  https://github.com/instituteforcriticalrobotics/instituteforcriticalrobotics.github.io/tree/main/kicad/nRF_Mega

   This file contains code for both transmitter and receiver.
   Transmitter at the top, receiver at the bottom.
   One of them is commented out.

   These sketches require the RF24 library by TMRh20
   Documentation here: https://nrf24.github.io/RF24/index.html

   change log

   11 Oct 2023 - ms - initial entry based on
                  rf24PerformingRobotsTemplate
   26 Oct 2023 - ms - revised for new board: nRF_Servo_Mega rev 2
   28 Oct 2023 - ms - add demo of NeoMatrix, servo, and Music Maker Shield
*/

// Common code
//

// Common pin usage
// Note there are additional pins unique to transmitter or receiver
//

// nRF24L01 uses SPI which is fixed
// on pins 11, 12, and 13 on the Uno
// and on pins 50, 51, and 52 on the Mega

// It also requires two other signals
// (CE = Chip Enable, CSN = Chip Select Not)
// Which can be any pins:

// For the transmitter
// const int NRF_CE_PIN = A4, NRF_CSN_PIN = A5;

// for the receiver
const int NRF_CE_PIN = A11, NRF_CSN_PIN = A15;

// In summary,
// nRF 24L01 pin    Uno Mega   name
//          1                     GND
//          2                     3.3V
//          3       9   A11       CE
//          4       10  A15       CSN
//          5       13  SCLK
//          6       11  MOSI/COPI
//          7       12  MISO/CIPO

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);  // CE, CSN

//#include <printf.h>  // for debugging

// See note in rf24Handshaking about address selection
//

// Channel and address allocation:
// Fatema, Jannah: Channel 30, addr = 0x76
// Andres, Ryan: Channel 40, addr = 0x73
// Nouf, Shaikha:  Channel 50, addr = 0x7C
// Aadhar, Shanaia: Channel 60, addr = 0xC6
// Akhat, Yunho:  Channel 70, addr = 0xC3
// Aakif, Marta: Channel 80, addr = 0xCC
// Yerk, Hamad: Channel 90, addr = 0x33
const byte addr = 0x7C;             // change as per the above assignment
const int RF24_CHANNEL_NUMBER = 50;  // change as per the above assignment

// Do not make changes here
const byte xmtrAddress[] = { addr, addr, 0xC7, 0xE6, 0xCC };
const byte rcvrAddress[] = { addr, addr, 0xC7, 0xE6, 0x66 };

const int RF24_POWER_LEVEL = RF24_PA_LOW;

// global variables
uint8_t pipeNum;
unsigned int totalTransmitFailures = 0;

struct DataStruct {
  uint8_t stateNumber;
};
DataStruct data;

void setupRF24Common() {

  // RF24 setup
  if (!radio.begin()) {
    Serial.println(F("radio  initialization failed"));
    while (1)
      ;
  } else {
    Serial.println(F("radio successfully initialized"));
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(RF24_CHANNEL_NUMBER);
  radio.setPALevel(RF24_POWER_LEVEL);
}

/*
  // Transmitter code

  // Transmitter pin usage
  const int LCD_RS_PIN = 3, LCD_EN_PIN = 2, LCD_D4_PIN = 4, LCD_D5_PIN = 5, LCD_D6_PIN = 6, LCD_D7_PIN = 7;
  const int SW1_PIN = 8, SW2_PIN = 9, SW3_PIN = 10, SW4_PIN = A3, SW5_PIN = A2;

  // LCD library code
  #include <LiquidCrystal.h>

  // initialize the library with the relevant pins
  LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);


  const int NUM_OF_STATES = 11;
  char *theStates[] = {"0 Robot Intro",
                     "1 I do not think...",
                     "2 Well, partner...",
                     "3 Etiquette, eh...",
                     "4 What does he...",
                     "5 Anyhow, I know...",
                     "6 Folks, I've seen...",
                     "7 I may be...",
                     "8 Uh yea you’re...",
                     "9 Magnus, I think...",
                     "10 Tell us..."
                    };

  void updateLCD() {

  lcd.clear();
  lcd.print(theStates[data.stateNumber]);
  lcd.setCursor(0, 1); // column, line (from 0)
  lcd.print("not transmitted yet");
  }

  void countDown() {
  if (--data.stateNumber < 0) {
    data.stateNumber = 0;
  }
  updateLCD();
  }

  void countUp() {
  if (++data.stateNumber >= NUM_OF_STATES) {
    data.stateNumber = NUM_OF_STATES-1;
  }
  updateLCD();
  }


  void spare1() {}
  void spare2() {}

  void rf24SendData() {

  radio.stopListening(); // go into transmit mode
  // The write() function will block
  // until the message is successfully acknowledged by the receiver
  // or the timeout/retransmit maxima are reached.
  int retval = radio.write(&data, sizeof(data));

  lcd.clear();
  lcd.setCursor(0, 0); // column, line (from 0)
  lcd.print("transmitting");
  lcd.setCursor(14, 0); // column, line (from 0)
  lcd.print(data.stateNumber);

  Serial.print(F(" ... "));
  if (retval) {
    Serial.println(F("success"));
    lcd.setCursor(0, 1); // column, line (from 0)
    lcd.print("success");
  } else {
    totalTransmitFailures++;
    Serial.print(F("failure, total failures = "));
    Serial.println(totalTransmitFailures);

    lcd.setCursor(0, 1); // column, line (from 0)
    lcd.print("error, total=");
    lcd.setCursor(13, 1); // column, line (from 0)
    lcd.print(totalTransmitFailures);
  }
  }

  class Button
  {
    int pinNumber;
    bool previousState;
    void(* buttonFunction)();
  public:

    // Constructor
    Button(int pn, void* bf) {
      pinNumber = pn;
      buttonFunction = bf;
      previousState = 1;
    }

    // update the button
    void update() {
      bool currentState = digitalRead(pinNumber);
      if (currentState == LOW && previousState == HIGH) {
        Serial.print("button on pin ");
        Serial.print(pinNumber);
        Serial.println();
        buttonFunction();
      }
      previousState = currentState;
    }
  };

  const int NUMBUTTONS = 5;
  Button theButtons[] = {Button(SW1_PIN, countDown),
                       Button(SW2_PIN, rf24SendData),
                       Button(SW3_PIN, countUp),
                       Button(SW4_PIN, spare1),
                       Button(SW5_PIN, spare2),
                      };

  void setupRF24() {

  setupRF24Common();

  // Set us as a transmitter
  radio.openWritingPipe(xmtrAddress);
  radio.openReadingPipe(1, rcvrAddress);

  // radio.printPrettyDetails();
  Serial.println(F("I am a transmitter"));

  data.stateNumber = 0;
  }

  void setup() {
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Radio init ....");

  setupRF24();

  // Initialize the switches
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(SW3_PIN, INPUT_PULLUP);
  pinMode(SW4_PIN, INPUT_PULLUP);
  pinMode(SW5_PIN, INPUT_PULLUP);
  lcd.setCursor(0, 1); // column, line (from 0)
  lcd.print("setup() finished");

  updateLCD();
  }



  void loop() {
  for (int i = 0; i < NUMBUTTONS; i++) {
    theButtons[i].update();
  }
  delay(50); // for testing
  }


  void clearData() {
  // set all fields to 0
  data.stateNumber = 0;
  }

  // End of transmitter code
*/

// Receiver Code

// Additional libraries for music maker shield
#include <Adafruit_VS1053.h>
#include <SD.h>

// Servo library
#include <Servo.h>

// Additional libraries for graphics on the Neo Pixel Matrix
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

// Additional pin usage for receiver

// Adafruit music maker shield
#define SHIELD_RESET -1  // VS1053 reset pin (unused!)
#define SHIELD_CS 7      // VS1053 chip select pin (output)
#define SHIELD_DCS 6     // VS1053 Data/command select pin (output)
#define CARDCS 4         // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3  // VS1053 Data request, ideally an Interrupt pin
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// Servo motors
const int HEAD_SERVO_PIN = 17; // TODO why doesn't pin 21 work?
const int CROWNECK_SERVO_PIN = 16;
const int CROWBEAK_SERVO_PIN = 18;

// Neopixel
const int NEOPIXELPIN1 = 20;
const int NEOPIXELPIN2 = 21;
const int NUMPIXELS = 64;
//#define NEOPIXELPIN 18
//#define NUMPIXELS 64  // change to fit
//Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix matrix1 = Adafruit_NeoMatrix(8, 8, NEOPIXELPIN1,
                             NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                             NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                             NEO_GRB            + NEO_KHZ800);

Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(8, 8, NEOPIXELPIN2,
                             NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                             NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                             NEO_GRB            + NEO_KHZ800);

Servo head;  // change names to describe what's moving
Servo crowNeck;
Servo crowBeak;

// change as per your robot
const int HEAD_LEFT = 52;
const int HEAD_RIGHT = 62;
const int CROWNECK_FAR_LEFT = 73;
const int CROWNECK_FAR_RIGHT = 166;
const int CROWBEAK_OPEN = 62;
const int CROWBEAK_CLOSED = 30;

void setup() {
  Serial.begin(9600);
  // printf_begin();

  // Set up all the attached hardware
  setupMusicMakerShield();
  setupServoMotors();
  setupNeoPixels();

  setupRF24();

  // Brief flash to show we're done with setup()
  flashNeoPixels();
}

void setupRF24() {
  setupRF24Common();

  // Set us as a receiver
  radio.openWritingPipe(rcvrAddress);
  radio.openReadingPipe(1, xmtrAddress);

  // radio.printPrettyDetails();
  Serial.println(F("I am a receiver"));
}

void setupMusicMakerShield() {
  if (!musicPlayer.begin()) {  // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1)
      ;
  }
  Serial.println(F("VS1053 found"));

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD card failed or not present"));
    while (1)
      ;  // don't do anything more
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20, 20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
}

void setupServoMotors() {
  head.attach(HEAD_SERVO_PIN);
  head.write(56);
  crowNeck.attach(CROWNECK_SERVO_PIN);
  crowNeck.write(80);
  crowBeak.attach(CROWBEAK_SERVO_PIN);
  crowBeak.write(CROWBEAK_CLOSED);
}

void setupNeoPixels() {
  //  pixels.begin();
  //  pixels.clear();
  //  pixels.show();
  matrix1.begin();
  matrix2.begin();
  matrix1.setTextWrap(false);
  matrix2.setTextWrap(false);
  matrix1.setBrightness(40);
  matrix2.setBrightness(40);
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();
}

void flashNeoPixels() {

  // Using the Matrix library
  matrix1.fillScreen(matrix1.Color(0, 255, 0));
  matrix1.show();
  delay(500);
  matrix1.fillScreen(0);
  matrix1.show();

  //  // all on
  //  for (int i = 0; i < NUMPIXELS; i++) {  // For each pixel...
  //    pixels.setPixelColor(i, pixels.Color(0, 100, 0));
  //  }
  //  pixels.show();
  //  delay(500);
  //
  //  // all off
  //  pixels.clear();
  //  pixels.show();
}

void loop() {

  /////////////////////////////////////////////////// BLINKING EYES  ////////////////////////////////////////////////////////////

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(3000);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // from the top!

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // Draw the shapes on matrix 1
  matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
  matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
  matrix1.show();

  // Draw the same shapes on matrix 2
  matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
  matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
  matrix2.show();

  delay(100);

  // Clear both matrices
  matrix1.fillScreen(0);
  matrix2.fillScreen(0);
  matrix1.show();
  matrix2.show();

  // If there is data, read it,
  // and do the needfull
  // Become a receiveR

  radio.startListening();
  if (radio.available(&pipeNum)) {
    radio.read(&data, sizeof(data));
    Serial.print(F("message received Data = "));
    Serial.print(data.stateNumber);
    Serial.println();



    switch (data.stateNumber) {
      case 0:

        ///////////////////////////////////////// EYE ROLL //////////////////////////////////////////////

        break;
      case 1:

        musicPlayer.startPlayingFile("/track001.mp3");

        //////////////////////////////////////// RED EYES //////////////////////////////////////////////

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 0, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 0, 30));
        matrix2.show();

        delay(1000);

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 0, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 0, 30));
        matrix2.show();

        delay(1000);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        /////////////////////////////////////// CROW SCREECH //////////////////////////////////////////

        musicPlayer.startPlayingFile("/crow01.mp3");

        crowBeak.write(CROWBEAK_OPEN);
        delay(500);
        crowBeak.write(CROWBEAK_CLOSED);
        delay(500);

        musicPlayer.startPlayingFile("/crow01.mp3");

        crowBeak.write(CROWBEAK_OPEN);
        delay(500);
        crowBeak.write(CROWBEAK_CLOSED);
        delay(500);

        break;

      case 2:

        musicPlayer.startPlayingFile("/track002.mp3");

        ////////////////////////////////////// EYE ROLL //////////////////////////////////////

        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 4, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 4, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();

        delay(1000);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(4, 3, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(4, 3, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 1, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 1, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 4, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 4, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();

        delay(1000);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(4, 3, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(4, 3, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 1, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 1, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 4, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 4, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();

        delay(1000);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(4, 3, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(4, 3, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 1, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 1, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 4, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 4, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();

        delay(1000);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(4, 3, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(4, 3, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 1, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 1, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        break;
      case 3:

        musicPlayer.startPlayingFile("/track003.mp3");

        ///////////////////////////////////////// FAST BLINK ////////////////////////////////////////////

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // from the top!

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // from the top!

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // from the top!

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // from the top!

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 4, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 2, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 4, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 2, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 5, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 5, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 6, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(3, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 6, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(3, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 7, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(4, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 7, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(4, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(50);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        //////////////////////////////////////////////////// CROW SHAKES HEAD ////////////////////////////////////////////////

                // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();
        
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(800);
        crowNeck.write(CROWNECK_FAR_RIGHT);
        delay(800);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(800);
        crowNeck.write(CROWNECK_FAR_RIGHT);
        delay(800);
        crowNeck.write(119);

        break;

      case 4:

        musicPlayer.startPlayingFile("/track004.mp3");

        ////////////////////////////////////////////////////// SHOCKED EYES ////////////////////////////////////////////

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 2, 6, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 2, 6, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 2, 6, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 2, 6, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 2, 6, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 2, 6, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 2, 6, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 2, 6, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 2, 6, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 2, 6, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(500);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        ////// crow screech ////////

                        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();
        
        musicPlayer.startPlayingFile("/crow01.mp3");

        crowBeak.write(CROWBEAK_OPEN);
        delay(500);
        crowBeak.write(CROWBEAK_CLOSED);

        musicPlayer.startPlayingFile("/crow01.mp3");

        crowBeak.write(CROWBEAK_OPEN);
        delay(500);
        crowBeak.write(CROWBEAK_CLOSED);

        break;

      case 5:

        musicPlayer.startPlayingFile("/track005.mp3");

        ////////////////////////////////////////////////// SHAKES HEAD ////////////////////////////////////
        ////////////////////////////////////////////////// CROW SHAKES HEAD ////////////////////////////////

                        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(500);
        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);
        delay(500);
        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(500);
        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);
        delay(500);
        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(500);
        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);

        break;

      case 6:

        musicPlayer.startPlayingFile("/track006.mp3");

        ///////////////////////////////////////////// SIDE EYE, SHAKES HEAD ////////////////////////////////////

        head.write(HEAD_RIGHT);
        
        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();
        
        head.write(HEAD_RIGHT);
        
        delay(1000);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

                // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_RIGHT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

                head.write(HEAD_RIGHT);
        
        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();
        
        head.write(HEAD_RIGHT);
        
        delay(1000);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

                // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_RIGHT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

                head.write(HEAD_RIGHT);
        
        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();
        
        head.write(HEAD_RIGHT);
        
        delay(1000);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

                // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_RIGHT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

                head.write(HEAD_RIGHT);
        
        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();
        
        head.write(HEAD_RIGHT);
        
        delay(1000);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

                // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_RIGHT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        /////////////////////////////////// CROW SCREECH AND SHAKE HEAD ///////////////////

        musicPlayer.startPlayingFile("/crow01.mp3");
        crowBeak.write(CROWBEAK_OPEN);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(500);
        crowBeak.write(CROWBEAK_CLOSED);
        crowNeck.write(CROWNECK_FAR_RIGHT);
        delay(500);

        musicPlayer.startPlayingFile("/crow01.mp3");
        crowBeak.write(CROWBEAK_OPEN);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(500);
        crowBeak.write(CROWBEAK_CLOSED);
        crowNeck.write(CROWNECK_FAR_RIGHT);
        delay(500);

        break;

      case 7:

      /////////////////////////////////////// EYE ROLL & SIDEYE /////////////////////////////////////////////

        musicPlayer.startPlayingFile("/track007.mp3");

        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 4, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 4, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();

        delay(1000);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(4, 3, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(4, 3, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the outer rectangle
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 1, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 1, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();
        delay(300);

        matrix1.fillScreen(0); // Clear the matrix
        matrix2.fillScreen(0); // Clear the matrix
        matrix1.show();
        matrix2.show();

        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix1.fillRect(2, 4, 3, 3, matrix1.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix1.show();

        // Draw the outer rectangle
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        // Draw the blinking inner rectangle
        matrix2.fillRect(2, 4, 3, 3, matrix2.Color(255, 120, 30)); // Draw the blinking inner rectangle
        matrix2.show();

        delay(1000);
        
        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        head.write(HEAD_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

        // Draw the shapes with the blinking inner rectangle on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 4, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes with the blinking inner rectangle on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(0, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 4, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();
        
        head.write(HEAD_RIGHT);
        
        delay(1000);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();
        delay(10);

                // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(0, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        break;

      case 8:
      
      ///////////////////////////////////////////// RED EYES /////////////////////////////////////////////
      
              // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 0, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 0, 30));
        matrix2.show();

        delay(1000);

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        delay(1000);

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 0, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 0, 30));
        matrix2.show();

        break;

      case 9:

        musicPlayer.startPlayingFile("/track008.mp3");

        ///////////////////////////////////////////////// SHAKES HEAD ////////////////////////////////////////////////
        ////////////////////////////////////////////////// CROW SHAKES HEAD ////////////////////////////////////////////

                        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(500);
        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);
        delay(500);
        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(500);
        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);
        delay(500);
        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);
        delay(500);
        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);

        break;

      case 10:

      ////////////////////////////////////////// SHOCKED, SHAKING HEAD ///////////////////////////////////////

        musicPlayer.startPlayingFile("/track009.mp3");

       // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);

        delay(500);

        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 2, 6, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 2, 6, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);

        delay(500);

        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);

        delay(500);

        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(1, 2, 6, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(1, 2, 6, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);

        delay(500);

        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 3, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 3, 4, matrix2.Color(255, 120, 30));
        matrix2.show();

        head.write(HEAD_RIGHT);
        crowNeck.write(CROWNECK_FAR_RIGHT);

        delay(500);

        head.write(HEAD_LEFT);
        crowNeck.write(CROWNECK_FAR_LEFT);

        // Clear both matrices
        matrix1.fillScreen(0);
        matrix2.fillScreen(0);
        matrix1.show();
        matrix2.show();

        /////////////////////////////////////// CROW SCREECH /////////////////////////////////////////

                        // Draw the shapes on matrix 1
        matrix1.drawRect(0, 0, 8, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(5, 0, 3, 8, matrix1.Color(255, 120, 30));
        matrix1.fillRect(2, 2, 4, 4, matrix1.Color(255, 120, 30));
        matrix1.show();

        // Draw the same shapes on matrix 2
        matrix2.drawRect(0, 0, 8, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(5, 0, 3, 8, matrix2.Color(255, 120, 30));
        matrix2.fillRect(2, 2, 4, 4, matrix2.Color(255, 120, 30));
        matrix2.show();
        
        musicPlayer.startPlayingFile("/crow01.mp3");

        crowBeak.write(CROWBEAK_OPEN);
        delay(500);
        crowBeak.write(CROWBEAK_CLOSED);

        musicPlayer.startPlayingFile("/crow01.mp3");

        crowBeak.write(CROWBEAK_OPEN);
        delay(500);
        crowBeak.write(CROWBEAK_CLOSED);

        break;

      default:
        Serial.println(F("Invalid option"));
    }

  }
}  // end of loop()
// end of receiver code
