#include <Wire.h>
#include "MAX30105.h"


MAX30105 particleSensor;

bool MODE_GREEN_ONLY = true;
//Analag input GSR
const int GSR = A16;
int sensorValue = 0;
int average_gsr = 0;
int human_resistance = 0;
float human_conductance = 0.0;

void setup()
{
  
  Serial.begin(115200);

  //PPG
  //Initialize sensor
  if (particleSensor.begin() == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  //particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
  particleSensor.setup(0xFF,4,3,800,411,4096); //sample with 3200 Hz
  /*
   Power level: 0xFF    powerLevel = 0xFF, 50.0mA - Presence detection of ~12 inch
   Sample Agerage: 4    The chip will average multiple samples of same type together if you wish
   LED Mode: 3          Watch all three LED channels (no toption for green only
   Sample Rate: 800     Standard: 50
   Pulse Width: 411     18 bit resolution
   ADC Range: 4096      15.63pA per LSB (Standard: 2048, 7.81pA per LSB
   */

  //GSR
  pinMode(GSR, INPUT);
}

void loop()
{
  //GSR
  long sum_gsr = 0;
  for(int i=0; i<10; i++)           //Average the 10 measurements to remove the glitch
      {
      sensorValue = analogRead(GSR);
      sum_gsr += sensorValue;
      //delay(5);
      }
   average_gsr = sum_gsr/10;
   human_resistance = ((1024 + 2 * average_gsr) * 10000) / (512 - average_gsr);
   human_conductance = (1 / (float)human_resistance) * 1000000.0;

  //Print to PC
  //PPG
  Serial.print("G");
  Serial.print(particleSensor.getGreen());
  if(!MODE_GREEN_ONLY){
    Serial.print("R");
    Serial.print(particleSensor.getRed());
    Serial.print("I");
    Serial.print(particleSensor.getIR());
  }
  //GSR
  Serial.print("O");
  Serial.print(human_resistance);
  Serial.print("\n");
}
