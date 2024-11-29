#include <Arduino.h>

/* Declaration of variables __________________________________________ */
const byte interruptPin = 14; /* Pin for the interrupt, this is the D5 pin of NodeMCU */
volatile boolean interrupt_occurred = false; 
int cnt = 0;

void setup() 
{
  Serial.begin(115200); /* Start the serial communication */
  pinMode(interruptPin, INPUT_PULLUP); /* Set the pin as input in pull-up arrangement */
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, FALLING); /* Attach interrupt in falling mode */
}

void loop() 
{
  Serial.println(cnt);    
}

ICACHE_RAM_ATTR void ISR() /* Interrupt Service Routine, triggered on interrupt */
{ 
  cnt++;
}
