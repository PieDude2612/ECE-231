// ECE 231, Lab 2
// 2/16/22
// Arjun Viswanathan
// LED blinking using switches. 
// On switch 1 pushed, blink red LED at 5 Hz
// On switch 2 pushed, blink green LED at 5 Hz
// On switch 3 pushed, blink both LEDs at 5 Hz
// On any two switches pushed, turn off all LEDs

// |= : HIGH
// &=~ : LOW
// ^= : toggle

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#define DELAY 100

void blinkRED();
void blinkGREEN();
void blinkBOTH();

int main(void) {
    DDRD = 0x00;
    DDRD |= (1<<5);                  // set output pins 5 and 6, leave others as inputs
    DDRD |= (1<<6);

    bool red = false;
    bool green = false;

    while(1) {
        if((PIND & (1<<2)) == 1){
            blinkRED();
        } else if((PIND & (1<<3)) == 1) {
            red = false;
            green = !green;
        } else if((PIND & (1<<4)) == 1) {
            red = true;
            green = true;
        } else if((((PIND & (1<<2)) == 1) && ((PIND & (1<<3)) == 1)) || 
                    (((PIND & (1<<2)) == 1) && ((PIND & (1<<4)) == 1)) ||
                    (((PIND & (1<<3)) == 1) && ((PIND & (1<<4)) == 1))) {
            red = false;
            green = false;
        }

        if(red && green) {
            blinkBOTH();
        } else if(red) {
            blinkRED();
        } else if(green) {
            blinkGREEN();
        }
    }
    return 0;
}

void blinkRED() {
    PORTD |= (1<<6);            // pin 6 HIGH
    _delay_ms(DELAY);
    PORTD &=~ (1<<6);           // pin 6 LOW
    _delay_ms(DELAY);
}

void blinkGREEN() {
    PORTD |= (1<<5);            // pin 5 HIGH
    _delay_ms(DELAY);
    PORTD &=~ (1<<5);           // pin 5 LOW
    _delay_ms(DELAY);
}

void blinkBOTH(){
    PORTD |= (1<<6);            // pin 6 HIGH
    PORTD |= (1<<5);            // pin 5 HIGH
    _delay_ms(DELAY);
    PORTD &=~ (1<<6);           // pin 6 LOW
    PORTD &=~ (1<<5);           // pin 5 LOW
    _delay_ms(DELAY);
}