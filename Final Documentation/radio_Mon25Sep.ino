/* USE THIS ONE FOR SPEED IN MOVEMENT
   Minimal decoding of multiple RC channels using pin change interrupts

   Does not check for reasonable values or timeouts

   Based on https://ryanboland.com/blog/reading-rc-receiver-values

  change log

  02 may 2022 - ms - initial entry
  24 may 2022 - ms - changed RC_CHx_PIN names
  15 sep 2022 - ms - changed pin numbers to new assignments

*/



// install this library from the library manager
// by Mike "GreyGnome" Schwager
#include <EnableInterrupt.h>

#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS  4

#define RC_CH1  1
#define RC_CH2  2
#define RC_CH3  3
#define RC_CH4  4

// pin usage
//radio
#define RC_CH1_PIN  8
#define RC_CH2_PIN  7
#define RC_CH3_PIN  4
#define RC_CH4_PIN  2
// motor drive
const int MOTOR1_PIN1 = 6;
const int MOTOR1_PIN2 = 5;
const int MOTOR2_PIN1 = 3;
const int MOTOR2_PIN2 = 9;

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

void rc_read_values() {
  noInterrupts();
  memcpy(rc_values, (const void *)rc_shared, sizeof(rc_shared));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    rc_start[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - rc_start[channel]);
    rc_shared[channel] = rc_compare;
  }
}

void calc_ch1() {
  calc_input(RC_CH1, RC_CH1_PIN);
}
void calc_ch2() {
  calc_input(RC_CH2, RC_CH2_PIN);
}
void calc_ch3() {
  calc_input(RC_CH3, RC_CH3_PIN);
}
void calc_ch4() {
  calc_input(RC_CH4, RC_CH4_PIN);
}

void reverse(int speed) {
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]);
      Serial.print(" front: "); Serial.println(speed);
  analogWrite(MOTOR1_PIN1, 0);
  analogWrite(MOTOR1_PIN2, speed);
  analogWrite(MOTOR2_PIN1, 0);
  analogWrite(MOTOR2_PIN2, speed);
}

void forward(int speed) {
  Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]);
      Serial.print(" front: "); Serial.println(speed);
  analogWrite(MOTOR1_PIN1, speed);
  analogWrite(MOTOR1_PIN2, 0);
  analogWrite(MOTOR2_PIN1, speed);
  analogWrite(MOTOR2_PIN2, 0);
}

void turnRight(int speed) {
  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]);
      Serial.print(" right: "); Serial.println(speed);
  analogWrite(MOTOR1_PIN1, speed);
  analogWrite(MOTOR1_PIN2, 0);
  analogWrite(MOTOR2_PIN1, 0);
  analogWrite(MOTOR2_PIN2, speed);
}

void turnLeft(int speed) {
  Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]);
      Serial.print(" right: "); Serial.println(speed);
  analogWrite(MOTOR1_PIN1, 0);
  analogWrite(MOTOR1_PIN2, speed);
  analogWrite(MOTOR2_PIN1, speed);
  analogWrite(MOTOR2_PIN2, 0);
}

void stop() {
  analogWrite(MOTOR1_PIN1, 0);
  analogWrite(MOTOR1_PIN2, 0);
  analogWrite(MOTOR2_PIN1, 0);
  analogWrite(MOTOR2_PIN2, 0);
}

void setup() {
  Serial.begin(SERIAL_PORT_SPEED);

  pinMode(RC_CH1_PIN, INPUT);
  pinMode(RC_CH2_PIN, INPUT);
  pinMode(RC_CH3_PIN, INPUT);
  pinMode(RC_CH4_PIN, INPUT);

  enableInterrupt(RC_CH1_PIN, calc_ch1, CHANGE);
  enableInterrupt(RC_CH2_PIN, calc_ch2, CHANGE);
  enableInterrupt(RC_CH3_PIN, calc_ch3, CHANGE);
  enableInterrupt(RC_CH4_PIN, calc_ch4, CHANGE);

      pinMode(MOTOR1_PIN1, OUTPUT);
      pinMode(MOTOR1_PIN2, OUTPUT);
      pinMode(MOTOR2_PIN1, OUTPUT);
      pinMode(MOTOR2_PIN2, OUTPUT);
}

void loop() {
  rc_read_values();

//    Serial.print("CH1:"); Serial.print(rc_values[RC_CH1]); Serial.print("\t");
//    Serial.print("CH2:"); Serial.print(rc_values[RC_CH2]); Serial.print("\t");
//    Serial.print("CH3:"); Serial.print(rc_values[RC_CH3]); Serial.print("\t");
//    Serial.print("CH4:"); Serial.println(rc_values[RC_CH4]);

  delay(200);
  
  // use whichever channel is the trigger to control forward and reverse movement
if (rc_values[RC_CH2] < 1450) reverse(map(rc_values[RC_CH2], 1500, 2100, 255, 0));
if (rc_values[RC_CH2] > 1550) forward(map(rc_values[RC_CH2], 1550, 2100, 0, 180));
if ((rc_values[RC_CH2] < 1545) && (rc_values[RC_CH2] > 1500)) stop();

// use whichever channel is the steering wheel to turn:
 if (rc_values[RC_CH1] > 1490) turnRight(map(rc_values[RC_CH1], 1400, 2100, 0, 180));

 if (rc_values[RC_CH1] < 1390) turnLeft(map(rc_values[RC_CH1], 1390, 2100, 255, 0));
}
