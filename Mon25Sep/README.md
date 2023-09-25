# "The Dance of a Shy Robot"

Have you ever wondered what dance truly is? Dance can encompass a wide range of movements, from graceful footwork to rhythmic sways of the body. But imagine having to execute these smooth rhythms with a rigid body. Our robot sets out to demonstrate the power of conveying emotions through precise, yet initially hesitant, dance moves and joyful spins.

At the outset, our robot is marked by timidity, doubting its own movements and capabilities. It pauses frequently, carefully reconsidering each step before proceeding. But as the music envelops it, something remarkable occurs. The robot begins to sway, gradually gaining confidence with each graceful movement. It dares to venture into more intricate turns, pushing its mechanical boundaries further.

By the performance's grand finale, the robot has transformed. Its once-shy demeanor has given way to an ecstatic display of emotion. In a whirlwind of twirls and spins, the robot conveys a profound sense of excitement and happiness. The robot proves that even in the world of rigid mechanics, there exists a capacity for the most human of expressions – the joy of dance.

```
void setup() {
  // Pins 2 and 3 are connected to In1 and In2 respectively
  // of the L298 motor driver
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

}

void loop() {
    // make the motor turn in one direction
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(50); // let it turn for 5 seconds

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1500);

    // make the motor turn in one direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(50); // let it turn for 5 seconds

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1500);

    // make the motor turn in one direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(100); // let it turn for 5 seconds

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1500);
      
  // make the motor turn in one direction
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(100); // let it turn for 5 seconds

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(500);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(750);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(200);

    // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
    delay(1000);

    // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(500);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
   digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(700);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
    delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(500);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(100);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
    delay(1000);

    // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(500);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
   digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(700);

   // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
   digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(1000);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(1800);

        // now reverse direction
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
   digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(1300);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(250);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
   digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(1300);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(250);

  // now reverse direction
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
   digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(1300);

  // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(250);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
   digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(5000);

    // stop
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
      delay(5000);
}
```
