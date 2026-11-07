// ==========================================
// C.H.I.T.R.A. - Unified Data Extractor & Eraser
// Features: NO LED
// ==========================================

#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  
  Serial.println("==========================================");
  Serial.println("C.H.I.T.R.A. DATA EXTRACTION TERMINAL");
  Serial.println("Type 'X' and press Enter to pull data.");
  Serial.println("==========================================");
}

void loop() {
  if (Serial.available() > 0) {
    char incoming = Serial.read();
    
    // The Physical Handshake
    if (incoming == 'X' || incoming == 'x') {
      Serial.println("\n--- COMMENCING EXTRACTION ---");
      Serial.println("Slot,Light_Delta");

      // Extract all 512 slots (stepped by 2 bytes)
      for (int addr = 0; addr < 1024; addr += 2) {
        uint16_t lightValue;
        EEPROM.get(addr, lightValue);
        
        // Only print valid readings, skip empty 0xFFFF slots
        if (lightValue != 0xFFFF) {
          Serial.print(addr / 2); // Reconstructs the Slot number (0 to 511)
          Serial.print(",");
          Serial.println(lightValue); // The pure Solar Delta
        }
      }

      Serial.println("--- EXTRACTION COMPLETE ---");
      Serial.println("\nBeginning total silicon wipe in 15 seconds.");
      Serial.println("TAKE YOUR SCREENSHOT NOW!");
      
      // The 15-Second Screenshot Buffer
      for (int i = 15; i > 0; i--) {
        Serial.print(i);
        Serial.print("... ");
        delay(1000);
      }
      
      // The Deep Silicon Wipe
      Serial.println("\nErasing Vault...");
      for (int i = 0; i < 1024; i++) {
        EEPROM.write(i, 0xFF);
      }
      
      Serial.println("Vault Cleared. System safely paralyzed for tomorrow.");
      while(1); // Freeze the processor forever
    }
  }
}
