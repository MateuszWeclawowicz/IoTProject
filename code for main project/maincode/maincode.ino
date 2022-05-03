/*
 * cheapStepper_simple.ino
 * ///////////////////////////////////////////
 * using CheapStepper Arduino library v.0.2.0
 * created by Tyler Henry, 7/2016
 * ///////////////////////////////////////////
 * 
 * this sketch illustrates basic step() functionality of the library:
 * the stepper performs a full rotation, pauses 1 second,
 * then does a full rotation in the other direction, and so on
 * 
 * //////////////////////////////////////////////////////
 */

// first, include the library :)

#include <CheapStepper.h>


CheapStepper stepper;
// here we declare our stepper using default pins:
// arduino pin <--> pins on ULN2003 board:
// 8 <--> IN1
// 9 <--> IN2
// 10 <--> IN3
// 11 <--> IN4

 // let's create a boolean variable to save the direction of our rotation

boolean moveClockwise = true;

#include <math.h>



 
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A2;     // Grove - Temperature Sensor connect to A2
 
#if defined(ARDUINO_ARCH_AVR)
#define debug  Serial
#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
#define debug  SerialUSB
#else
#define debug  Serial
#endif

/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLRyVnpDp5"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "tHspTf0KjXGwFjRpC6wA1tMvK-fdJ5zW"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <Bridge.h>
#include <BlynkSimpleYun.h>

char auth[] = "tHspTf0KjXGwFjRpC6wA1tMvK-fdJ5zW";
int phoneButton;
BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  phoneButton = value;
  // Update state
  Blynk.virtualWrite(V1, value);
  
   
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}



void setup()
{
  
  //Debug console
  Serial.begin(115200);
  
  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk.cloud", 80);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  Serial.begin(9600);
  
  
  
    
}
 
void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  int a = analogRead(pinTempSensor);
 
  float R = 1023.0/a-1.0;
  R = R0*R;

  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; // convert to temperature via datasheet
  
  Serial.print("temperature = ");
  Serial.println(temperature);
    if(temperature >= 29 || phoneButton == 1) //once temperature is above 25 degrees the servo starts spinning
    {
      
            // let's move a full rotation (4096 mini-steps)
        // we'll go step-by-step using the step() function
        
        for (int s=0; s<4096; s++){
          // this will loop 4096 times
          // 4096 steps = full rotation using default values
          /* Note:
           * you could alternatively use 4076 steps... 
           * if you think your 28BYJ-48 stepper's internal gear ratio is 63.68395:1 (measured) rather than 64:1 (advertised)
           * for more info, see: http://forum.arduino.cc/index.php?topic=71964.15)
           */
      
          // let's move one "step" (of the 4096 per full rotation)
          
          stepper.step(moveClockwise);
          /* the direction is based on moveClockwise boolean:
           * true for clockwise, false for counter-clockwise
           * -- you could also say stepper.stepCW(); or stepper.stepCCW();
           */
      
          // now let's get the current step position of motor
          
          int nStep = stepper.getStep();
      
          // and if it's divisible by 64...
          
          if (nStep%2048==0){ 
      
            // let's print the position to the console
            
            //Serial.print("current step position: "); Serial.print(nStep);
            //Serial.println();
            temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15;
      
            Serial.print("temperature = ");
            Serial.println(temperature);
          }
        }
      
      
    }
  
}
