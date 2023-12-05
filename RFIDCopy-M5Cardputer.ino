#include <Wire.h>
#include "MFRC522_I2C.h"
#include <M5Cardputer.h>

MFRC522 mfrc522(0x28); // Create MFRC522 instance.

enum state {
  read_mode,
  write_mode
} currentState;

bool readUID = false;

byte UID[20];
uint8_t UIDLength = 0;

void displayReadMode() {
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("RFID Cloner. V1.0"));
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("(Press 'A' to write after reading...)"));
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("Ready to READ..."));
}

void displayWriteMode() {
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("RFID Cloner. V1.0"));
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("(Press 'A' to read a new card...)"));
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("Ready to WRITE..."));
  displayUID();
}

void beep_attention() {
  M5Cardputer.Speaker.tone(882, 40);
  delay(100);
}

void beep_error() {
  M5Cardputer.Speaker.tone(495, 60);
  delay(60);
}

void beep_writeOK() {
  M5Cardputer.Speaker.tone(661, 60);
  delay(60);
  M5Cardputer.Speaker.tone(882, 100);
  delay(100);
}

void cls() {
  M5Cardputer.Display.setTextColor(YELLOW);
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.fillScreen(BLACK);
  M5Cardputer.Display.setCursor(0, 0);
}

void setup() {
  M5Cardputer.begin();
  M5Cardputer.Display.setRotation(1); // Rotate the display by 90 degrees
  M5Cardputer.Display.fillScreen(BLACK);
  M5Cardputer.Display.setCursor(0, 0);
  M5Cardputer.Display.setTextColor(YELLOW);
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.setCursor(0, 0);
  Serial.begin(115200);
  Wire.begin();
  mfrc522.PCD_Init();
  currentState = read_mode;
  displayReadMode();
}

void loop() {
  M5.update();

  if (M5.BtnA.wasReleased() && readUID) {
    cls();
    switch (currentState) {
      case read_mode:
        currentState = write_mode;
        displayWriteMode();
        break;
      case write_mode:
        currentState = read_mode;
        displayReadMode();
        readUID = false;
        break;
    }
  }

  if (!mfrc522.PICC_IsNewCardPresent())
    return;
  if (!mfrc522.PICC_ReadCardSerial())
    return;

  cls();

  switch (currentState) {
    case read_mode:
      displayReadMode();
      readCard();
      break;
    case write_mode:
      displayWriteMode();
      writeCard();
      break;
  }

  mfrc522.PICC_HaltA();
}

void readCard() {
  MFRC522::PICC_Type piccType = (MFRC522::PICC_Type)mfrc522.PICC_GetType(mfrc522.uid.sak);
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.print(F(""));
  M5Cardputer.Display.print(mfrc522.PICC_GetTypeName(piccType));
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.print(F(" (SAK "));
  M5Cardputer.Display.print(mfrc522.uid.sak);
  M5Cardputer.Display.print(")\r\n");
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    M5Cardputer.Display.setTextColor(RED);
    M5Cardputer.Display.setTextSize(1); // Reduce text size
    M5Cardputer.Display.setCursor(0, 80); // Move the error message down
    M5Cardputer.Display.println(F("Not MIFARE Classic"));
    M5Cardputer.Display.setCursor(0, 0); // Reset cursor
    M5Cardputer.Display.setTextColor(YELLOW);
    beep_error();
    delay(1000);
  } else {
    M5Cardputer.Display.println("");
    readUID = true;
    UIDLength = mfrc522.uid.size;
    for (byte i = 0; i < UIDLength; i++) {
      UID[i] = mfrc522.uid.uidByte[i];
    }
    Serial.println();
    displayUID();
    beep_attention();
    delay(1000);
  }
}

void displayUID() {
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  M5Cardputer.Display.println(F("User ID:"));
  M5Cardputer.Display.setTextSize(1); // Reduce text size
  for (byte i = 0; i < UIDLength; i++) {
    M5Cardputer.Display.print(UID[i] < 0x10 ? " 0" : " ");
    M5Cardputer.Display.print(UID[i], HEX);
  }
}

void writeCard() {
  if (mfrc522.MIFARE_SetUid(UID, (byte)UIDLength, true)) {
    M5Cardputer.Display.println();
    M5Cardputer.Display.setTextSize(1); // Reduce text size
    M5Cardputer.Display.println(F("Wrote UID."));
    M5Cardputer.Display.setTextSize(1); // Reduce text size
    M5Cardputer.Display.println();
    beep_writeOK();
  } else {
    M5Cardputer.Display.setTextColor(RED);
    M5Cardputer.Display.setTextSize(1); // Reduce text size
    M5Cardputer.Display.println();
    M5Cardputer.Display.println(F("Write failed."));
    M5Cardputer.Display.setTextSize(1); // Reduce text size
    M5Cardputer.Display.setTextColor(YELLOW);
    beep_error();
  }

  mfrc522.PICC_HaltA();
  delay(1000);
}