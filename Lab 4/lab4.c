// Arjun Viswanathan
// 4/4/22
// ECE 231, Lab 4
// Controlling motor speed using buttons and LEDs on the bare metal ATMega328p IC

#include <avr/io.h>
#include <util/delay.h>
#define DELAY 1000

int main() {
    DDRB = 1<<5;

    while(1) {
        PORTB = (1<<5);
        _delay_ms(DELAY);
        PORTB =~ (1<<5);
        _delay_ms(DELAY);
    }
}