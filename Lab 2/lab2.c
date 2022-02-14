// ECE 231, Lab 2
// 2/14/22
// Arjun Viswanathan
// Using the Arduino UNO board with VS Code 

#include <avr/io.h>
#include <util/delay.h>
#define DELAY 1000

int main(void) {
    DDRB = 1<<PB5;

    while(1) {
        PORTB = 1<<PB5;
        _delay_ms(DELAY);
        PORTB = ~ (1<<PB5);
        _delay_ms(DELAY);
    }
    return 0;
}