/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define NOTE_C5  523
const int BUZZER_PIN = 9;

RF24 radio(6, 7); // CE, CSN
const byte address[6] = "00001";
const int RELAY_PIN = 2; 
const int LED_PIN = 8;
const char CMD_OPEN[] = "OPEN";
const char CMD_CLOSED[] = "CLOSED";
const unsigned long last_message_timeout = 5000;
unsigned long last_message_millis = 0;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); 
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); 
    
  Serial.begin(19200);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print("received:");
    Serial.println(text);
    if(strcmp(text, CMD_OPEN) == 0) {
      Serial.println("switch off relay");
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      tone(BUZZER_PIN, NOTE_C5, 700);
      last_message_millis = millis();
    }
    if(strcmp(text, CMD_CLOSED) == 0) {
      Serial.println("switch on relay");
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, NOTE_C5, 300);
      last_message_millis = millis();
      delay(300);
    }
  } else {
    Serial.println(F("waiting"));
  }
  if (millis() - last_message_millis > last_message_timeout) {
    Serial.println(F("more than timeout_interval passed since last OPEN message - turn on relay"));
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, NOTE_C5, 50);
    delay(100);
  }
  digitalWrite(LED_PIN, LOW);
  Serial.flush();
  delay(200);
}
