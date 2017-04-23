/*
 emonTx Shield 4 x CT + Voltage example
 
 An example sketch for the emontx Arduino shield module for
 CT and AC voltage sample electricity monitoring. Enables real power and Vrms calculations. 
 
 Part of the openenergymonitor.org project
 Licence: GNU GPL V3
 
 Authors: Glyn Hudson, Trystan Lea
 Builds upon JeeLabs RF12 library and Arduino
 
 emonTx documentation:   http://openenergymonitor.org/emon/modules/emontxshield/
 emonTx firmware code explination: http://openenergymonitor.org/emon/modules/emontx/firmware
 emonTx calibration instructions: http://openenergymonitor.org/emon/modules/emontx/firmware/calibration
 THIS SKETCH REQUIRES:
 Libraries in the standard arduino libraries folder:
  - JeeLib    https://github.com/jcw/jeelib
  - EmonLib   https://github.com/openenergymonitor/EmonLib.git
 Other files in project directory (should appear in the arduino tabs above)
  - emontx_lib.ino
 
*/

/*Recommended node ID allocation
------------------------------------------------------------------------------------------------------------
-ID-  -Node Type- 
0 - Special allocation in JeeLib RFM12 driver - reserved for OOK use
1-4     - Control nodes 
5-10  - Energy monitoring nodes
11-14 --Un-assigned --
15-16 - Base Station & logging nodes
17-30 - Environmental sensing nodes (temperature humidity etc.)
31  - Special allocation in JeeLib RFM12 driver - Node31 can communicate with nodes on any network group
-------------------------------------------------------------------------------------------------------------
emonhub.conf node decoder:
See: https://github.com/openenergymonitor/emonhub/blob/emon-pi/configuration.md
[[6]]
    nodename = emonTxShield
    firmware =emonTxShield
    hardware = emonTxShield
    [[[rx]]]
       names = power1, power2, power3, power4, Vrms
       datacode = h
       scales = 1,1,1,1,0.01
       units =W,W,W,W,V
       
*/
#include <MCP3008s.h>
#define CS_PIN 15          // edit these values in the emonlib.cpp
#define CLOCK_PIN 14
#define MOSI_PIN 13
#define MISO_PIN 12
#define FILTERSETTLETIME 5000                                           //  Time (ms) to allow the filters to settle before sending data

const int CT1 = 1; 
const int CT2 = 1;                                                      // Set to 0 to disable 
const int CT3 = 1;
const int CT4 = 1;
const int CT5 = 1;
const int CT6 = 1;
const int CT7 = 1;
const int CT8 = 1;
const int CT9 = 1;


#include "EmonLibmcpS.h"

EnergyMonitor ct1,ct2,ct3,ct4,ct5,ct6,ct7,ct8,ct9;                                              // Create  instances for each CT channel

// Note: Please update emonhub configuration guide on OEM wide packet structure change:
// https://github.com/openenergymonitor/emonhub/blob/emon-pi/configuration.md
typedef struct { int power1, power2, power3, power4, power5, power6, power7, power8, power9,  Vrms;} PayloadTX;      // create structure - a neat way of packaging data for RF comms
PayloadTX emontx;                                                       

//const int LEDpin = 9;                                                   // On-board emonTx LED 

boolean settled = false;

void setup() 
{
  Serial.begin(115200);
   //while (!Serial) {
 
  Serial.println("emonTX Shield CT123 Voltage example"); 
  Serial.println("OpenEnergyMonitor.org");

 
                                             // if using an arduino remove D calcVID to use onboard analog pin
  if (CT1) ct1.current(1, 66.606);                                     // Setup emonTX CT channel (ADC input, calibration)
  if (CT2) ct2.current(2, 60.606);                                     // Calibration factor = CT ratio / burden resistance
  if (CT3) ct3.current(3, 60.606);                                     // emonTx Shield Calibration factor = (100A / 0.05A) / 33 Ohms
  if (CT4) ct4.current(4, 60.606); 
   if (CT5) ct5.current(5, 66.606);                                     // Setup emonTX CT channel (ADC input, calibration)
  if (CT6) ct6.current(6, 60.606);                                     // Calibration factor = CT ratio / burden resistance
  if (CT7) ct7.current(7, 60.606);                                     // emonTx Shield Calibration factor = (100A / 0.05A) / 33 Ohms
  if (CT8) ct8.current(8, 60.606);          // this is using the next level MCP
  if (CT9) ct9.current(0, 60.606);          // this one is using pin 0 on the MCP chip make sure you use calcVID with it 
  
  if (CT1) ct1.voltage(0, 149, 1.7);                                // ct.voltageTX(ADC input, calibration, phase_shift) - make sure to select correct calibration for AC-AC adapter  http://openenergymonitor.org/emon/modules/emontx/firmware/calibration. Default set for Ideal Power adapter                                         
  if (CT2) ct2.voltage(0, 149, 1.7);                                // 268.97 for the UK adapter, 260 for the Euro and 130 for the US.
  if (CT3) ct3.voltage(0, 149, 1.7);
  if (CT4) ct4.voltage(0, 149, 1.7);
    if (CT5) ct5.voltage(0, 149, 1.7);                                // ct.voltageTX(ADC input, calibration, phase_shift) - make sure to select correct calibration for AC-AC adapter  http://openenergymonitor.org/emon/modules/emontx/firmware/calibration. Default set for Ideal Power adapter                                         
  if (CT6) ct6.voltage(0, 149, 1.7);                                // 268.97 for the UK adapter, 260 for the Euro and 130 for the US.
  if (CT7) ct7.voltage(0, 149, 1.7);
  if (CT8) ct8.voltage(0, 149, 1.7);                                // 268.97 for the UK adapter, 260 for the Euro and 130 for the US.
  if (CT9) ct9.voltage(0, 149, 1.7);
 


 // pinMode(LEDpin, OUTPUT);                                              // Setup indicator LED 
//  digitalWrite(LEDpin, HIGH);
  
                                                                                     
}
                              
                                             // if using an arduino remove D calcVID to use onboard analog pin
void loop() 
{ 
  if (CT1) {
    ct1.calcVID(20,2000, 1.53);                                                  // Calculate all. No.of crossings, time-out 
    emontx.power1 = ct1.realPower;            // remove the D  from calcVID is you wishe to use onboard analog pin 
    Serial.print(" ");
    Serial.print(emontx.power1);                                         
  }
  
  emontx.Vrms = ct1.Vrms;                                            // AC Mains rms voltage 
  
  if (CT2) {
    ct2.calcVID(20,2000, 1.53);                                                  //the 1.53 is to adjust for the mcp timing difference also work to balance CTs has no effect on arduino analog pins 
    emontx.power2 = ct2.realPower;
  
    Serial.print(" "); Serial.print(emontx.power2);
  } 

  if (CT3) {
    ct3.calcVID(20,2000, 1.53);                                                  // Calculate all. No.of crossings, time-out 
    emontx.power3 = ct3.realPower;
    Serial.print(" "); Serial.print(emontx.power3);
  } 
  
   if (CT4) {
     ct4.calcVID(20,2000, 1.53);                                                  // Calculate all. No.of crossings, time-out 
    emontx.power4 = ct4.realPower;
 
    Serial.print(" "); Serial.print(emontx.power4);
  } 
 
  if (CT5) {
    ct1.calcVID(20,2000, 1.53);                                                  // Calculate all. No.of crossings, time-out 
    emontx.power5 = ct5.realPower;
    Serial.print(" ");                                                          //84 arduino
    Serial.print(emontx.power5);                                         
  }
  
                                       // AC Mains rms voltage 
  
  if (CT6) {
    ct6.calcVID(20,2000,1.52);                                                  // Calculate all. No.of crossings, time-out 
    emontx.power6 = ct6.realPower;                                                // .96 arduino
  
    Serial.print(" "); Serial.print(emontx.power6);
  } 

  if (CT7) {
    ct7.calcVID(20,2000, 1.515);                                                  // Calculate all. No.of crossings, time-out 
    emontx.power7 = ct7.realPower;
    Serial.print(" "); Serial.print(emontx.power7);
  } 

if (CT8) {
    ct8.calcVID(20,2000, 1.53);                                                  // Calculate all. No.of crossings, time-out 
    emontx.power8 = ct8.realPower;
    Serial.print(" "); Serial.print(emontx.power8);
  } 
if (CT9) {
    ct9.calcVID(20,2000, 1.53);                                                  // Calculate all. No.of crossings, time-out 
    emontx.power9 = ct9.realPower;
    Serial.print(" "); Serial.print(emontx.power9);
  } 

Serial.print(" "); Serial.print(emontx.Vrms);


  // because millis() returns to zero after 50 days ! 
  if (!settled && millis() > FILTERSETTLETIME) settled = true;

  if (settled)                                                            // send data only after filters have settled
  { 
 Serial.println(" ");
    delay(500);                                                          // delay between readings in ms
  }
}
