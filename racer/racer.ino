/* include libraries */
#include <ZumoBuzzer.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>


/* define constants */
#define QTR_THRESHOLD     1000    /* IR THRESHOLD */
#define FORWARD_SPEED     270
#define TURN_SPEED        210
#define TURN_DELAY        38
#define GAP_CHECK         180
#define NUM_SENSORS       6


/* assign mechanical variables */
ZumoMotors motors;
ZumoBuzzer buzzer;
Pushbutton button(ZUMO_BUTTON);
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);


/* assign digital variables */
unsigned int sensor_values[NUM_SENSORS];


/* initialize functions */
void turnFromLine(int);


/* wait for button to begin */
void setup(){
  button.waitForButton();
}


/* main loop */
void loop()
{
  sensors.read(sensor_values);
  
  if(sensor_values[0] > QTR_THRESHOLD){ //left
    
    turnFromLine(0);
    buzzer.playNote(NOTE_C(4),5,15);
    
  }else if(sensor_values[5] > QTR_THRESHOLD){ //right
    
    turnFromLine(5);
    buzzer.playNote(NOTE_C(5),5,15);
  }
  
  motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
  delay(1);
}


void turnFromLine(int sensor){
  if(sensor<4){
    motors.setSpeeds(TURN_SPEED,-TURN_SPEED);
  }else{
    motors.setSpeeds(-TURN_SPEED,TURN_SPEED);
  }
  delay(TURN_DELAY);
}
