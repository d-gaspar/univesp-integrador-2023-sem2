#include <SPI.h>               // library SPI, interligar perifericos a um dispositivo mestre utilizando comunicacao serial
#include <MFRC522.h>           // library RFID

#define DELAY_MS 500           // DELAY
#define RGB_RED_LED_PIN 6      // LED pino vermelho
#define RGB_GREEN_LED_PIN 5    // LED pino verde
#define RGB_BLUE_LED_PIN 3     // LED pino azul
#define SS_PIN 10              // pino SDA do arduino
#define RST_PIN 9              // pino RST (RESET) do arduino

#define NUMBER_OF_KEYS 2
static unsigned short code[NUMBER_OF_KEYS][4] = { {45, 28, 7, 50},{100, 20, 300, 400} };

MFRC522 rfid(SS_PIN, RST_PIN); //Iniciamos o objeto RFID

String cardID;      //Declaramos uma variável de tipo string 
                    //para armazenar o valor dos dados obtidos da
                    // etiqueta RFID

void setup() {

  // RFID
  Serial.begin(9600); // inicia comunicacao serial para ler as respostas do modulo RFID
  SPI.begin(); // inicia comunicacao SPI
  rfid.PCD_Init(); // inicia RFID
  delay(10);
  rfid.PCD_DumpVersionToSerial();
  
  // LED RGB
  pinMode(RGB_RED_LED_PIN, OUTPUT);
  pinMode(RGB_BLUE_LED_PIN, OUTPUT);
  pinMode(RGB_GREEN_LED_PIN, OUTPUT);
}

// Funcao para escolher a cor dos LEDs (HIGH ou LOW)
void setRgbLedColor(int red, int green, int blue){
  digitalWrite(RGB_RED_LED_PIN, red);
  digitalWrite(RGB_GREEN_LED_PIN, green);
  digitalWrite(RGB_BLUE_LED_PIN, blue);
}

void loop() {
  // Reseta o loop se nenhum cartao for detectado
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  Serial.println("Cartao detectado");

  unsigned short doOpen = checkID();
  if (doOpen != 0) {
    setRgbLedColor(LOW, HIGH, LOW); // LED verde
    delay(DELAY_MS);
    setRgbLedColor(LOW, LOW, LOW); // LED off
  } else {
    setRgbLedColor(HIGH, LOW, LOW); // LED vermelho
    delay(DELAY_MS);
    setRgbLedColor(LOW, LOW, LOW); // LED off
  }
}

unsigned short checkID() {
  if (!rfid.PICC_ReadCardSerial()) {
    return 0;
  }

  Serial.print("UID size: "); Serial.println(rfid.uid.size);
  Serial.print("UID: ");
  Serial.print(rfid.uid.uidByte[0]); Serial.print(":");
  Serial.print(rfid.uid.uidByte[1]); Serial.print(":");
  Serial.print(rfid.uid.uidByte[2]); Serial.print(":");
  Serial.print(rfid.uid.uidByte[3]); Serial.println(":");

  short doOpen = 0, i;
  for(i=0; i<NUMBER_OF_KEYS; i++){
    if(rfid.uid.uidByte[0]==code[i][0] && rfid.uid.uidByte[1]==code[i][1] && rfid.uid.uidByte[2]==code[i][2] && rfid.uid.uidByte[3]==code[i][3]){
      doOpen = 1;
      Serial.println("Codigo aceito");
      break;
    }
  }
  
  rfid.PICC_HaltA(); // Stop reading
  return doOpen;
}
