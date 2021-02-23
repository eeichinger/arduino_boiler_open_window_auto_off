/*
  Window Monitor Agent

  Monitors a window for open/closed state using a normally open (NO) reed switch 
  and sends "OPEN" or "CLOSED" message to a central controller via NRF24L01 transceiver.

  based on:
  Arduino Wireless Communication Tutorial
      Example 1 - Transmitter Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <JeeLib.h>

// Constants & Variables
RF24 radio(6, 7); // CE, CSN

const byte address[6] = "00001";
const int check_interval_ms = 500; // interval in ms to check reed switch
const int REED_PIN = 2;
const int BUZZER_PIN = 9;
const int LED_PIN = 8;
const char CMD_OPEN[] = "OPEN";
const char CMD_CLOSED[] = "CLOSED";

void setup() {
  Serial.begin(19200);
  pinMode(REED_PIN, INPUT_PULLUP); // set NO reed pin to HIGH by default, only if the switch triggers it will go LOW.
  pinMode(BUZZER_PIN, OUTPUT); // acoustic signal
  digitalWrite(LED_PIN, LOW); // Turn the LED off
  
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
}

void loop() {
  radio.powerUp(); // Power up the radio after sleeping
  radio.stopListening(); // get ready to send

  int proximity = digitalRead(REED_PIN); // Read the state of the switch
  Serial.print("reading switch: ");
  if (proximity == HIGH) // If the pin reads low, the switch is closed.
  {
    Serial.println("closed");
    digitalWrite(LED_PIN, HIGH); // Turn the LED off
    digitalWrite(BUZZER_PIN, HIGH);
    radio.write(&CMD_CLOSED, sizeof(CMD_CLOSED));
    delay(50);
  }
  else
  {
    Serial.println("open");
    digitalWrite(LED_PIN, LOW); // Turn the LED off
    digitalWrite(BUZZER_PIN, HIGH);
    radio.write(&CMD_OPEN, sizeof(CMD_OPEN));
    delay(100);
  }
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("going to sleep...");
  Serial.flush();

  radio.powerDown();              // NOTE: The radio MUST be powered back up again manually
  // Sleep the MCU.
  Sleepy::loseSomeTime(check_interval_ms);
}

ISR(WDT_vect) {
  Sleepy::watchdogEvent();  // Setup the watchdog
}
