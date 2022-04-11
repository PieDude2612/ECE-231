// Arjun Viswanathan
// 4/11/22
// ECE 231, Lab 4
// Controlling the intensity of an LED using the ATMega328p bare-metal IC

#include <avr/io.h>
#include <util/delay.h>
#define S1 0
#define S2 1
#define RED 3
#define GREEN 5
#define BLUE 6
#define DELAY 10

void setIntensity(int intensity);

int main() {
    DDRB = 1<<5;
    DDRD = 1<<RED | 1<<GREEN | 1<<BLUE;

    // Since 2 colors (pins 5 and 6) run on TCCR0A, we set COM0A1 and COM0B1 to high
    // Since 1 color (pin 3) runs on TCCR2A, we set COM0B1 to high
    TCCR0A = 0xA3;
    TCCR0B = 0x03;
    TCCR2A = 0x23;
    TCCR2B = 0x03;

    int b1, b2;

    while(1) {
        b1 = PIND & (1<<S1);
        b2 = PIND & (1<<S2);

        if(b1) {
            // LED on dim brightness
            setIntensity(10);
        } 
        if(b2) {
            // LED on medium brightness
            setIntensity(100);
        } 
        if(b1 && b2) {
            // LED on max brightness
            setIntensity(255);
        }
        if(!b1 && !b2) {
            // LED off
            PORTD = 0x00;
            setIntensity(0);
        }
    }
}

/**
 * @brief Sets the upper bound on the timers, and controls the intensity of the LED
 * The timer counts upto the OCR and then shuts the signal off, resets the OCR and counts again
 * This will keep the LED at given intensity on RGB
 * As extra, we can loop the OCR values so the LED cycles different colors
 * @param intensity 
 */
void setIntensity(int intensity) {
    OCR0A = intensity;
    OCR0B = intensity;
    OCR2B = intensity;
}