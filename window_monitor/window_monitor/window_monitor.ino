/*
  Arduino Wireless Communication Tutorial
      Example 1 - Transmitter Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// Constants & Variables
RF24 radio(6, 7); // CE, CSN

const byte address[6] = "00001";
const int REED_PIN = 2;
const char CMD_OPEN[] = "OPEN";
const char CMD_CLOSED[] = "CLOSED";

void setup() {
  Serial.begin(9600);
  pinMode(REED_PIN, INPUT_PULLUP);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  int proximity = digitalRead(REED_PIN); // Read the state of the switch
  Serial.print("reading switch: ");
  if (proximity == HIGH) // If the pin reads low, the switch is closed.
  {
    Serial.println("closed");
    radio.write(&CMD_CLOSED, sizeof(CMD_CLOSED));
  }
  else
  {
    Serial.println("open");
    radio.write(&CMD_OPEN, sizeof(CMD_OPEN));
    //    digitalWrite(LED_PIN, LOW); // Turn the LED off
  }
}
