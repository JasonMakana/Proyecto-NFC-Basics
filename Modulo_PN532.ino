#include <SPI.h>
#include <Adafruit_PN532.h>

// Declaracionm de pines para SPI
#define SCK  12
#define MISO 13
#define MOSI 11
#define SS   5

// 7D F2 17 07
// 08 80 00 D3 

// Creacion del objeto PN532 usando SPI
Adafruit_PN532 nfc(SCK, MISO, MOSI, SS);

// Funcion para ID en binario
// imprimiendo cada byte 
void printBinary(uint8_t byte) {
  for (int i = 7; i >= 0; i--) {
    Serial.print((byte >> i) & 1);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inicializa SPI
  SPI.begin(SCK, MISO, MOSI, SS);  // SCK, MISO, MOSI, SS (CSN)
  
  // Inicializa el módulo NFC
  nfc.begin();

  // Verificar si el módulo responde
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 no detectado");
    while (1);
  }

  // Activación del modo lectura
  nfc.SAMConfig();
  Serial.println("Esperando tarjeta NFC...");
}

void loop() {
  uint8_t uid[7]; // Variable para el ID de la tarjeta
  uint8_t uidLength; // Variable de la longitud

  // Espera a que detecte la tarjeta NFC
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {

    Serial.println("\nTarjeta detectada");

    // HEXADECIMAL
    // Convierte el UID a Hexadecimal
    Serial.print("HEX: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) Serial.print("0");
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // DECIMAL (como número grande)
    // Convierte UID a Decimal uniendo todos los byte
    // en un solo número 
    Serial.print("DEC: ");
    unsigned long long decimalUID = 0;

    for (uint8_t i = 0; i < uidLength; i++) {
      decimalUID = (decimalUID << 8) | uid[i];
    }

    Serial.println(decimalUID);

    // BINARIO
    // Imprime cada byte
    Serial.print("BIN: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      printBinary(uid[i]);
      Serial.print(" ");
    }
    Serial.println();

    delay(2000);
  }
}