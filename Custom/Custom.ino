//  Custom by Zachary

#include <ZumoMotors.h>
#include <Pushbutton.h>

Pushbutton button(ZUMO_BUTTON);
ZumoMotors motors;

void setup()
{
  
}

void loop()
{
  
  button.waitForButton();  // wait for button push to start
  
  turn45(3);
  delay(250);
  turn45(-3);
  motors.setSpeeds(0,0);   //stop robot at the end
  
}

void turn45(int turnCount){
  
  motors.setSpeeds(0,0);
  if(turnCount > 0){
    motors.setSpeeds(90,-90);
    delay(500*turnCount);
  }else if(turnCount < 0){
    motors.setSpeeds(90,-90);
    delay(-500*turnCount);
  }
  motors.setSpeeds(0,0);
}
