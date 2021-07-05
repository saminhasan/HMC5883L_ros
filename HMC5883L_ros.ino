
#define USE_USBCON
#define LED_PIN 13

#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/MagneticField.h>

#include "Wire.h"
#include "I2Cdev.h"
#include "HMC5883L.h"


ros::NodeHandle  n;
sensor_msgs::MagneticField mag;
ros::Publisher pub_mag("/hmc5883l/mag", &mag);
char mag_link[] = "mag_link";


HMC5883L magnetometer;
int16_t mx, my, mz;
bool blinkState = false;



void setup()
{
  n.getHardware()->setBaud(115200);
  n.initNode();
  n.advertise(pub_mag);


  Wire.begin();


  n.loginfo("Initializing I2C devices...");
  magnetometer.initialize();
  n.loginfo("Testing device connections...");
  if (magnetometer.testConnection())
    n.loginfo("HMC5883L connection successful");
  else
    n.logerror("HMC5883L connection failed");
  pinMode(LED_PIN, OUTPUT);

}

void loop()
{

  magnetometer.getHeading(&mx, &my, &mz);


  mag.header.stamp = n.now();

  mag.header.frame_id = mag_link;

  mag.magnetic_field.x = mx / 10.0;
  mag.magnetic_field.y = my / 10.0;
  mag.magnetic_field.z = mz / 10.0;

  mag.magnetic_field_covariance[0] = 0.0;
  mag.magnetic_field_covariance[1] = 0.0;
  mag.magnetic_field_covariance[2] = 0.0;
  mag.magnetic_field_covariance[3] = 0.0;
  mag.magnetic_field_covariance[4] = 0.0;
  mag.magnetic_field_covariance[5] = 0.0;
  mag.magnetic_field_covariance[6] = 0.0;
  mag.magnetic_field_covariance[7] = 0.0;
  mag.magnetic_field_covariance[8] = 0.0;

  pub_mag.publish(&mag);

  n.spinOnce();


}
