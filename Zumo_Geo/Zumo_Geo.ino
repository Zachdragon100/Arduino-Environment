 // 
//  Zumo Robot Move Code
//  August 15, 2014
//
//  Custom by Zachary

#include <ZumoMotors.h>
#include <Pushbutton.h>

Pushbutton button(ZUMO_BUTTON);

ZumoMotors motors;



void setup()
{
  // uncomment one or both of the following lines if your motors' directions need to be flipped
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);
}

void loop()
{
  int i = 4;
  
  button.waitForButton();  // wait for button push to start
  
  for (int count = 0; count < i; count++)  // This is a loop that repeats code
  {
    motors.setSpeeds(0,0);  // maixmum motor speed at speed = +- 400
    delay(250); // time is in milliseconds
    motors.setLeftSpeed(100);
    motors.setRightSpeed(100);
    delay(500);
  }
 
 motors.setSpeeds(0,0);   //stop robot at the end
 
}
