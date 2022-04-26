// Arjun Viswanathan
// 4/24/22
// ECE 231, Lab 5
// Measuring the distance an object is using the HC-SR04 sensor and showing the distance visibly on )LED 
// and audibly on the piezo speaker

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "i2c.h"
#include "SSD1306.h"

#define TRIG 1
#define ECHO 0
#define RANGE_PER_CLOCK 1.098
#define RED 3
#define GREEN 5
#define BLUE 6

void uart_init();
void uart_send(char c);
void send_string(char *stringAddress);
void timer0_init();
void send_to_monitor(float range);
void displayFade(int brightness, int uD);

int main() {
    uart_init();
    timer0_init();
    OLED_Init();
    unsigned char rising_edge_clocks, falling_edge_clocks, echo_width_clocks;
    float target_range;
    int brightness, updateDelay;

    DDRB = 1<<TRIG;                             // set the output pins
    DDRD = 1<<RED | 1<<GREEN | 1<<BLUE;
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
            send_to_monitor(target_range);
        }

        if(target_range < 5) {
            target_range = 5;
        }

        if(target_range > 200) {
            target_range = 200;
        }

        brightness = 200 - target_range;
        updateDelay = 200 / brightness;
        displayFade(brightness, updateDelay);

        char buffer[10];

        OLED_GoToLine(0);
        OLED_DisplayString("Target Range (cm): ");
        OLED_GoToLine(1);
        dtostrf(target_range, -3, 0, buffer);
        OLED_DisplayString(buffer);
        OLED_GoToLine(3);
        OLED_DisplayString("Target Range (inch): ");
        OLED_GoToLine(4);
        dtostrf(target_range/2.54, -3, 0, buffer);
        OLED_DisplayString(buffer);
    }
}

/**
 * @brief Initialize the UART
 */
void uart_init() {
    UCSR0B = 1<<TXEN0;
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
    UBRR0L = 103;
}

/**
 * @brief Send the letter passed into the serial monitor through UART
 * @param letter 
 */
void uart_send(char letter) {
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = letter;
}

/**
 * @brief Send a string character by character 
 * @param stringAddress 
 */
void send_string(char *stringAddress) {
    unsigned char i;
    for(i = 0; i < strlen(stringAddress); i++) {
        uart_send(stringAddress[i]);
    }
}

/**
 * @brief Initialize the timer counter
 */
void timer0_init() {
    TCCR0A = 0xA3;
    TCCR0B = 0x05;
    TCCR2A = 0x23;
    TCCR2B = 0x05;
    TCNT0 = 0;
}

/**
 * @brief Outputs the distance an object is in cm and inches to PuTTY
 * @param range 
 */
void send_to_monitor(float range) {
    char buffer[10];

    send_string("Target Range = ");
    dtostrf(range, 3, 0, buffer);
    send_string(buffer);
    send_string(" cm /");
    dtostrf(range/2.54, 3, 0, buffer);
    send_string(buffer);
    send_string(" inch");
    
    uart_send(13);
    uart_send(10);
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