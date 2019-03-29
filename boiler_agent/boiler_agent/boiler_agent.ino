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

RF24 radio(6, 7); // CE, CSN
const byte address[6] = "00001";
const int RELAY_PIN = 2; 
const char CMD_OPEN[] = "OPEN";
const char CMD_CLOSED[] = "CLOSED";
const unsigned long timeout_interval = 5000;
unsigned long last_closed_millis = -1;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); 
    
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
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
      digitalWrite(RELAY_PIN, HIGH);
      last_closed_millis = millis();
    }
  } else {
    Serial.println(F("waiting"));
  }
  if (millis() - last_closed_millis > timeout_interval) {
    Serial.println(F("more than timeout_interval passed since last OPEN message - turn on relay"));
    digitalWrite(RELAY_PIN, LOW);
  }
  delay(1000);
}
