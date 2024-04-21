/****************************************************************
 * Example1_Basics.ino
 * ICM 20948 Arduino Library Demo
 * Use the default configuration to stream 9-axis IMU data
 * Owen Lyke @ SparkFun Electronics
 * Original Creation Date: April 17 2019
 *
 * Please see License.md for the license information.
 *
 * Distributed as-is; no warranty is given.
 ***************************************************************/
#include "ICM_20948.h" // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
#include <math.h>

//#define USE_SPI       // Uncomment this to use SPI

#define SERIAL_PORT Serial

#define SPI_PORT SPI // Your desired SPI port.       Used only when "USE_SPI" is defined
#define CS_PIN 2     // Which pin you connect CS to. Used only when "USE_SPI" is defined

#define WIRE_PORT Wire // Your desired Wire port.      Used when "USE_SPI" is not defined
// The value of the last bit of the I2C address.
// On the SparkFun 9DoF IMU breakout the default is 1, and when the ADR jumper is closed the value becomes 0
#define AD0_VAL 1

#ifdef USE_SPI
ICM_20948_SPI myICM; // If using SPI create an ICM_20948_SPI object
#else
ICM_20948_I2C myICM; // Otherwise create an ICM_20948_I2C object
#endif

TaskHandle_t taskHandle;

double radiansToDegrees(double radians){
    return radians * 180/PI;
}

double angle = 0;
double k = .04;
volatile double gyroAngle = 0;

const int32_t DT = 10;
const int32_t T = 5;

//Moving average
volatile double circular_array[1000/DT * T];

volatile double accAngle = 0;

volatile double sum = 0;
int32_t i = 0;

void getAngles(ICM_20948_I2C *sensor)
{
    sensor->getAGMT();

    double acc_z = sensor->accZ();
    double acc_y = sensor->accY();

    if (acc_y != 0){
        accAngle = atan(acc_z/acc_y);
    }else{
      accAngle = 0;
    }

    //log_i("Acc Angle: %f", radiansToDegrees(accAngle));

    gyroAngle = angle - (DT/1000.0) * sensor->gyrX();

    //log_i("Gyro: %f", (DT/1000.0)*sensor->gyrX());
    //log_i("Gyro Angle: %f", gyroAngle);

    angle = k * (-radiansToDegrees(accAngle)) + (1-k)*gyroAngle;
    sum -= circular_array[i];
    sum += angle;
    circular_array[i] = angle;

    i++;
    if (i >= 1000.0/DT * T){
      i = 0;
    };

    //log_i("Angle: %f", angle);
}


void task(void *arg){
  TickType_t xTimeIncrement = configTICK_RATE_HZ * (DT/1000.0);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while (true)
  {
    getAngles(&myICM);
    vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
  }
}

void setup()
{
  SERIAL_PORT.begin(115200);
  while (!SERIAL_PORT)
  {
  };

  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);

  bool initialized = false;
  while (!initialized)
  {
    myICM.begin(WIRE_PORT, AD0_VAL);

    SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    SERIAL_PORT.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      SERIAL_PORT.println("Trying again...");
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }

  //Start loop
  xTaskCreatePinnedToCore(
      task,
      "UpdateSync_loop",
      10000, /* Stack size in words */
      NULL,  /* Task input parameter */
      1,     /* Priority of the task from 0 to 25, higher number = higher priority */
      &taskHandle,
      0); /* Core where the task should run */
}


void loop()
{
  //Print mean
  log_i("Mean: %f", sum/(1000/DT*T));

  double mean = sum/(1000.0/DT*T);
  double varians = 0;
  for (i = 0; i < 1000/DT*T; i++) {
    varians += (circular_array[i] - mean) * (circular_array[i] - mean);
  }

  varians = varians / (1000/DT*T);
  log_i("Varians: %f", varians);

  /*
    Jeg har valgt at systemet virker i de øverste 180 grader. Den har en ændringsperiode ved ændring af orentiation fra opad til nedad og omvendt.
    Dette er pga at accererometeret har orientationerne 
  */

  log_i("Acc Angle: %f", radiansToDegrees(accAngle));
  log_i("Gyro Angle: %f", gyroAngle);

  log_i("Angle: %f", angle);

  delay(1500);
}