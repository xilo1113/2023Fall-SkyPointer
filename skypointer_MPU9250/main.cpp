/* MPU9250 Basic Example Code
 by: Kris Winer
 date: April 1, 2014
 license: Beerware - Use this code however you'd like. If you
 find it useful you can buy me a beer some time.

 Demonstrate basic MPU-9250 functionality including parameterizing the register
 addresses, initializing the sensor, getting properly scaled accelerometer,
 gyroscope, and magnetometer data out. Added display functions to allow display
 to on breadboard monitor. Addition of 9 DoF sensor fusion using open source
 Madgwick and Mahony filter algorithms. Sketch runs on the 3.3 V 8 MHz Pro Mini
 and the Teensy 3.1.

 SDA and SCL should have external pull-up resistors (to 3.3V).
 10k resistors are on the EMSENSR-9250 breakout board.

 Hardware setup:
 MPU9250 Breakout --------- Arduino
 VDD ---------------------- 3.3V
 VDDI --------------------- 3.3V
 SDA ----------------------- A4
 SCL ----------------------- A5
 GND ---------------------- GND

 Note: The MPU9250 is an I2C sensor and uses the Arduino Wire library.
 Because the sensor is not 5V tolerant, we are using a 3.3 V 8 MHz Pro Mini or
 a 3.3 V Teensy 3.1. We have disabled the internal pull-ups used by the Wire
 library in the Wire.h/twi.c utility file. We are also using the 400 kHz fast
 I2C mode by setting the TWI_FREQ  to 400000L /twi.h utility file.
 */

//#include "ST_F401_84MHZ.h"
// F401_init84 myinit(0);
#include "MPU9250.h"
#include "mbed.h"
#include "stm32l475xx.h"
#include <chrono>
// Using NOKIA 5110 monochrome 84 x 48 pixel display
// pin 9 - Serial clock out (SCLK)
// pin 8 - Serial data out (DIN)
// pin 7 - Data/Command select (D/C)
// pin 5 - LCD chip select (CS)
// pin 6 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(9, 8, 7, 5, 6);

float sum = 0;
uint32_t sumCount = 0;

MPU9250 mpu9250;

Timer t;

void magcalMPU9250(float *dest1) {
  uint16_t ii = 0, sample_count = 0;
  int32_t mag_bias[3] = {0, 0, 0};
  int16_t mag_max[3] = {-32767, -32767, -32767},
          mag_min[3] = {32767, 32767, 32767}, mag_temp[3] = {0, 0, 0};

  printf("Mag Calibration: Wave device in a figure eight until done!\n");
  wait_us(4000000);

  sample_count = 300;
  for (ii = 0; ii < sample_count; ii++) {
    mpu9250.readMagData(mag_temp); // Read the mag data
    for (int jj = 0; jj < 3; jj++) {
      if (mag_temp[jj] > mag_max[jj])
        mag_max[jj] = mag_temp[jj];
      if (mag_temp[jj] < mag_min[jj])
        mag_min[jj] = mag_temp[jj];
    }
    wait_us(135000); // at 8 Hz ODR, new mag data is available every 125 ms

    printf("dcx = %d", mag_temp[0]);
    printf(" dcy = %d",  mag_temp[1]);
    printf(" dcz = %d \n\r", mag_temp[2]);
  }

     printf("mag x min/max: %d----%d\n", mag_max[0],mag_min[0]); 
     printf("mag y min/max: %d----%d\n", mag_max[1],mag_min[1]); 
     printf("mag z min/max: %d----%d\n", mag_max[2],mag_min[2]); 

  mag_bias[0] =
      (mag_max[0] + mag_min[0]) / 2; // get average x mag bias in counts
  mag_bias[1] =
      (mag_max[1] + mag_min[1]) / 2; // get average y mag bias in counts
  mag_bias[2] =
      (mag_max[2] + mag_min[2]) / 2; // get average z mag bias in counts

  dest1[0] = (float)mag_bias[0] * mRes *
             magCalibration[0]; // save mag biases in G for main program
  dest1[1] = (float)mag_bias[1] * mRes * magCalibration[1];
  dest1[2] = (float)mag_bias[2] * mRes * magCalibration[2];

  printf("Mag Calibration done!\n");
}

int main() {
  //   pc.baud(9600);

  // Set up I2C
  //   i2c.frequency(400000); // use fast (400 kHz) I2C

  printf("CPU SystemCoreClock is %d Hz\r\n", SystemCoreClock);

  t.start();

  // Read the WHO_AM_I register, this is a good test of communication
  uint8_t whoami = mpu9250.readByte(
      MPU9250_ADDRESS, WHO_AM_I_MPU9250); // Read WHO_AM_I register for MPU-9250
  printf("I AM 0x%x\n\r", whoami);
  printf("I SHOULD BE 0x71\n\r");

  if (whoami == 0x71) // WHO_AM_I should always be 0x68
  {
    printf("MPU9250 is online...\n\r");
    wait_us(1000000);

    mpu9250.resetMPU9250(); // Reset registers to default in preparation for
                            // device calibration
    mpu9250.calibrateMPU9250(gyroBias,
                             accelBias); // Calibrate gyro and accelerometers,
                                         // load biases in bias registers
    printf("x gyro bias = %f\n\r", gyroBias[0]);
    printf("y gyro bias = %f\n\r", gyroBias[1]);
    printf("z gyro bias = %f\n\r", gyroBias[2]);
    printf("x accel bias = %f\n\r", accelBias[0]);
    printf("y accel bias = %f\n\r", accelBias[1]);
    printf("z accel bias = %f\n\r", accelBias[2]);
    wait_us(2000000);
    mpu9250.initMPU9250();
    printf(
        "MPU9250 initialized for active data mode....\n\r"); // Initialize
                                                             // device for
                                                             // active mode read
                                                             // of acclerometer,
                                                             // gyroscope, and
                                                             // temperature
    mpu9250.initAK8963(magCalibration);
    printf(
        "AK8963 initialized for active data mode....\n\r"); // Initialize device
                                                            // for active mode
                                                            // read of
                                                            // magnetometer
    printf("Accelerometer full-scale range = %f  g\n\r",
           2.0f * (float)(1 << Ascale));
    printf("Gyroscope full-scale range = %f  deg/s\n\r",
           250.0f * (float)(1 << Gscale));
    if (Mscale == 0)
      printf("Magnetometer resolution = 14  bits\n\r");
    if (Mscale == 1)
      printf("Magnetometer resolution = 16  bits\n\r");
    if (Mmode == 2)
      printf("Magnetometer ODR = 8 Hz\n\r");
    if (Mmode == 6)
      printf("Magnetometer ODR = 100 Hz\n\r");
    wait_us(2000000);
  } else {
    printf("Could not connect to MPU9250: \n\r");
    printf("%#x \n", whoami);

    while (1)
      ; // Loop forever if communication doesn't happen
  }

  mpu9250.getAres(); // Get accelerometer sensitivity
  mpu9250.getGres(); // Get gyro sensitivity
  mpu9250.getMres(); // Get magnetometer sensitivity
  printf("Accelerometer sensitivity is %f LSB/g \n\r", 1.0f / aRes);
  printf("Gyroscope sensitivity is %f LSB/deg/s \n\r", 1.0f / gRes);
  //   printf("Magnetometer sensitivity is %f LSB/G \n\r", 1.0f / mRes);
  //   magbias[0] = +470.; // User environmental x-axis correction in
  //   milliGauss,
  //                       // should be automatically calculated
  //   magbias[1] = +120.; // User environmental x-axis correction in milliGauss
  //   magbias[2] = +125.; // User environmental x-axis correction in milliGauss
  magcalMPU9250(magbias);
  printf("x magbias = %f\n\r", magbias[0]);
  printf("y magbias = %f\n\r", magbias[1]);
  printf("z magbias = %f\n\r", magbias[2]);
  uint64_t count =
      chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
  int16_t magCount[3] = {0, 0, 0};
  while (1) {

    // If intPin goes high, all data registers have new data
    if (mpu9250.readByte(MPU9250_ADDRESS, INT_STATUS) &
        0x01) { // On interrupt, check if data ready interrupt

      mpu9250.readAccelData(accelCount); // Read the x/y/z adc values
      // Now we'll calculate the accleration value into actual g's
      ax = (float)accelCount[0] * aRes -
           accelBias[0]; // get actual g value, this depends on scale being set
      ay = (float)accelCount[1] * aRes - accelBias[1];
      az = (float)accelCount[2] * aRes - accelBias[2];

      mpu9250.readGyroData(gyroCount); // Read the x/y/z adc values
      // Calculate the gyro value into actual degrees per second
      gx =
          (float)gyroCount[0] * gRes -
          gyroBias[0]; // get actual gyro value, this depends on scale being set
      gy = (float)gyroCount[1] * gRes - gyroBias[1];
      gz = (float)gyroCount[2] * gRes - gyroBias[2];

      mpu9250.readMagData(magCount); // Read the x/y/z adc values
      // Calculate the magnetometer values in milliGauss
      // Include factory calibration per data sheet and user environmental
      // corrections

    //   printf("dx = %d", magCount[0]);
    //   printf(" dy = %d",  magCount[1]);
    //   printf(" dz = %d \n\r", magCount[2]);
      mx = (float)magCount[0] * mRes * magCalibration[0] -
           magbias[0]; // get actual magnetometer value, this depends on scale
                       // being set
      my = (float)magCount[1] * mRes * magCalibration[1] - magbias[1];
      mz = (float)magCount[2] * mRes * magCalibration[2] - magbias[2];
    }

    Now = chrono::duration_cast<chrono::microseconds>(t.elapsed_time()).count();
    deltat = (float)((Now - lastUpdate) /
                     1000000.0f); // set integration time by time elapsed since
                                  // last filter update
    lastUpdate = Now;

    sum += deltat;
    sumCount++;

    //    if(lastUpdate - firstUpdate > 10000000.0f) {
    //     beta = 0.04;  // decrease filter gain after stabilized
    //     zeta = 0.015; // increasey bias drift gain after stabilized
    //   }

    // Pass gyro rate as rad/s
    mpu9250.MadgwickQuaternionUpdate(ax, ay, az, gx * PI / 180.0f,
                                     gy * PI / 180.0f, gz * PI / 180.0f, my, mx,
                                     mz);
    // mpu9250.MahonyQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f,
    // gz*PI/180.0f, my, mx, mz);

    // Serial print and/or display at 0.5 s rate independent of data rates
    delt_t =
        chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count() -
        count;
    if (delt_t >
        2000) { // update LCD once per half-second independent of read rate

      //   printf("ax = %f", 1000 * ax);
      //   printf(" ay = %f", 1000 * ay);
      //   printf(" az = %f  mg\n\r", 1000 * az);

      //   printf("gx = %f", gx);
      //   printf(" gy = %f", gy);
      //   printf(" gz = %f  deg/s\n\r", gz);

      printf("mx = %f", mx);
      printf(" my = %f", my);
      printf(" mz = %f  mG\n\r", mz);


      tempCount = mpu9250.readTempData(); // Read the adc values
      temperature = ((float)tempCount) / 333.87f +
                    21.0f; // Temperature in degrees Centigrade
      printf(" temperature = %f  C\n\r", temperature);

      //   printf("q0 = %f\n\r", q[0]);
      //   printf("q1 = %f\n\r", q[1]);
      //   printf("q2 = %f\n\r", q[2]);
      //   printf("q3 = %f\n\r", q[3]);

      // Define output variables from updated quaternion---these are Tait-Bryan
      // angles, commonly used in aircraft orientation. In this coordinate
      // system, the positive z-axis is down toward Earth. Yaw is the angle
      // between Sensor x-axis and Earth magnetic North (or true North if
      // corrected for local declination, looking down on the sensor positive
      // yaw is counterclockwise. Pitch is angle between sensor x-axis and Earth
      // ground plane, toward the Earth is positive, up toward the sky is
      // negative. Roll is angle between sensor y-axis and Earth ground plane,
      // y-axis up is positive roll. These arise from the definition of the
      // homogeneous rotation matrix constructed from quaternions. Tait-Bryan
      // angles as well as Euler angles are non-commutative; that is, the get
      // the correct orientation the rotations must be applied in the correct
      // order which for this configuration is yaw, pitch, and then roll. For
      // more see
      // http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
      // which has additional links.
      yaw = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]),
                  q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
      pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
      roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]),
                   q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
      pitch *= 180.0f / PI;
      yaw *= 180.0f / PI;
      yaw += 5.033f; // Declination at Danville, California is 13 degrees 48
                     // minutes and 47 seconds on 2014-04-04
      roll *= 180.0f / PI;

      printf("Yaw, Pitch, Roll: %f %f %f\n\r", yaw, pitch, roll);
      //   printf("average rate = %f\n\r", (float)sumCount / sum);

      myled = !myled;
      count =
          chrono::duration_cast<chrono::milliseconds>(t.elapsed_time()).count();
      sum = 0;
      sumCount = 0;
    }
  }
}
