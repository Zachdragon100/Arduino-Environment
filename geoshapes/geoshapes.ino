/* include libraries */
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <LSM303.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>


/* define constants */
#define CAL_SPEED 200 // Maximum motor speed when going straight; variable speed when turning
#define CALIBRATION_SAMPLES 100  // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C // CRA_REG_M value for magnetometer 220 Hz update rate
#define DEVIATION_THRESHOLD 10.0// Allowed deviation (in degrees) relative to target angle that must be achieved before driving straight
#define SENSOR_THRESHOLD 100 //140?
#define REVERSE_SPEED -150
#define TURN_SPEED 100
#define FORWARD_SPEED 150
#define REVERSE_DURATION 100
#define TURN_DURATION 100
#define NUM_SENSORS 6


/* assign mechanical variables */
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;


/* assign digital variables */
unsigned int sensor_values[NUM_SENSORS];
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);


/* initialize functions */
void calibrate();
float heading();
void rightTurn(float,int);
void leftTurn(float,int);
float relativeHeading(float,float);
float avgHeading(int);


/* set up function */
void setup()
{
  // Initiate Serial Communication
  Serial.begin(9600);

  // Initiate the Wire library and join the I2C bus as a master
  Wire.begin();  
  
  //Calibrate the compass
  calibrate();
}

//#################################################################################################
//  DO NOT MAKE CHANGES ABOVE THIS LINE
//#################################################################################################


/* main loop */
void loop()
{
 Wait for button push to start robot.
  button.waitForButton(); 
  
  int nsides = 4;
  int MAXSIDECOUNT = 8;
  int MINSIDECOUNT = 3;
  
  float turn = 360/nsides;
  
  for (int j=MAXSIDECOUNT; j>=MINSIDECOUNT; j--)
  {
    int nsides = j;
    float turn = (360/nsides);
    
    for (int i=0; i<nsides; i++)
    {
      motors.setSpeeds(250,250);
      delay(450);
      motors.setSpeeds(100,100);
      delay(50);
      motors.setSpeeds(0,0);
      rightTurn(turn,100);
    }
    
    delay(500);
  }
  
  
  
}

//#################################################################################################
//  DO NOT MAKE CHANGES BELOW THIS LINE
//#################################################################################################


/* calibrate the compass */
void calibrate()
{
  // Initiate LSM303
  compass.init();

  // Enables accelerometer and magnetometer
  compass.enableDefault();
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);    // 220 Hz compass update rate

  button.waitForButton();

  // To calibrate the magnetometer, the Zumo spins to find the max/min
  // magnetic vectors. This information is used to correct for offsets
    int cal_min_x = 32767;
    int cal_max_x = -32767;
    int cal_min_y = 32767;
    int cal_max_y = -32767;
  
  // in the magnetometer data.
  motors.setLeftSpeed(CAL_SPEED);
  motors.setRightSpeed(-CAL_SPEED);

  for(int index = 0; index < CALIBRATION_SAMPLES; index ++)
  {
    // Take a reading of the magnetic vector and store it in compass.m
    compass.read();
 
    cal_min_x = min(cal_min_x, compass.m.x);
    cal_max_x = max(cal_max_x, compass.m.x);
    cal_min_y = min(cal_min_y, compass.m.y);
    cal_max_y = max(cal_max_y, compass.m.y);
    delay(25);
  }

  motors.setLeftSpeed(0);
  motors.setRightSpeed(0);
     
// Set calibrated values to compass.m_max and compass.m_min
  compass.m_min.x = cal_min_x;
  compass.m_max.x = cal_max_x;
  compass.m_min.y = cal_min_y;
  compass.m_max.y = cal_max_y;

} 

//#################################################################################################


/* current direction */
float heading()
{
  // Returns the current heading of the robot in degrees.
  // Compass must first be calibrated
  
  compass.read();
  float x_scaled =  2.0*(float)(compass.m.x - compass.m_min.x) / ( compass.m_max.x - compass.m_min.x) - 1.0;
  float y_scaled =  2.0*(float)(compass.m.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

  float angle = atan2(y_scaled, x_scaled)*180 / M_PI;
  if (angle < 0)
    angle += 360;
  return angle;
}
//#################################################################################################


/* heading relative to goal direction */
float relativeHeading(float heading_from, float heading_to)
{
  float relative_heading = heading_to - heading_from;

  // constrain to -180 to 180 degree range
  if (relative_heading > 180)
    relative_heading -= 360;
  if (relative_heading < -180)
    relative_heading += 360;

  return relative_heading;
}


//#################################################################################################


/* turn right */
void rightTurn(float turn_angle,int turn_speed)
{
  float init_heading = heading();
  float delta = turn_angle;
 
  while(delta>20)
  {
   motors.setSpeeds(turn_speed,-turn_speed);
   delay(4);
   delta = relativeHeading(heading(), fmod(init_heading+turn_angle, 360))*1.0;
   }


  while(delta>5)
  {
   motors.setSpeeds(100,-100);
   delay(4);
   delta = relativeHeading(heading(), fmod(init_heading+turn_angle, 360))*1.0;
   }
  
  motors.setSpeeds(0,0);
}  


//#################################################################################################
  float avgHeading(int j)
 {
   float gohead=0;
   for (int i = 0; i<j; i++)
   {
   gohead+=heading();
   }
   gohead=gohead/j;
   return gohead;
  }
//#################################################################################################
