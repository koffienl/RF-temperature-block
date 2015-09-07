/*
 Replacement of my glowing egg http://www.maredana.nl/home-automation/woonkamer-cv-raspberry/
 Source:  https://github.com/incmve/RF-temperature-block
 3D printer model: http://www.thingiverse.com/thing:988027 
 */


//Includes
#include "LedControlMS.h"
#include <OneWire.h> // http://www.pjrc.com/teensy/arduino_libraries/OneWire.zip
#include <DallasTemperature.h> // http://download.milesburton.com/Arduino/MaximTemperature/DallasTemperature_LATEST.zip
#include <NewRemoteReceiver.h>

//Define pins
#define NBR_MTX 2 // number of 8x8 displays
#define ONE_WIRE_BUS 8 // DS18B20 PIN
#define ledPinR 5    // Red LED connected to digital pin 5
#define ledPinB 9    // Blue LED connected to digital pin 9
#define ledPinG 6    // Green LED connected to digital pin 6
LedControl lc=LedControl(12,11,10, NBR_MTX); // DataIn,CLK,LOAD, number of displays

// Variables
int NIGHT = 0;      // 0 daytime, 1 Night mode
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature
DeviceAddress insideThermometer;

// 1 = enable debug, 0 = disable debug
boolean debug = 1;

void setup() {
    NewRemoteReceiver::init(0, 2, egg); // RF receiver on pin 2
  Serial.begin (9600);
  Serial.println("Setup");
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.shutdown(1,false);// turn off power saving, enables display
  lc.setIntensity(0,8);// sets brightness (0~15 possible values)
  lc.setIntensity(1,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen just in case
  lc.clearDisplay(1);// clear screen just in case
  
  
    //start up temp sensor
    sensors.begin();   
    sensors.getAddress(insideThermometer, 0);
    int reso = sensors.getResolution(insideThermometer);
    if (reso != 12) {
      if (debug) {
        Serial.print("Resolution of DS18B20 is not 12 but ");
        Serial.print(reso);
        Serial.print(" changing to 12\n");
      }
      sensors.setResolution(insideThermometer, 12);
      if (debug) {
        Serial.print("Done\n");
      }
    }    
  
  
  
  }


void loop() { 
  
  if (NIGHT == 0) {
    lc.shutdown(0,false);
    lc.shutdown(1,false);

                // Read DS18B20 and transmit value as sensor 1
                 float temperaturefloat;
                 sensors.begin(); //start up temp sensor
                 sensors.requestTemperatures(); // Get the temperature
                 temperaturefloat = sensors.getTempCByIndex(0); // Get temperature in Celcius

                 
                int temperature = temperaturefloat;
                int   ones = (temperature%10); // extract ones from temperature
                int tens = ((temperature/10)%10); // extract tens from temperature
                  if (debug) {
                              Serial.print("Original Temp: ");
                              Serial.println(temperaturefloat);
                              Serial.print("Temp: ");
                              Serial.println(temperature);
                              Serial.print("Ones: ");
                              Serial.println(ones);
                              Serial.print("Tens: ");
                              Serial.println(tens);
                             }
                int temp1 = tens;
                int temp2 = ones;
                lc.displayChar(0 , temp1);
                lc.displayChar(1 , temp2);
                delay(10000);
                // lc.clearAll();
              
  }
  if (debug) {
              Serial.print("Nightmode: ");
              Serial.println(NIGHT);
              }
}
void egg(NewRemoteCode receivedCode) {  
      if (debug) {
      // debug RF
     Serial.print("Addr ");
     Serial.print(receivedCode.address);
      if (receivedCode.groupBit) {
        Serial.print(" group");
      } else {
        Serial.print(" unit ");
        Serial.print(receivedCode.unit);
      }
      
      switch (receivedCode.switchType) {
        case 0:
          Serial.print(" off");
          break;
        case 1:
          Serial.print(" on");
          break;
        case 2:
          Serial.print(" dim level");
          Serial.print(receivedCode.dimLevel);
          break;
                  }
                }
            
  if (receivedCode.address == 66 && receivedCode.unit == 6 && receivedCode.switchType == 1) // Unit 66 ID 6 On signal
          {  
          NIGHT = 0;
          analogWrite(ledPinR, 0); 
          analogWrite(ledPinG, 0);
          analogWrite(ledPinB, 0);
          for(int i = 0; i < 3; i++)
            {
          delay(500);
            // fade in from min to max in increments of 1 points:
          for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
            // sets the value (range from 0 to 255):
            analogWrite(ledPinR, fadeValue);         
            // wait for 500 milliseconds to see the dimming effect    
            delay(500);                            
              } 
        
          // fade out from max to min in increments of 1 points:
          for(int fadeValue = 255 ; fadeValue >= 15; fadeValue -=1) { 
            // sets the value (range from 0 to 255):
            analogWrite(ledPinR, fadeValue);         
            // wait for 500 milliseconds to see the dimming effect    
            delay(500);     
              }
            }  
          }
  
  if (receivedCode.address == 66 && receivedCode.unit == 6 && receivedCode.switchType == 0) // Unit 66 ID 6 Off signal
          {
        NIGHT = 0;
        analogWrite(ledPinR, 0); 
        analogWrite(ledPinG, 0);
        analogWrite(ledPinB, 0);
        for(int i = 0; i < 3; i++)
          {
        delay(500);
          // fade in from min to max in increments of 1 points:
        for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
          // sets the value (range from 0 to 255):
          analogWrite(ledPinB, fadeValue);         
          // wait for 500 milliseconds to see the dimming effect    
          delay(500);                            
        } 
      
        // fade out from max to min in increments of 1 points:
        for(int fadeValue = 255 ; fadeValue >= 15; fadeValue -=1) { 
          // sets the value (range from 0 to 255):
          analogWrite(ledPinB, fadeValue);         
          // wait for 500 milliseconds to see the dimming effect    
          delay(500);     
            }
          }  
        }
  if (receivedCode.address == 66 && receivedCode.unit == 7 && receivedCode.switchType == 0) // Unit 66 ID 7 Off signal
          {
        analogWrite(ledPinR, 0); 
        analogWrite(ledPinG, 0);
        analogWrite(ledPinB, 0);
          {
        BlinkR(ledPinR,10);
        delay(50);
          }
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 7 && receivedCode.switchType == 1) // Unit 66 ID 7 On signal
          {
        analogWrite(ledPinR, 0); 
        analogWrite(ledPinG, 0);
        analogWrite(ledPinB, 0);
          {
        BlinkRB(ledPinR,10);
        delay(50);
          }
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 8 && receivedCode.switchType == 0) // Unit 66 ID 8 Off signal
        {
        analogWrite(ledPinR, 0); 
        analogWrite(ledPinG, 0);
        analogWrite(ledPinB, 0);
        // Glow Pink
        analogWrite(ledPinR, 255); 
        analogWrite(ledPinG, 51);
        analogWrite(ledPinB, 153); 
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 8 && receivedCode.switchType == 1) // Unit 66 ID 8 On signal
        {
        analogWrite(ledPinR, 0); 
        analogWrite(ledPinG, 0);
        analogWrite(ledPinB, 0);
        // Glow Green
        analogWrite(ledPinG, 255);
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 9 && receivedCode.switchType == 0) // Unit 66 ID 9 Off signal
        {
        analogWrite(ledPinR, 0); 
        analogWrite(ledPinG, 0);
        analogWrite(ledPinB, 0);
        NIGHT = 1; // LED display off
        if (debug)  {
                    Serial.print("Nightmode: ");
                    Serial.println(NIGHT);
                    }
        lc.shutdown(0,true);
        lc.shutdown(1,true);
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 9 && receivedCode.switchType == 1) // Unit 66 ID 9 On signal
        {
        analogWrite(ledPinR, 0); 
        analogWrite(ledPinG,40);
        analogWrite(ledPinB, 0);
        }


//Blink routine
}
void BlinkR(int led, int times) //Unit 66 ID 7 On Blink
{
 for (int i=0; i< times; i++)
 {
  digitalWrite(ledPinR,HIGH);
  delay (10000);
  digitalWrite(ledPinR,LOW);
  delay (10000);
 }
}
void BlinkRB(int led, int times) //Unit 66 ID 7 Off Blink
{
 for (int i=0; i< times; i++)
 {
  digitalWrite(ledPinB,HIGH);
  delay (10000);
  digitalWrite(ledPinR,HIGH);
  delay (10000);
  digitalWrite(ledPinB,LOW);
  delay (10000);
  digitalWrite(ledPinR,LOW);
  delay (10000);
 }
}
