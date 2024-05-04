#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Reset pin untuk modul RFID
#define SS_PIN          10         // Slave Select pin untuk modul RFID
#define RELAY_PIN       8          // Pin untuk mengendalikan relay

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Inisialisasi MFRC522

void setup() {
  Serial.begin(9600);             // Inisialisasi komunikasi serial
  SPI.begin();                    // Inisialisasi SPI bus
  mfrc522.PCD_Init();              // Inisialisasi modul RFID

  pinMode(RELAY_PIN, OUTPUT);      // Set pin relay sebagai output
  digitalWrite(RELAY_PIN, LOW);    // Matikan relay secara default
}

void loop() {
  // Pindai kartu RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("Kartu ID:  ");

    // Tampilkan ID kartu
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Cek ID kartu untuk menyalakan relay
    if (checkRFID(mfrc522.uid.uidByte, mfrc522.uid.size)) {
      digitalWrite(RELAY_PIN, HIGH);  // Nyalakan relay
      Serial.println("Relay ON");
      delay(5000);  // Tahan relay selama 1 detik
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("Relay OFF");
    } else {
      Serial.println("ID kartu tidak valid!");
    }

    mfrc522.PICC_HaltA();  // Hentikan kartu
  }
}

// Fungsi untuk memeriksa ID kartu yang valid
bool checkRFID(byte *uid, byte uidLength) {
  // Ganti dengan ID kartu RFID yang diizinkan
  byte allowedUID[] = {0xC3, 0x41, 0xE2, 0x12};

  // Bandingkan ID kartu
  for (byte i = 0; i < uidLength; i++) {
    if (uid[i] != allowedUID[i]) {
      return false;  // ID kartu tidak valid
    }
  }
  return true;  // ID kartu valid
}
