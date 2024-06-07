/*
 * options.h
 *
 * Created: 15/09/2019 10:40:03 PM
 *  Author: fhu
 */ 


#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <avr/io.h>

#define LED_PIN 5
#define LED_DDR DDRB

// uncomment to reverse the direction of the motor, NB. is has to rotate counter clockwise
#define MOTOR_REVERSE
// 0 to only forward digit once motor is off; helps reduce current load
// by only having one motor active in chain at a time
#ifndef DEFAULT_FORWARD_ALWAYS
#define DEFAULT_FORWARD_ALWAYS   0
#endif

#define MOTOR_C0_PIN  5
#define MOTOR_C0_DDR  DDRD
#define MOTOR_C0_PORT PORTD

#define MOTOR_C1_PIN  6
#define MOTOR_C1_DDR  DDRD
#define MOTOR_C1_PORT PORTD

#define MOTOR_C2_PIN  7
#define MOTOR_C2_DDR  DDRD
#define MOTOR_C2_PORT PORTD

#define MOTOR_C3_PIN  0
#define MOTOR_C3_DDR  DDRB
#define MOTOR_C3_PORT PORTB


#ifndef MAG_TRIGGER_INVERT
#define MAG_TRIGGER_INVERT    1
#endif

#if MAG_TRIGGER_INVERT == 1
#define MAG_TRIGGER_THRESHOLD 565
#define MIN_VAL_RESET         0
#else
#define MAG_TRIGGER_THRESHOLD 435
#define MIN_VAL_RESET         0xFFFF
#endif

#endif /* OPTIONS_H_ */
