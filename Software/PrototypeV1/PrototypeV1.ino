// [Printing MPU6050 accelerometer data to CSV using PuTTY :D
// [Based on MPU6050_raw example from this kind gentleman mentioned below, refer to it for in-depth explanation
// [All my comments start with these brackets, Mr. Rowberg's are left as is
// [Use zeroing example to find offsets

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO

#define LED_PIN 13
bool blinkState = false;

// [count up the number of samples
int sampleCount = 1;

// [second between samples in milliseconds
int delayMs = 25;


void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    Serial.begin(38400);

    // initialize device [REMOVED SERIAL]
    // Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection [REMOVED SERIAL]
    // Serial.println("Testing device connections...");
    // Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // use the code below to change accel/gyro offset values

    // [REMOVED SERIAL]
    // Serial.println("Updating internal sensor offsets...");
    
    
    /* [REMOVED SERIAL]
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    */
    // [See Zeroing output photo for test I ran. I don't really trust that I was level enough for accurate
    // [accel data, but the gyro stuff is on point, so I'm using that here. The offsets are so small that
    // [they don't matter too much for my application, where I detect changes, not positions of readings
    accelgyro.setXGyroOffset(-11);
    accelgyro.setYGyroOffset(-46);
    accelgyro.setZGyroOffset(-63);
    /* [REMOVED SERIAL]
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    */

    // [title the columns
    Serial.print("Sample-Count,X-accel,Y-accel,Z-accel,Net-accel,X-gyro,Y-gyro,Z-gyro,Sample delay ");
    Serial.print(delayMs); Serial.println(" ms");
    
    // configure Arduino LED pin for output
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display comma-separated accel/gyro x/y/z values
        // Serial.print("a/g:\t");
        Serial.print(sampleCount);                          Serial.print(",");
        Serial.print(ax);                                   Serial.print(",");
        Serial.print(ay);                                   Serial.print(",");
        Serial.print(az);                                   Serial.print(",");
        Serial.print(sqrt(pow(ax,2)+pow(ay,2)+pow(az,2)));  Serial.print(","); // [Net acceleration
        Serial.print(gx);                                   Serial.print(",");
        Serial.print(gy);                                   Serial.print(",");
        Serial.println(gz);

        sampleCount++;
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif


    //ADD IN LATER--------------------------------
    /*
    #ifdef PRINTTOPLOTTER
      printDataForArduinoPlotter(&Serial);
    #endif
    */
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    // [Pretty sure this changes sample size, so I mess with it
    delay(delayMs);
}
