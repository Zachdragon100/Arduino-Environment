/* include libraries */
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>


/* define constants */
#define QTR_THRESHOLD     1000    /* IR THRESHOLD */
#define REVERSE_SPEED     -200
#define FORWARD_SPEED     200
#define REV_DELAY         300
#define TURN_DELAY        450
#define GAP_CHECK         180
#define NUM_SENSORS       6


/* assign mechanical variables */
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);


/* assign digital variables */
unsigned int sensor_values[NUM_SENSORS];


/* initialize functions */
void halt();
void turnAround(int);
void reactToChange(int);


/* wait for button to begin */
void setup()
{button.waitForButton();}


/* main loop */
void loop()
{
  
    sensors.read(sensor_values);
    
    if(sensor_values[0] > QTR_THRESHOLD){ //left
      
      delay(GAP_CHECK);
      reactToChange(0);
      
      
    }else if(sensor_values[5] > QTR_THRESHOLD){ //right
      
      delay(GAP_CHECK);
      reactToChange(5);
      
    }else if(sensor_values[1] > QTR_THRESHOLD || sensor_values[2] > QTR_THRESHOLD || sensor_values[3] > QTR_THRESHOLD || sensor_values[4] > QTR_THRESHOLD){
      delay(100);
      halt();
    }
  
  motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
  delay(25);
  
  
}



/* stop and wait for the button */
void halt(){
  motors.setSpeeds(0,0);
  button.waitForButton();
}



/* back up and spin */
void turnAround(int sensor){
  motors.setSpeeds(REVERSE_SPEED,REVERSE_SPEED);
  delay(REV_DELAY);
  if(sensor<4){
    motors.setSpeeds(FORWARD_SPEED,-FORWARD_SPEED);
  }else{
    motors.setSpeeds(-FORWARD_SPEED,FORWARD_SPEED);
  }
  delay(TURN_DELAY);
}



/* if line changes, turn around, otherwise, halt */
void reactToChange(int sensor){
  sensors.read(sensor_values);
  if(sensor_values[sensor] < QTR_THRESHOLD){
    turnAround(sensor);  
  }else{
    halt();
  }
}
