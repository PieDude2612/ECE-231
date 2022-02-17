// ECE 231, Lab 2
// 2/16/22
// Arjun Viswanathan
// LED blinking using switches on Arduino UNO. 
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

void blinkRED();
void blinkGREEN();
void blinkBOTH();
void kill();

int main(void) {
    DDRD = 0x00;                    // initialize input and output pins in D and C
    DDRD |= (1<<5);                 // set output pins 5 and 6, leave others as inputs
    DDRD |= (1<<6);

    int s1; int s2; int s3;

    while(1) {
        s1 = PIND & (1<<2);         // switches 1, 2, and 3 sending input to pins 2, 3, 4
        s2 = PIND & (1<<3);
        s3 = PIND & (1<<4);

        if((s1 && s2) || (s1 && s3) || (s2 && s3)) {
            kill();
        } else if(s1){
            blinkRED();
        } else if(s2) {
            blinkGREEN();
        } else if(s3) {
            blinkBOTH();
        }
    }
    return 0;
}

/* 
Send periodic on-off signal to pin 6, which then blinks the red LED 
on the Arduino board. 
*/
void blinkRED() {
    PORTD |= (1<<6);            // pin 6 HIGH
    _delay_ms(DELAY);
    PORTD &=~ (1<<6);           // pin 6 LOW
    _delay_ms(DELAY);
}

/*
Send periodic on-off signal to pin 5, which then blinks the green LED
on the Arduino board. 
*/
void blinkGREEN() {
    PORTD |= (1<<5);            // pin 5 HIGH
    _delay_ms(DELAY);
    PORTD &=~ (1<<5);           // pin 5 LOW
    _delay_ms(DELAY);
}

/*
Send periodic on-off signal to both pins, causing both LEDs to blink
*/
void blinkBOTH(){
    PORTD |= (1<<6);            // pin 6 HIGH
    PORTD |= (1<<5);            // pin 5 HIGH
    _delay_ms(DELAY);
    PORTD &=~ (1<<6);           // pin 6 LOW
    PORTD &=~ (1<<5);           // pin 5 LOW
    _delay_ms(DELAY);
}

/*
Stop blinking on all LEDs
*/
void kill(){
    PORTD &=~ (1<<5);
    PORTD &=~ (1<<6);
}