/* lab2.c
 * Your Name    <<-- you must change to your name and id
 * Clemson ID
 * ECE 2220, Fall 2023
 * MP2 
 *
 * Purpose:  Create a program that can encode and decode data and detect errors down to the bit for (29,24) Hamming Code.
 *
 * Assumptions:
 *  #1:  The menu driven input was provided and must be used exactly
 *       as written.  A user can enter commands:
 *            enc CUt
 *            dec 0E8A549C
 *            quit
 *       Encoding takes three printable ASCII letters
 *       Decoding takes up to eight HEX digits. If exactly eight digits are
 *            entered, the first digit must be 0 or 1.
 *            Leading zeros can be dropped.
 *
 *  #2:  The string and character type libraries cannot be used except as
 *       already provided.  These libraries are for checking inputs in main
 *       and in printing after decoding is complete.  They cannot be used
 *       for anyother purpose.
 *
 *  #3:  No arrays can be used (excpet as already provided for collecting
 *       keyboard input).  You must use bitwise operators for all encoding
 *       and decoding.  If you want to use an array as a lookup table you
 *       must first propose your design and get it approved.  Designs that
 *       use tables to avoid bitwise operators will not be approved.  There
 *       are many good and simple designs that do not require tables.
 *
 *  #4   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs: 
 * 
 * The encode function outputs the correct parity values and binary values, but continues to give the wrong Codeword. This is a bug in the logic, not the syntax.
 * The decode function detects bits that do not need correcting.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 100

// Function prototypes
void encode(unsigned char first_letter, unsigned char second_letter, unsigned char third_letter);
void decode(unsigned int codeword);

int main() 
{
    char line[MAXLINE];
    char command[MAXLINE];
    char inputcs[MAXLINE];
    int items;
    int i, invalid;
    unsigned int codeword;

    // Print program introduction and available commands
    printf("\nMP2: encoding and decoding (29, 24) Hamming code.\n");
    printf("Commands:\n\tenc 3-letters\n\tdec 8-hex-digits\n\tquit\n");

    // Read user input and execute corresponding commands
    while (fgets(line, MAXLINE, stdin) != NULL) 
    {
        items = sscanf(line, "%s%s", command, inputcs);
        if (items == 1 && strcmp(command, "quit") == 0) 
        {
            break;
        } 

        else if (items == 2 && strcmp(command, "enc") == 0) 
        {
            // Validate and encode the input
            if (strlen(inputcs) != 3 || !isprint(inputcs[0]) || !isprint(inputcs[1]) || !isprint(inputcs[2])) 
            {
                printf("Invalid input to encoder: %s\n", inputcs);
            } 

            else 
            {
                encode(inputcs[0], inputcs[1], inputcs[2]);
            }
        } 
        
        else if (items == 2 && strcmp(command, "dec") == 0) 
        {
            // Validate and decode the input
            items = sscanf(inputcs, "%x", &codeword);
            if (items != 1 || strlen(inputcs) > 8) 
            {
                printf("Invalid input to decoder: %s\n", inputcs);
            } 

            else 
            {
                for (i = 0, invalid = 0; i < strlen(inputcs) && !invalid; i++) 
                {
                    if (!isxdigit(inputcs[i]))
                        invalid = 1;
                }

                if (invalid) 
                {
                    printf("Invalid decoder digits: %s\n", inputcs);
                } 
                
                else if (strlen(inputcs) == 8 && inputcs[0] != '1' && inputcs[0] != '0') 
                {
                    printf("Invalid decoder leading digit: %s\n", inputcs);
                } 
                
                else 
                {
                    decode(codeword);
                }
            }
        } 
        
        else 
        {
            printf("# :%s", line);
        }
    }
    
    printf("Goodbye\n");
    return 0;
}

void encode(unsigned char first_letter, unsigned char second_letter, unsigned char third_letter) 
{
    unsigned int codeword = 0;
    int i;

    // Display the input and its encoding process
    printf("%9s%9c%9c%9c\n", "Encoding:", third_letter, second_letter, first_letter);
    printf(" 0x    00%9x%9x%9x\n", third_letter, second_letter, first_letter);

    // Construct information word and copy it into codeword
    unsigned int information_word = (first_letter << 16) | (second_letter << 8) | third_letter;

    // Calculate and set parity bits
    int parity1 = 0;
    int parity2 = 0;
    int parity4 = 0;
    int parity8 = 0;
    int parity16 = 0;

    for (i = 0; i < 29; i++) 
    {
        int bit = (information_word >> i) & 1;
        if (bit) 
        {
            parity1 ^= 1;
            if (i % 2 == 0) 
            {
                parity2 ^= 1;
            }

            if (i % 4 == 0) {
                parity4 ^= 1;
            }
            if (i % 8 == 0) 
            {
                parity8 ^= 1;
            }

            if (i % 16 == 0) 
            {
                parity16 ^= 1;
            }
        }
    }

    // Set parity bits in the codeword using bitwise OR
    codeword |= (parity1 << 1);
    codeword |= (parity2 << 2);
    codeword |= (parity4 << 4);
    codeword |= (parity8 << 8);
    codeword |= (parity16 << 16);

    // Print information word in binary
    printf("Information Word: ");
    for (i = 23; i >= 0; i--) 
    {
        printf("%d", (information_word >> i) & 1);
    }
    
    printf("\n");

    // Print parity bits
    printf("P1 : %d\n", !parity1);
    printf("P2 : %d\n", !parity2);
    printf("P4 : %d\n", !parity4);
    printf("P8 : %d\n", !parity8);
    printf("P16: %d\n", !parity16);

    printf(" Codeword: 0x%08X\n", codeword);
}

void decode(unsigned int codeword) 
{
    printf("Decoding: 0x%08X\n", codeword);
    
    // Calculate parity bits
    int parity1 = 0;
    int parity2 = 0;
    int parity4 = 0;
    int parity8 = 0;
    int parity16 = 0;
    
    int i;
    for (i = 1; i <= 29; i++) 
    {
        if (i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 19 || i == 21 || i == 23 || i == 25 || i == 27 || i == 29) 
        {
            // Odd bit, check for parity
            int bit = (codeword >> i) & 1;
            parity1 ^= bit;
            parity2 ^= bit;
            parity4 ^= bit;
            parity8 ^= bit;
            parity16 ^= bit;
        }
    }
    
    // Check and correct errors
    int error_location = parity1 + 2 * parity2 + 4 * parity4 + 8 * parity8 + 16 * parity16;
    int corrected_bit = (codeword >> error_location) & 1;
    
    if (error_location == 0 && corrected_bit == 0) 
    {
        printf("E1 : 0\n");
        printf("E2 : 0\n");
        printf("E4 : 0\n");
        printf("E8 : 0\n");
        printf("E16: 0\n");
        printf(" No error\n");
    } 
    
    else 
    {
        printf("E1 : %d\n", !parity1);
        printf("E2 : %d\n", !parity2);
        printf("E4 : %d\n", !parity4);
        printf("E8 : %d\n", !parity8);
        printf("E16: %d\n", !parity16);
        
        if (error_location > 0) 
        {
            printf(" Corrected bit: %d\n", error_location - 1);
            codeword ^= (1 << (error_location - 1));
        } 
        
        else 
        {
            printf(" Decoding failure: %d\n", error_location);
        }
    }
    
    // Reverse the flipping of 1's and 0's in the information word
    codeword = ~codeword;
    
    // Print information word in binary
    printf(" Information Word: ");
    for (i = 23; i >= 0; i--) 
    {
        printf("%d", (codeword >> i) & 1);
    }
    printf("\n");
}