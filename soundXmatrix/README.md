# Syncing the Neopixel matrix display with 3 sound tracks
- Objective:
To develop a synchronized visual and audio experience using the Adafruit NeoPixel Matrix and the arduino mega music maker shield.

## Process
We started by setting up the NeoPixel Matrix. This involved configuring the matrix width, height, pin, and orientation:

```
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define PIN 49

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);
```

We then created three display modes with the shape functions in the neopixel matrix library:
- Square Track: Simple square outlines that grow smaller over time.
- Circle Track: filled circles that grow bigger.
- Triangle Track: A triangle animation moving downwards.

We then worked on the audio integration. This involved setting up the board and defining pins for communication (voltage, ground, and pin 49). The challenge here was to play 
the track and display its respective visual simultaneously. Timing was crucial. After multiple iterations, 
we were able to sync the visual tracks with the 8-second tracks. The circle and triangle tracks required precise delay adjustments to match the 8-second mark:

```
delay(2650);  // Display for 2.65 seconds
```

Here is the [link](https://drive.google.com/file/d/1KJtta6wOBliOEjJkawJlqGtaNAs1xMzT/view?usp=sharing) to the video showcasing the full animation and sound

Here is the full code:

```
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#define PIN 49
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

// NeoPixel setup
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

// VS1053 setup
#define SHIELD_RESET  -1      
#define SHIELD_CS     7      
#define SHIELD_DCS    6      
#define CARDCS 4     
#define DREQ 3       
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void setup() {
  Serial.begin(9600);

  // NeoPixel matrix setup
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.fillScreen(0);
  matrix.show();

  // VS1053 setup
  if (! musicPlayer.begin()) {
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  
  }
  musicPlayer.setVolume(20,20);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
}

void loop() {
  // Square track and animation
  musicPlayer.startPlayingFile("/track001.mp3");
  for(int i = 0; i < 3; i++) {
    if(!musicPlayer.playingMusic) break;  // Exit the loop if the track has stopped
    matrix.drawRect(i, i, 8 - i*2, 8 - i*2, matrix.Color(0, 255, 255 - i*85)); 
    matrix.show();
    delay(1000);
    matrix.fillScreen(0); 
    matrix.show();
    delay(1000);
  }
  while(musicPlayer.playingMusic) delay(10);  // Wait for the track to finish before moving on
  
  // Circle track and animation
  musicPlayer.startPlayingFile("/track002.mp3");
  for(int i = 0; i < 3; i++) {
    if(!musicPlayer.playingMusic) break;  // Exit the loop if the track has stopped
    matrix.fillCircle(4, 4, 2 + i, matrix.Color(255 - i*40, 0, 0)); 
    matrix.show();
    delay(2650);  // Display for 2.65 seconds
    matrix.fillScreen(0);
    matrix.show();
    delay(10);
  }
  while(musicPlayer.playingMusic) delay(10);  // Wait for the track to finish before moving on

  // Triangle track and animation
  musicPlayer.startPlayingFile("/track003.mp3");
  for(int i = 0; i < 3; i++) {
    if(!musicPlayer.playingMusic) break;  // Exit the loop if the track has stopped
    matrix.fillTriangle(4, i*2, 0, 6, 7, 6, matrix.Color(255, 150, 0)); 
    matrix.show();
    delay(2650);  // Display for 2.65 seconds
    matrix.fillScreen(0);
    matrix.show();
    delay(10);
  }
  while(musicPlayer.playingMusic) delay(10);  // Wait for the track to finish before moving on
  
  delay(10000);
}
```
