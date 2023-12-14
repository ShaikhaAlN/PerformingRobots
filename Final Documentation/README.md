# Process
## movement
- After being given the rectangular base, we started sketching out where all the parts would go. we made sure to leave suffiecient space between all the different components.
- to attach the motors to the base, we drilled holes for the motor brackets. instead of 4, we found a way to only drill two holes diagonally so that the motor would not budge.
- we then attached the motor by again screwing bolts between the motor and bracket.
- we then soldered the wires connecting the motors to the arduino so that they would go all the way around.
- we ziptied the H bridge and placed the wheels on the motor by using bolts and metal hubs. we made sure to screw it very tightly so that any movement would be due to the code as opposed to a loosely attached wheel
## script and character
- After settling on the robo-love island concept for the play, we got to thinking on which character would bring the most interesting story to the table.
- We were adamant to have our robot be different in that it does not come from a wealthy or urban family, as to introduce some diversity into the cast.
- after running through a few ideas, we decided on 'Emma-Lou', a southern, country farm-helper robot who's a bit rough around the edges and hardworking.
- We agreed to pair her up with 'Magnus' who's on the complete opposite side of the spectrum when it comes to privelege and class. We thought this dynamic could introduce some interesting drama and thick tension to the play.
- The main parts we focused on when it came to the script was allowing her country side to shine, giving her some goofy lines that contrast heavily with the uptightness of Magnus.
- Words like "partner" and "Folks" keep her endearing to the audience, and the references to country slang such as "June bug" and "Bayou" add some spring to her dialogue. However, we still made sure to keep reminding the audience that shes a robot with references to "short circuiting" and being "programmed".
## character design
- Building off of the country theme, we were inspired by the wizard of Oz to make our robot look like a scarecrow. After looking up several images of typical scarecrows, we came up with the design below:
- (insert image here)
- We felt like the design was feasible for the timeline we had to complete all the mechanics and aesthetics, as many of the outward components consisted of clothes we already had or could make, and costume props we could burrow from the costume or textile studio.
- From this design stemmed our overall plan in terms of the mechanics we were going to implement, and in which order.
#### creating the body
- At first, we felt the need to create a square body out of plywood with a long rectangular stick going through it for the arms, as to match the rectangular base we had (see image below). 
- (insert image here)
- However, we felt as though this did not stick true to our vision of the scarecrow and made it look way too boxy and large compared to the other robots. We also ran the risk of offsetting the balance of the base, which would have caused us a lot of trouble with movement and preventing it from tipping over.
- So instead, we decided to use two square sticks of wood in cross formation to mimick a real life scarecrow.
- To stabelize it, we used right angle steel parts and drilled screws to set them in place. We used three of these for the main body stick; one on each side and one on the back. We made sure it was stable before we moved on to the arms.
- For the arms, we used two screws in the middle to attach it to the main body, which ended up being really sturdy and did not need any further reinforcements.
- Then, we moved on to creating the crow. We wanted the crow to be lightweight and easy to manage or handle, since we had a strong feeling that we would need to pull it a part and have it sit on the arm which would be moving around a lot during the play. As such, we chose to use aluminum foil and light-weight clay.
- We first created the body so that we can test the durability and feasibility of our method. We first created a large ball out of the aluminum, and then slowly started flattening the bottom and lengthening the back to create a base and a tail. After we were satisfied with the shape, we began slowly building up layers of clay to cover over the aluminium. This proved to be a challenging task, as the clay would not stick to the aluminium and would instead slide around. To fix this, we used some water to make it stickier, which worked really well.
- After the body was done, we let it dry overnight and the result was just as we expected. The structure was light yet firm and retained the color and shape. So, we moved on to the head, which was made in two parts out of two hemispheres, and the beak, which was also in two parts. We wanted the head to be hollow and in two halfs so that we had more control over the internal mechanics and to allow for easy disassembling without damaging anything.
- When this was done, we were ready to move onto the main mechanics, which mainly involved the crow on the scarecrows neck, the neopixel eyes, and the scarecrow's head (as described in more detail below).
## mechanics
- Since our robot is supposed to represent a scarecrow, which in reality lacks much movement, we felt that it was appropriate to animate and bring the crow to life instead, serving as both a funny and entertaining gag for the audience, but also as an ironic friend to a scarecrow who is meant to scare off crows.
#### crow beak
- For the crows beak, we used a small servo motor and followed a youtube tutorial on how to create a mouth that opend and closes. We used a paper clip and bent it into an s shape, with one side attaching to the servo motor, and the other inside the upper part of the crow's beak. we kept having to adjust the opening of the head to allow for the crow's beak to move in a very asnimated way that would show clearly from afar for the audience.
#### crow neck
- For the neck, we installed 1 piece of cardboard in the shape of a donut right at the top of the neck in order to contint the servo motor but have the turning piece peak out of the hole.
- we then attached this turning piece to the head using glue gun. We had to position it right in the middle, and we did this by checking the coordinates of the full range of movement, find the mid point, and then glue the head on. 
#### head
- for the robots head, we wanted something lightweight as anything heavier would weigh on the narrow stick and cause imbalance. So, we felt cardboard was a better material to work with than plywood.
- We measured the appropriate dimensions of a rectangular box that would fit perfectly through the stick, and looked proportional to the arms. We then cut out two squares where the neopixel eyes would go, and after we made sure everything fit, we stuck all the sides together with the hot glue gun, and cut out a door from the back so that we could easily access the neopixels. We also kept the hole hole on the bottom of the head wider so that we could install a servo motor at the top that would allow the head to move slightly from left to right. Installing this was difficult as we stuck the servo motor to the stick and then put the cardboard head on top, so we werent able to see how well it lined up on the inside. However, after a few tries (and torn pieces of cardboard) later, we lined it up well and made sure to not move the servo motor dramatically as to not tear the hot glue.
#### neopixels
- before using both neopixels, we first tested out a few designs on one neopixel matrix, experimenting with the shapes at our disposable from the neopixel library installed on arduino. We settled on using the rectangles and sticking to one color (orange) to match the boxy, warm toned aesthetics of our robot. We first created the baseline eyes, then moved on to creating a blinking animation.
- For the animation, we thought displaying frames rapidly would create a smooth transition that would give our robot life. So we started with the eyes wide open, and then slowly lowered the larger rectangle (eyelids) by manipulating its dimensions frame by frame. We also moved the smaller rectangle (the pupil) so that it with appear to be under the eyelid when it closes. We then looped this and made it look natural using trial and error with the delay() function and matrix1.fillScreen(0).
- Then, since we already had an almost finalized version of the play and script, we knew what kind of expressions we wanted to code and use for the interactions and reactions between the characters; eyeroll, sideeye, red eyes, fast blinking, and shocked eyes.
- we coded these the same we we created the initial standard eyes; we manipulated the shapes frame by frame, and in the case of the red eyes, we changed the color of the pupils, and for the eyeroll we changed the position of the pupil and extended the duration of the delay.
- to ensure that the animation would work on both eyes exact, we did not daisy-chain the two matrices, but instead treated them like two seperate units in the code. We did this by labelling them "matrix1" and "matrix2" and just duplicating the code. This was much easier than trying to work with the coordinates of two combined matrices.
#### sound
- At first, we thought of recording the voice of Emma-Lou ourselves, but then decided against it for a number of reasons. We felt that the audio may contain a lot of noise, would be inconcistent in overall volume, and would be difficult to make clear and legible. So, we instead used the AI text to speech software ElevenLabs. We wanted the voice to sound enthusiaistic, so we went with a young adult with an american accent, and fed all the lines into the software with exagerated punctuation marks to accentuate some of the dialogue. After we had the sounds, 
## costume
## issues and troubleshooting
#### Wheel screws and base extensions
#### Sound issues
#### crow beak and neck
- After testing it out, we realized that the paper clip kept moving around, causing the whole upper beak to be misaligned with the  
#### movement speed
