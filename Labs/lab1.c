// ECE 231, Lab 1
// 2/3/22
// Arjun Viswanathan
// Convert and print a user-input decimal number to its corresponding value in a user-specified base

// References:
// Bitwise shifting: https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/
// Working with chars: https://stackoverflow.com/questions/14416759/return-char-string-from-a-function

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

char *convertToBinary(int number);

void main() {
    while(1) {
        int inputs[2];      // input array
        printf("Enter a number and a base separated by a space (0 0 to quit): ");
        scanf("%d %d", &inputs[0], &inputs[1]);

        if(inputs[0] == 0 && inputs[1] == 0){           // if any input is 0 exit
            break;
        }
        
        while(!(inputs[1] == 16 || inputs[1] == 10 || inputs[1] == 8 || inputs[1] == 2)) {
            printf("Enter a base of either 16, 10, 8, or 2: ");
            scanf("%d", &inputs[1]);                    // as long as the base is not one of the allowed bases, it asks to repeat
        }

        // Automatic conversion with built-in delimiters
        if(inputs[1] == 10) {
            printf("%d in base %d = %d \n", inputs[0], inputs[1], inputs[0]);
        } else if(inputs[1] == 2) {
            char *binaryVal = convertToBinary(inputs[0]);   // returns an array with the binary value
            printf("%d in base %d = %s \n", inputs[0], inputs[1], binaryVal);
        } else if(inputs[1] == 8) {
            printf("%d in base %d = %o \n", inputs[0], inputs[1], inputs[0]);
        } else if(inputs[1] == 16) {
            printf("%d in base %d = %X \n", inputs[0], inputs[1], inputs[0]);
        }

        // Manual conversion
        // if(inputs[1] == 10) {
        //     printf("%d in base %d = %d \n", inputs[0], inputs[1], inputs[0]);
        // } else if(inputs[1] == 2) {
        //     printf("%d in base %d = %s \n", inputs[0], inputs[1], binaryVal);
        // } else if(inputs[1] == 8) {
        //     char octVals[3];
        //     int oct1 = 0; int oct2 = 0; int oct3 = 0;
        //
        //     for(int i = 0; i < 8; i++) {
        //         if(i < 2) {
        //             oct1 += (binaryVal[i]- '0') * pow(2, 1-i);
        //         } else if(i >= 2 && i < 5) {
        //             oct2 += (binaryVal[i]- '0') * pow(2, 4-i);
        //         } else if(i >= 5) {
        //             oct3 += (binaryVal[i]- '0') * pow(2, 7-i);
        //         } // find decimal value equivalent for the numbers
        //     }
        //
        //     octVals[0] = oct1 + '0';
        //     octVals[1] = oct2 + '0';
        //     octVals[2] = oct3 + '0';        // tack on ASCII 0 to offset into char
        //     octVals[3] = '\0';
        //
        //     printf("%d in base %d = %s \n", inputs[0], inputs[1], octVals);
        // } else if(inputs[1] == 16) {
        //     char hexVals[2];
        //     int hex1 = 0; int hex2 = 0;
        //
        //     for(int i = 0; i < 8; i++) {
        //         if(i < 4) {
        //             hex1 += (binaryVal[i]- '0') * pow(2, 4-i);
        //         } else {
        //             hex2 += (binaryVal[i]- '0') * pow(2, 7-i);
        //         } // find decimal equivalent for numbers
        //     }
        //
        //     if(hex1 < 10) {
        //         hexVals[0] = hex1 + '0';    // tack on ASCII offset to get hex values
        //     } else {
        //         hexVals[0] = hex1 + '7';    // tack on ASCII offset to get hex values
        //     }
        //
        //     if(hex2 < 10) {
        //         hexVals[1] = hex2 + '0';    // tack on ASCII offset to get hex values
        //     } else {
        //         hexVals[1] = hex2 + '7';    // tack on ASCII offset to get hex values
        //     }
        //
        //     hexVals[2] = '\0';
        //
        //     printf("%d in base %d = %s \n", inputs[0], inputs[1], hexVals);
        // }
    }
}

char *convertToBinary(int number){
    char *conversion = malloc(sizeof(char) * 8);            // final binary answer

    for(int j = 7; j >= 0; j--) {                           // keep going down from MSB to LSB
        if((number >> j) & 1) {                             // the bitwise shift right will return the binary bit value from computer memory
            conversion[7-j] = '1';                          // append 1 or 0 to end of char array
        } else {
            conversion[7-j] = '0';                          
        }
    }
    conversion[8] = '\0';                                   // set full stop to an array
    return conversion;
}