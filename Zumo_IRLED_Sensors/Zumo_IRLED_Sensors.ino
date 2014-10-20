#include <QTRSensors.h> 
#include <ZumoReflectanceSensorArray.h>

//Setup the QTR sensor array 
#define NUM_SENSORS 6 
unsigned int sensor_values[NUM_SENSORS]; 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN); 

void setup(){ 
 Serial.begin(9600); 
}

void loop(){ 
//Read and print the sensor value of sensor[3] every 0.5 seconds 
//to the serial monitor. 
 sensors.read(sensor_values); 
 Serial.println(sensor_values[3]); 
 delay(500); 
}
