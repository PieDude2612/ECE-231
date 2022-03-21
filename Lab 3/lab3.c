// Arjun Viswanathan 
// 3/21/22
// ECE 231, Lab 3
// Using the 7 segment display LED to output numbers for thermometer

/** TODO: 
 * Add in input code from temperature sensor
 * Add in logic to separate decimal point and print out decimal points into the display
 * Add in logic to choose celcius or farenheit at push button switch input
 **/

#include <avr/io.h>
#include <util/delay.h>

int main() {
    unsigned char ledDigits[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};   // digits 0-9 in LED flashes
    char celciusSymbol = 0x39;                                  // lights up C on display, (a, d, e, f)
    char farenheitSymbol = 0x71;                                // lights up F on display, (a, e, f, g)
    int DIG1, DIG2, DIG3, DIG4;                                 // the digits on the 7 segment LED display from left to right

    DDRD = 0xFF;
    DDRB = 0xFF;
    PORTB = 0xFF;                                               // set all pins to output in D and B sides

    while(1) {
        //double celcius = 32.3;                                  // take in input from temperature sensor
        //double farenheit = 77.4;

        //double cel2far = ((9/5)*celcius) + 32;                  // conversion formulas
        //double far2cel = (farenheit-32) * (5/9);

        int i = 25;

        DIG1 = (i/100) % 10;                                    // 100s place, divide by 1000 and mod 10
        if(DIG1 != 0) {                                         // if 0, don't print out to display
            PORTD = ledDigits[DIG1];                            // obtain the corresponding LED code
            PORTB =~ (1<<0);                                    // set the port to low so it flashes
        }

        DIG2 = (i/10) % 10;                                     // 10s place, same procedure
        if(DIG2 != 0) {
            PORTD = ledDigits[DIG2];
            PORTB =~ (1<<1);
        }

        DIG3 = i % 10;                                          // 1s place, same procedure
        PORTD = ledDigits[DIG3];                                // print out anyway if 0 as we want to see output at 0C or 0F
        PORTB =~ (1<<2);

        DIG4 = farenheitSymbol;                                 // choose symbol to print with number
        PORTD = DIG4;
        PORTB =~ (1<<3);
    }
}