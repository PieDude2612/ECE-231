// Arjun Viswanathan
// 4/21/22
// ECE 231, Lab 4
// Controlling the intensity of an LED using the ATMega328p bare-metal IC
// I have made two variations: 
// The first is the simple one: control the intensity of a static color
// The second is controlling the intensity of a fade of RGB colors. User can choose which one to execute

// RGB LED fader code referenced from: https://www.programming-electronics-diy.xyz/2021/02/how-to-control-rgb-leds-crossfading-rgb.html

#include <avr/io.h>
#include <util/delay.h>
#define S1 0
#define S2 1
#define RED 3
#define GREEN 5
#define BLUE 6

void displayFade(int intensity, int uD);
void displayNormal(int intensity);

int main() {
    DDRC = 1<<6;
    int brightness = 0;
    int updateDelay = 0;

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
            updateDelay = 20;
        } 
        if(b2) {
            // LED on medium brightness
            DDRD = 1<<RED | 1<<GREEN | 1<<BLUE;
            brightness = 50;
            updateDelay = 4;
        } 
        if(b1 && b2) {
            // LED on high brightness
            DDRD = 1<<RED | 1<<GREEN | 1<<BLUE;
            brightness = 200;
            updateDelay = 1;
        }
        if(!b1 && !b2) {
            // LED off
            DDRD = 0;
        }

        //displayNormal(brightness);
        displayFade(brightness, updateDelay);
    }
}

/**
 * @brief This function will just shine the colors of the LED, with NO fader
 * 
 * @param intensity 
 */
void displayNormal(int intensity) {
    OCR0A = intensity;
    OCR0B = intensity;
    OCR2B = intensity;                    // set the OCRs to PWM the LEDs
}

/**
 * @brief EXTRA: Displays a fader of colors on the RGB LED. The intensity is set by the input variable which 
 * control the brightness of the fader. 
 * NOTE: I have used a variable delay proportional to the intensity, so the cycle of colors is constant across
 * each intensity level, and brightness is the only thing that is changing 
 * @param intensity
 * @param uD
 */
void displayFade(int intensity, int uD) {
    int rgb_values[] = {intensity, 0, 0};         // order: red, green, blue
    int fadeup = 1;                               // color to fade up
    int fadedown = 0;                             // color to fade down

    int count;

    for(count = 0; count < intensity; count++){
        if(rgb_values[fadedown] < 0) {
            rgb_values[fadedown] = 0;
            fadedown += 1;                        // if fade at 0, then move to next color

            if(fadedown > 2) {                    // if we cycled all colors, go back to color 1
                fadedown = 0;
            }
        }

        if(rgb_values[fadeup] > intensity) {      // if fade at max, then move to next color
            rgb_values[fadeup] = intensity;
            fadeup += 1;

            if(fadeup > 2) {                      // if we cycled all colors, go back to color 1
                fadeup = 0;
            }
        }

        OCR0A = rgb_values[2];
        OCR0B = rgb_values[1];
        OCR2B = rgb_values[0];                    // set the OCRs to PWM the LEDs

        rgb_values[fadedown] -= 2;
        rgb_values[fadeup] += 2;                  // increment/decrement the fader values
        _delay_ms(uD);                            // a delay so all cycle rates are all intensities are the same
    }
}