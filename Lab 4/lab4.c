// Arjun Viswanathan
// 4/4/22
// ECE 231, Lab 4
// Controlling motor speed using buttons and LEDs on the bare metal ATMega328p IC

#include <avr/io.h>
#include <util/delay.h>
#define DELAY 1000
#define S1 0
#define S2 1
#define RED 4
#define GREEN 5
#define BLUE 6
#define MOTOR 7

int main() {
    DDRB = 1<<5;
    DDRD = 0xF0;

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