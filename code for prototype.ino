// Demo code for Grove - Temperature Sensor V1.1/1.2
// Loovee @ 2015-8-26
/* Servo Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.
 
 modified 8 Nov 2013
 by Scott Fitzgerald
 modified 21 Mar 2022
 by Mateusz Weclawowicz
 https://arduino.cc/en/Tutorial/Sweep
*/
#include <Servo.h>
Servo myservo;// create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0; // variable to store the servo position
#include <math.h>



 
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A2;     // Grove - Temperature Sensor connect to A0
 
#if defined(ARDUINO_ARCH_AVR)
#define debug  Serial
#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
#define debug  SerialUSB
#else
#define debug  Serial
#endif

void setup()
{
  
  Serial.begin(9600);
  myservo.attach(5); // attaches the servo on pin 5 to the servo object
    
}
 
void loop()
{
  
  int a = analogRead(pinTempSensor);
 
  float R = 1023.0/a-1.0;
  R = R0*R;

  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet

  Serial.print("temperature = ");
  Serial.println(temperature);
    if(temperature >= 25) //once temperature is above 25 degrees the servo starts spinning
    {
      
      for (pos = 0; pos <= 180; pos += 1) 
      { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      for (pos = 180; pos >= 0; pos -= 1) 
      { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15;
      
      Serial.print("temperature = ");
      Serial.println(temperature);
      delay(100);
    }
  delay(100);
}
