// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
   // set LED
#include "EmonLibmcpS.h"             // Include Emon Library
#include <MCP3008s.h>
#define CS_PIN 15         // edit these values in the emonlib.cpp
#define CLOCK_PIN 14
#define MOSI_PIN 13
#define MISO_PIN 12
EnergyMonitor emon1;             // Create an instance

void setup()
{ 

  Serial.begin(115200);
 
  emon1.voltage(0, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 111.1);       // Current: input pin, calibration.
}

void loop()
{
  emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
  
  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;             //extract Irms into Variable
}
