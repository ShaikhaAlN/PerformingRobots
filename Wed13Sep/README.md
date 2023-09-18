# Working on the Dancing Robot - Base
### Sketches
- We wanted to create a robot based on nature to create an oxymoronic combination. Below are the prelimenary sketches we made:
![First Robot Sketches](PRrobot1sketch.JPG)
- the first one was designed after a sunflower and had a hexagon-shaped base. we liked the idea that sunflowers turn to face the sun and thought of mimicking this with a light sensor that would communicate a change of rotation or movement to the motor.
- the second one is modeled after the venus fly trap plant from the super mario games, and we were hoping to also mimick the natural tendency for it to close upon detecting a prey by using a pressure (or light) sensor. This design had a pentagon shape.
- ultimately, we decided to go with the second design as to challenge ourselves with the complex and comical shape. Although we did not get a pentagon-shaped base, we were able to get a big enough rectangular base that would allow us to integrate the tube design and possible surrounding grass elements (to mimick the 2d space the games are set in).
### Base Process


![dancingrobot6](https://github.com/ShaikhaAlN/PerformingRobots/assets/98512587/a919b2f9-c23f-4403-8da4-77714349ca48)
![dancingrobot5](https://github.com/ShaikhaAlN/PerformingRobots/assets/98512587/8fef6ae2-f477-401c-ad21-8197e48c5be8)
![dancingrobot4](https://github.com/ShaikhaAlN/PerformingRobots/assets/98512587/d3cd59b1-6ef5-4bf9-af17-78b7845ca413)
![dancingrobot3](https://github.com/ShaikhaAlN/PerformingRobots/assets/98512587/53be1bd2-a65a-4ecd-9e2f-fa1ffe4be47f)
![dancingrobot2](https://github.com/ShaikhaAlN/PerformingRobots/assets/98512587/a2f36419-4db8-4d9e-83f9-15b65f2cadce)
![dancingrobot1](https://github.com/ShaikhaAlN/PerformingRobots/assets/98512587/d00a1cdc-c394-4267-b4ae-9c7248dc4794)


- After being given the rectangular base, we started sketching out where all the parts would go. we made sure to leave suffiecient space between all the different components.
- to attach the motors to the base, we drilled holes for the motor brackets. instead of 4, we found a way to only drill two holes diagonally so that the motor would not budge.
- we then attached the motor by again screwing bolts between the motor and bracket.
- we then soldered the wires connecting the motors to the arduino so that they would go all the way around.
- we ziptied the H bridge and placed the wheels on the motor by using bolts and metal hubs. we made sure to screw it very tightly so that any movement would be due to the code as opposed to a loosely attached wheel
- after making sure the wheels were both moving according to our code (see below):

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
  delay(500); // let it turn for 5 seconds

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(500);

  // now reverse direction
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(1000);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(1000);

    // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(500);

  // now reverse direction
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
   digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(350);
}
```

- we then ziptied the battery in place, and gluegunned the arduino to the base.


