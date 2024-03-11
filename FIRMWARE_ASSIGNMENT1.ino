/*
NAME: HRIDYA V V
DATE: 11/03/2024
DESCRIPTION: Blinking arduino uno onboard led based on temperature
*/

#include <Arduino.h>
#define LM35 A0  //defining temperature sensor to A0
#define LED_PIN 13  // Onboard LED pin

volatile int temperature = 0;  // Variable to store temperature reading

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  
  // Configure Timer 0 for overflow interrupt with prescaler 64
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;
  OCR0A = 125;  // Set compare match register for 250ms blink (assuming 16 MHz clock)
  TIMSK0 |= (1 << TOIE0);  // Enable Timer 0 overflow interrupt
  TCCR0B |= (1 << CS01) | (1 << CS00);  // Start Timer 0 with prescaler 64
}

void loop() {
  // Read analog value from LM35 sensor (connected to A0)
  int sensorValue = analogRead(A0);

  // Convert analog reading to voltage (assuming 5V reference)
  float voltage = sensorValue * 5.0 / 1024.0;

  // Calculate temperature in Celsius (LM35: 10 mV/°C)
  temperature = voltage * 100;
}

// Timer 0 overflow interrupt service routine
ISR(TIMER0_OVF_vect)
{
  TCNT0 = 0;  // Reset timer count

  // Check temperature and adjust blink interval
  if (temperature < 30)
  {
    OCR0A = 125;  // Set compare match for 250ms blink
  }
  else 
  {
    OCR0A = 250;  // Set compare match for 500ms blink (double the value)
  }

  // Toggle LED based on interrupt
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
