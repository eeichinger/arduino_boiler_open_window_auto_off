/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// MONITOR/RIGHT

RF24 radio(6, 7); // CE, CSN

const byte address[6] = "00001";
const int REED_PIN = 2; 
const char CMD_OPEN[] = "OPEN";
const char CMD_CLOSED[] = "CLOSED";

void setup() {
  pinMode(REED_PIN, INPUT_PULLUP);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
 int proximity = digitalRead(REED_PIN); // Read the state of the switch
  if (proximity == LOW) // If the pin reads low, the switch is closed.
  {
//    Serial.println("Switch closed");
//    digitalWrite(LED_PIN, HIGH); // Turn the LED on
    radio.write(&CMD_CLOSED, sizeof(CMD_CLOSED));
  }
  else
  {
//    digitalWrite(LED_PIN, LOW); // Turn the LED off
    radio.write(&CMD_OPEN, sizeof(CMD_OPEN));
  }  
//  radio.write(&text, sizeof(text));
//  delay(1000);
//  const char text[] = "CLOSED";
//  radio.write(&text, sizeof(text));
  delay(200);
}
