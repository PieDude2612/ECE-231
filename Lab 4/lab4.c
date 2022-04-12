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

void display(int intensity);

int main() {
    DDRB = 1<<5;
    int brightness = 0;

    // Since 2 colors (pins 5 and 6) run on TCCR0A, we set COM0A1 and COM0B1 to high
    // Since 1 color (pin 3) runs on TCCR2A, we set COM0B1 to high
    // Both run on clk/64 prescaler so CS01 and CS00 are high
    TCCR0A = 0xA3;
    TCCR0B = 0x03;
    TCCR2A = 0x23;
    TCCR2B = 0x03;

    int b1, b2;

    while(1) {
        b1 = PIND & (1<<S1);
        b2 = PIND & (1<<S2);

        // When no buttons are pressed, set DDRD 0 so no output is passed out
        if(b1) {
            // LED on dim brightness
            DDRD = 1<<RED | 1<<GREEN | 1<<BLUE;
            brightness = 10;
        } 
        if(b2) {
            // LED on medium brightness
            DDRD = 1<<RED | 1<<GREEN | 1<<BLUE;
            brightness = 50;
        } 
        if(b1 && b2) {
            // LED on max brightness
            DDRD = 1<<RED | 1<<GREEN | 1<<BLUE;
            brightness = 255;
        }
        if(!b1 && !b2) {
            // LED off
            DDRD = 0;
        }

        display(brightness);
    }
}

/**
 * @brief Displays a fader of colors on the RGB LED. The intensity is set by the input variable which 
 * control the brightness of the fader. * 
 * @param intensity 
 */
void display(int intensity) {
    int rgb_values[] = {intensity, 0, 0};         // order: red, green, blue
    int fadeup = 1;                               // color to fade up
    int fadedown = 0;                             // color to fade down

    int count;

    for(count = 0; count < 200; count++){
        if(rgb_values[fadedown] < 0) {
            rgb_values[fadedown] = 0;
            fadedown += 1;

            if(fadedown > 2) {
                fadedown = 0;
            }
        }

        if(rgb_values[fadeup] > intensity) {
            rgb_values[fadeup] = intensity;
            fadeup += 1;

            if(fadeup > 2) { 
                fadeup = 0;
            }
        }

        OCR0A = rgb_values[2];
        OCR0B = rgb_values[1];
        OCR2B = rgb_values[0];

        rgb_values[fadedown] -= 1;
        rgb_values[fadeup] += 1;
        _delay_ms(1);
    }
}