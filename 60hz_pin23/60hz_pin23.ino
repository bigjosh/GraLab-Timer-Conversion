#include <FastGPIO.h>

#define PIN_IN1   2
#define PIN_IN2   3
#define PIN_ENA   4

// This is the driving parameter. It is the number of full sine wave cycles per second.
// It will likely be either 60 or 50 depending on what country the motor is designed to run in. 
static const unsigned CYCLES_PER_S = 60;

// We will only model half of a full sine wave. We manually switch polarity between phases. 
static const unsigned PHASES_PER_CYCLE = 2;

// How finly will we sample the sine wave?
// The higher this number, the more accurate our sine wave will be, but if we make this too
// high then we will run out of time and not be able to keep up.
static const unsigned SAMPLES_PER_PHASE = 100;   // How many steps in our sine wave sample array?

static const unsigned long SAMPLES_PER_CYCLE = (SAMPLES_PER_PHASE * PHASES_PER_CYCLE);

static const unsigned long SAMPLES_PER_S = (SAMPLES_PER_CYCLE * CYCLES_PER_S );

// Units
static const unsigned long MS_PER_S  = (1000UL);
static const unsigned long US_PER_MS = (1000UL);
static const unsigned long US_PER_S  = (MS_PER_S*US_PER_MS);


// One clock is a single clock cycle on the AVR which is running at 16Mhz on the Aurdion UNO
#define CLOCKS_PER_SECOND F_CPU
static const unsigned long CLOCKS_PER_SAMPLE = ( CLOCKS_PER_SECOND / SAMPLES_PER_S );

static_assert( CLOCKS_PER_SAMPLE > 100UL , "We need time to run the ISR. Decrease samples per phase or cycles per second.");
static_assert( CLOCKS_PER_SAMPLE > (2UL^16) , "The timer compare value only has 16 bits. Increase samples per phase or cycles per second.");

static const unsigned long US_PER_SAMPLE = ( US_PER_S / SAMPLES_PER_S );

// Table of samples
int single_phase_sin_delay_us[SAMPLES_PER_PHASE];

void setup()  {
  // put your setup code here, to run once:
  
  pinMode( PIN_IN1 , OUTPUT );      // IN1
  pinMode( PIN_IN2 , OUTPUT );      // IN2
  pinMode( PIN_ENA , OUTPUT );      // ENA
  
  
  
  for(unsigned i=0; i < SAMPLES_PER_PHASE; i++ )  {

    float r = sin( PI * i / SAMPLES_PER_PHASE  );   // Range 0-1, domain from 0 to PI (which is one phase of the the full since wave cycle);
    
    single_phase_sin_delay_us[i] = (int) ( r * US_PER_SAMPLE);   // Convert the 0.0-1.0 sin value to a 0-1000us value. Note we kow that there are no negative sin values in this first phase of the sin wave.
    
  }
  
  cli();      // Disable interrupts so they do not add jitter.
  
  // We leave the enable pin high and control direction and power with the EN pins
  digitalWrite( PIN_ENA , HIGH );
  
  // Turn off the builtin LED for slight power savings
  pinMode( LED_BUILTIN , OUTPUT );  
  digitalWrite( LED_BUILTIN , LOW );

  
}

// Send a single phase of the sin wave out on the specified pin 

template < unsigned pin,  unsigned on_level,  unsigned off_level>
void pwm_single_phase() {

  for(unsigned i=0; i< SAMPLES_PER_PHASE; i++) {
    int on_us= single_phase_sin_delay_us[i];
    int off_us = US_PER_SAMPLE-on_us;
    
    FastGPIO::Pin<pin>::setOutput(on_level);   
    delayMicroseconds( on_us );
    FastGPIO::Pin<pin>::setOutput(off_level);    
    delayMicroseconds( off_us );    
  }  
  
}

// +-----+-----+-----+-------------------------------------+
// | ENA | IN1 | IN2 | Description                         |
// +-----+-----+-----+-------------------------------------+
// | 0   | N/A | N/A | Motor A is off                      |
// | 1   | 0   | 0   | Motor A is stopped (brakes)         |
// | 1   | 0   | 1   | Motor A is on and turning backwards |
// | 1   | 1   | 0   | Motor A is on and turning forwards  |
// | 1   | 1   | 1   | Motor A is stopped (brakes)         |
// +-----+-----+-----+-------------------------------------+

void loop() {
  
  // Positive forward phase
  FastGPIO::Pin<PIN_IN1>::setOutput(HIGH);    
  pwm_single_phase< PIN_IN2 , LOW , HIGH >();
  
  // Negative backward phase    
  FastGPIO::Pin<PIN_IN1>::setOutput(LOW);    
  pwm_single_phase< PIN_IN2 , HIGH , LOW >();
  
}
