// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3
  // set LED
#include "EmonLibmcpS.h"                   // Include Emon Library
#include <MCP3008s.h>
#define CS_PIN 15   // edit these values in the emonlib.cpp 
#define CLOCK_PIN 14
#define MOSI_PIN 13
#define MISO_PIN 12
EnergyMonitor emon1;                   // Create an instance

void setup()
{ 
 
  Serial.begin(115200);

 
 

  emon1.current(1, 111.1);             // Current: input pin, calibration.
}

void loop()                    /// remove the D  from calcIrmsD to use ardunino analogs
{
  double Irms = emon1.calcIrmsD(29600, 1.51);  // Calculate Irms only 1.51 adjustment for mCP  has no effect on arduino analogs
  
  Serial.print(Irms*230.0);	       // Apparent power
  Serial.print(" ");
  Serial.println(Irms);		       // Irms
}
