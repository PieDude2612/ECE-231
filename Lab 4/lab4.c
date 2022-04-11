// Arjun Viswanathan
// 4/4/22
// ECE 231, Lab 4
// Controlling motor speed using buttons and LEDs on the bare metal ATMega328p IC

#include <avr/io.h>
#include <util/delay.h>
#define S1 0
#define S2 1
#define RED 3
#define GREEN 5
#define BLUE 6

int main() {
    DDRB = 1<<5;
    DDRD = 1<<RED | 1<<GREEN | 1<<BLUE;

    int b1, b2;

    while(1) {
        b1 = PIND & (1<<S1);
        b2 = PIND & (1<<S2);

        if(!b1 && !b2) {
            // set motor speed 0, LED colour RED
            PORTD = (1<<RED);
        } 
        if(b1) {
            // set motor speed low, LED colour YELLOW
            PORTD = (1<<RED) | (1<<GREEN);
        } 
        if(b2) {
            // set motor speed medium, LED colour GREEN
            PORTD = (1<<GREEN);
        } 
        if(b1 && b2) {
            // set motor speed high, LED colour MAGENTA
            PORTD = (1<<RED) | (1<<BLUE);
        }
    }
}