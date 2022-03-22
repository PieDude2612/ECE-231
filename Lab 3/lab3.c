// Arjun Viswanathan 
// 3/21/22
// ECE 231, Lab 3
// Using the 7 segment display LED to output numbers for thermometer
// Temp in F when button unpressed, temp is C when button pressed

/** TODO: 
 * Fix input from temp sensor using ADC logic
 **/

#include <avr/io.h>
#include <util/delay.h>
#define DELAY 1

int main() {
    unsigned char ledDigits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};   // digits 0-9 in LED flashes
    char celciusSymbol = 0x39;                                  // lights up C on display, (a, d, e, f)
    char farenheitSymbol = 0x71;                                // lights up F on display, (a, e, f, g)
    int DIG1, DIG2, DIG3, DIG4;                                 // the digits on the 7 segment LED display from left to right

    DDRD = 0xFF;                                                // set pins for powering parts of the number (a-DP)
    DDRC = 0x00;                                                // read in analog values (temp sensor)
    DDRB = 0x0F;                                                // set pins for COM ports of display numbers
    PORTB = 0x0F;                                               // set 4 pins to HIGH to turn all displays off

    while(1) {
        double temp = 69.6;                                     
        double far2cel = (temp-32.0) * (5.0/9.0);               // convert temp in F to C
        int i;

        // Displaying the C or F at the end of the number
        DIG4 = PINB & (1<<5);                                   // choose symbol to print with number
        if(DIG4 == 0) {                                         // if button pressed, PB5 HIGH, so celcius display, else farenheit
            PORTD = celciusSymbol;
            i = far2cel * 10;
        } else {
            PORTD = farenheitSymbol;
            i = temp * 10;
        }
        PORTB =~ (1<<3);
        _delay_ms(DELAY);

        // Displaying values to the left of decimal point
        DIG1 = (i/100) % 10;                                    // 100s place, divide by 100 and mod 10
        if(DIG1 != 0) {                                         // if 0, don't print out to display
            PORTD = ledDigits[DIG1];                            // obtain the corresponding LED code
            PORTB =~ (1<<0);                                    // set the port to low so it flashes
        }
        _delay_ms(DELAY);

        DIG2 = (i/10) % 10;                                     // 10s place, same procedure
        PORTD = ledDigits[DIG2];
        PORTD |= (1<<7);                                        // decimal point after first 2 numbers
        PORTB =~ (1<<1);
        _delay_ms(DELAY);

        // Displaying values to the right of decimal point
        DIG3 = i % 10;                                          // 1s place, same procedure
        PORTD = ledDigits[DIG3];                                // print out anyway if 0 as we want to see output at 0C or 0F
        PORTB =~ (1<<2);
        _delay_ms(DELAY);
    }
}