#include "avr/wdt.h"

#define PIN_IN1   A1
#define PIN_IN2   A2

// Note that we do not control the EN pin. Instead we install the jumper directly on the PCB. 

// the setup function runs once when you press reset or power the board
void setup() {
  
  pinMode( LED_BUILTIN , OUTPUT);
  
  pinMode( PIN_IN1 ,  INPUT_PULLUP );      // IN1
  pinMode( PIN_IN2 ,  INPUT_PULLUP );      // IN2
  
 // Set up the watchdog timer with the maximum timeout
 // We do this becuase the spike when the timer hits zero sometimes
 // glitches the Arduino. Some ferite beads would probably fix this, but I don't have any. 
  wdt_enable(WDTO_250MS);
    
}

// We divide a single cycle into 6 steps....
//    ** 
//  *   *  
//       **
//
// A full cycle takes 1/60th of a second, so each step takes 1/6*1/60=1/360th of a second

const unsigned long us_per_s = 1000000UL;

const unsigned long step_time_us = us_per_s / 360UL;

// Do some flashing so we can visually see that it is running

const unsigned long flash_rate_hz = 10;   // Blink the onboard LED at 10hz so we know it is running

const unsigned long countup_flash_time_us = (us_per_s / flash_rate_hz) / 2;   // div by two becuase we have to do on and off once per period

unsigned long countup_us = 0;

 
// the loop function runs over and over again forever
void loop() {
  
// +-----+-----+-----+-------------------------------------+
// | ENA | IN1 | IN2 | Description                         |
// +-----+-----+-----+-------------------------------------+
// | 0   | N/A | N/A | Motor A is off                      |
// | 1   | 0   | 0   | Motor A is stopped (brakes)         |
// | 1   | 0   | 1   | Motor A is on and turning backwards |
// | 1   | 1   | 0   | Motor A is on and turning forwards  |
// | 1   | 1   | 1   | Motor A is stopped (brakes)         |
// +-----+-----+-----+-------------------------------------+

  // Positive (FORWARD)
  digitalWrite( PIN_IN1 , HIGH );
  digitalWrite( PIN_IN2 , LOW );
  delayMicroseconds( step_time_us );
  delayMicroseconds( step_time_us );
  
  // Zero (Brakes)
  digitalWrite( PIN_IN2 , HIGH );
  delayMicroseconds( step_time_us );

  // Negative (BACKWARD)
  digitalWrite( PIN_IN1 , LOW );
  digitalWrite( PIN_IN2 , HIGH );
  delayMicroseconds( step_time_us );
  delayMicroseconds( step_time_us );

  // Zero  (Brakes)
  digitalWrite( PIN_IN2 , LOW );
  delayMicroseconds( step_time_us );
  
  // This diganostic code just blinks the built in LED so you know it is running. 
  
  countup_us += 6 * step_time_us;
  
  if (countup_us >  countup_flash_time_us ) {
    
    digitalWrite( LED_BUILTIN , !digitalRead(LED_BUILTIN));   // Toggle LED
    
    countup_us -= countup_flash_time_us;
    
  }
  
    // Reset the watchdog timer to prevent a reset
  wdt_reset();
  
}
