// ECE 231, Lab 2
// 2/22/22
// Arjun Viswanathan
// LED blinking using switches on Arduino UNO. 
// On switch 1 pushed, blink red LED at 5 Hz
// On switch 2 pushed, blink green LED at 5 Hz
// On switch 3 pushed, blink both LEDs at 5 Hz
// On any two switches pushed, turn off all LEDs

// |= : HIGH
// &=~ : LOW
// ^= : toggle

// Pinouts, Library Imports, Constants
#include <avr/io.h>
#include <util/delay.h>

#define DELAY 100                   // delay = 1000 (ms/s) / 10 (on-offs/s)
#define GREENOUT 5                  // output pin 5 to blink GREEN LED
#define REDOUT 6                    // output pin 6 to blink RED LED
#define S1 2                        // input pin 2 from switch 1, blinks RED LED
#define S2 3                        // input pin 3 from switch 2, blinks GREEN LED
#define S3 4                        // input pin 4 from switch 3, blinks BOTH LEDs

int main(void) {
    DDRD = 0x00;                    // initialize input pins
    DDRD |= (1<<REDOUT);            // set output pins 5 and 6, leave others as inputs
    DDRD |= (1<<GREENOUT);

    int switch1; int switch2; int switch3;

    while(1) {
        switch1 = PIND & (1<<S1);         // switches 1, 2, and 3 sending input to pins 2, 3, 4
        switch2 = PIND & (1<<S2);
        switch3 = PIND & (1<<S3);

        if(!((switch1 && switch2) || (switch1 && switch3) || (switch2 && switch3))) {
            if(switch1){
                PORTD |= (1<<REDOUT);               // pin 6 HIGH
            } else if(switch2) {
                PORTD |= (1<<GREENOUT);             // pin 5 HIGH
            } else if(switch3) {
                PORTD |= (1<<REDOUT);               // pin 6 HIGH
                PORTD |= (1<<GREENOUT);             // pin 5 HIGH
            }
        }
        _delay_ms(DELAY);
        PORTD &=~ (1<<REDOUT);             // pin 6 LOW
        PORTD &=~ (1<<GREENOUT);           // pin 5 LOW
        _delay_ms(DELAY);
    }
    return 0;
}