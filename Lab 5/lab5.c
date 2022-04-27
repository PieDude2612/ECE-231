// Arjun Viswanathan
// 4/24/22
// ECE 231, Lab 5
// Measuring the distance an object is using the HC-SR04 sensor and showing the distance visibly on OLED 
// and audibly on the piezo speaker. Extra: Added a RGB LED which increases in intensity as distance -> 0

// Additional: Added a switch to cut out the speaker as the sound is really annoying, so user only hears it 
// when button is pressed :)

// Code referenced from Prof. McLaughlin's slides on FDTIs and from previous lab

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "i2c.h"
#include "SSD1306.h"

#define TRIG 1
#define ECHO 0
#define RANGE_PER_CLOCK 1.098
#define RED 5
#define SPEAKER 3
#define BLUE 6
#define FOSC 16000000
#define PRESCALER 64

void timer0_init();
void controlLEDIntensity(int intensity);
void makeSound(int frequency);

int main() {
    timer0_init();
    OLED_Init();
    unsigned char rising_edge_clocks, falling_edge_clocks, echo_width_clocks;
    float target_range = 0, frequency = 0;
    int brightness;

    DDRB = 1<<TRIG;                             // set the output pins
    DDRD = 1<<RED | 1<<SPEAKER | 1<<BLUE;
    PORTB &=~ (1<<TRIG);                        // set to low initially 

    while(1) {
        TCNT0 = 0;                              // start the timer
        PORTB |= 1<<TRIG;                       // set to high
        _delay_us(10);                          // delay by 10 uS to create the pulse needed
        PORTB &=~ (1<<TRIG);                    // set to low

        while((PINB & (1<<ECHO)) == 0);         // wait for PB0 to go high and take the timer value
        rising_edge_clocks = TCNT0;
        while(!(PINB & (1<<ECHO)) == 0);        // wait for PB0 to go low and take the timer value
        falling_edge_clocks = TCNT0; 

        // The difference in the rising and falling edges make the distance of the pulse in clock cycles
        if(falling_edge_clocks > rising_edge_clocks) {
            echo_width_clocks = falling_edge_clocks - rising_edge_clocks;
            target_range = echo_width_clocks * RANGE_PER_CLOCK;
        }

        if(target_range < 5) {
            target_range = 5;
        }

        if(target_range > 200) {
            target_range = 200;
        }

        brightness = 200 - target_range;        // control LED brightness
        controlLEDIntensity(brightness);

        // The frequency is the clock frequency / frequency * prescaler * 2 from slides
        frequency = FOSC / ((target_range * 20) * PRESCALER * 2);
        makeSound(frequency);

        char buffer[10];                        // to store display strings

        // Printing the distance in cm
        OLED_GoToLine(0);
        OLED_DisplayString("Target Range (cm): ");
        OLED_GoToLine(1);
        dtostrf(target_range, -3, 0, buffer);
        OLED_DisplayString(buffer);

        // Printing the distance in inches
        OLED_GoToLine(3);
        OLED_DisplayString("Target Range (inch): ");
        OLED_GoToLine(4);
        dtostrf(target_range/2.54, -3, 0, buffer);
        OLED_DisplayString(buffer);

        // Extra: Printing the frequency to see if the speaker actually outputs different frequencies
        OLED_GoToLine(6);
        OLED_DisplayString("Frequency (Hz): ");
        OLED_GoToLine(7);
        dtostrf(frequency, -4, 0, buffer);
        OLED_DisplayString(buffer);
    }
}

/**
 * @brief Initialize the timer counter
 */
void timer0_init() {
    TCCR0A = 0xA3;
    TCCR0B = 0x05;
    TCNT0 = 0;

    TCCR2A = 0x23;
    TCCR2B = 0x03;
    TCNT2 = 0;
}

/**
 * @brief This function will just shine the colors of the LED, with NO fader
 * 
 * @param intensity 
 */
void controlLEDIntensity(int intensity) {
    OCR0A = intensity;
    OCR0B = intensity;                    // set the OCRs to PWM the LEDs
    _delay_ms(10);
}

void makeSound(int frequency) {
    OCR2B = frequency;
    _delay_ms(10);
}