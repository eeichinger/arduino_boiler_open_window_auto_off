/*
  Arduino Wireless Communication Tutorial
      Example 1 - Transmitter Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <JeeLib.h>
#include <avr/sleep.h>
#include <avr/power.h>
typedef enum { wdt_16ms = 0, wdt_32ms, wdt_64ms, wdt_128ms, wdt_250ms, wdt_500ms, wdt_1s, wdt_2s, wdt_4s, wdt_8s } wdt_prescalar_e;
void setup_watchdog(uint8_t prescalar);
void do_sleep(void);


// Constants & Variables
RF24 radio(6, 7); // CE, CSN

const byte address[6] = "00001";
const int REED_PIN = 2;
const char CMD_OPEN[] = "OPEN";
const char CMD_CLOSED[] = "CLOSED";

void setup() {
  Serial.begin(9600);
  pinMode(REED_PIN, INPUT_PULLUP);

  setup_watchdog(wdt_4s);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  //  radio.stopListening();
}

void loop() {
  radio.powerUp();                                // Power up the radio after sleeping
  radio.stopListening();

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
  Serial.println("going to sleep...");
  Serial.flush();

  radio.powerDown();              // NOTE: The radio MUST be powered back up again manually
  // Sleep the MCU.
  do_sleep();
  //  Sleepy::loseSomeTime(2000);
}

void wakeUp() {
  sleep_disable();
}
// Sleep helpers
//Prescalar values
// 0=16ms, 1=32ms,2=64ms,3=125ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(uint8_t prescalar) {
  uint8_t wdtcsr = prescalar & 7;
  if ( prescalar & 8 )
    wdtcsr |= _BV(WDP3);
  MCUSR &= ~_BV(WDRF);                      // Clear the WD System Reset Flag
  WDTCSR = _BV(WDCE) | _BV(WDE);            // Write the WD Change enable bit to enable changing the prescaler and enable system reset
  WDTCSR = _BV(WDCE) | wdtcsr | _BV(WDIE);  // Write the prescalar bits (how long to sleep, enable the interrupt to wake the MCU
}

//ISR(WDT_vect) {
//  Sleepy::watchdogEvent();  // Setup the watchdog
//}

ISR(WDT_vect) {
  //--sleep_cycles_remaining;
//  Serial.println(F("WDT"));
}

void do_sleep(void) {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  attachInterrupt(0, wakeUp, LOW);
  WDTCSR |= _BV(WDIE);
  sleep_mode();                        // System sleeps here
  // The WDT_vect interrupt wakes the MCU from here
  sleep_disable();                     // System continues execution here when watchdog timed out
  detachInterrupt(0);
  WDTCSR &= ~_BV(WDIE);
}
