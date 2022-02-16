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
#define DELAY 100
#define SWITCH 10

void blinkRED();
void blinkGREEN();

int main(void) {
    DDRD |= (1<<5);                  // set output pins 5 and 6, leave others unaffected
    DDRD |= (1<<6);
    int sum = 0;

    while(1) {
        if(sum < (SWITCH / 2)) {
            blinkRED();
        } else if(sum > (SWITCH / 2) && sum < SWITCH) {
            blinkGREEN();
        } else if(sum > SWITCH) {
            sum = 0;
        }
        sum += 1;
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