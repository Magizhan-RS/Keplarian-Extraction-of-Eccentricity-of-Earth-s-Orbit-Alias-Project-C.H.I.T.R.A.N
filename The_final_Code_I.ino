// ==========================================
// C.H.I.T.R.A. - 90-Minute Interval Logger (BH1750)
// Features: Dead Reckoning, Sky Subtraction, 11-Sec Interval, NO LED
// ==========================================

#include <Wire.h>
#include <BH1750.h>
#include <EEPROM.h>

BH1750 lightMeter;

uint16_t ambientBaseline = 0;
int memoryAddress = 0; 

void setup() {
  Wire.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  delay(100); // Hardware stabilization
  
  // Phase 1: Sky Subtraction Calibration (10 Seconds)
  // Takes 100 samples spaced 100ms apart to create a baseline average.
  uint32_t tempBaseline = 0;
  for (int i = 0; i < 100; i++) {
    tempBaseline += lightMeter.readLightLevel();
    delay(100); 
  }
  ambientBaseline = tempBaseline / 100;
  
  // Phase 2: Silent Transit Logging begins immediately after.
}

void loop() {
  // Phase 3: Memory Full & Safe Mode (After ~93.8 Minutes)
  if (memoryAddress >= 1024) {
    return; // Paralyze the loop silently to protect the transit curve
  }
  
  // ==========================================
  // The 11-Second Delta Logging
  // ==========================================
  uint16_t currentLight = lightMeter.readLightLevel();
  uint16_t solarDelta = 0;
  
  // Subtract atmospheric noise, flattening negative cloud spikes to zero.
  if (currentLight > ambientBaseline) {
    solarDelta = currentLight - ambientBaseline;
  }
  
  // Burn the purified 16-bit number into the current EEPROM slot
  EEPROM.put(memoryAddress, solarDelta);
  
  // Move 2 bytes forward for the next reading
  memoryAddress += 2; 
  
  // Wait exactly 11 seconds before taking the next reading
  delay(11000); 
}
