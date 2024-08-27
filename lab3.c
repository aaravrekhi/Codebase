/* lab3.c
 * Aarav Rekhi
 * aaravr
 * ECE 2220, Fall 2023 
 * PP3
 * Subject: ECE2220,PP3
 *
 * Purpose:  To perform operations with two ASCII inputs and then return an output in ASCII.
 *
 * Assumptions:
 *  #1   The user is prompted to enter a pseudo arithmetic command.  The
 *       input must be verified to be grammatically correct.
 *
 *  #2:  The string and character type library cannot be used under
 *       any circumstances.  You are encouraged to develop your own
 *       functions to perform any similar operations that are needed.
 *
 *  #3   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs:
 *
 * Notes:
 *
 * See the ECE 2220 programming guide
 *
 * If your formatting is not consistent you must fix it.  You can easily
 * reformat (and automatically indent) your code using the astyle
 * command.  In a terminal on the command line do
 *
 *     astyle --style=kr lab3.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up
 * your own style.
 */

// do not include any additional libraries
#include <stdio.h>

// do not change these constants
#define MAXLINE 80
#define MAXOPER 13

// named constants and strings
enum operations {NOOP, ADD, MUL, DIV, POW};
const char *operation_str[] = {"Invalid", "+", "*", "/", "^"};

// function prototypes
int process_input(const char *input, char *op_left, char *op_right);
void calc_output(const char *op_l, int op, const char *op_r, char *result);

// do not change any code in main.  We are grading based on the format
// of the output as given in the printfs in main.
int main()
{
    char input_line[MAXLINE];
    char left_operatorerand[MAXOPER];
    char right_operatorerand[MAXOPER];
    char answer[MAXOPER];
    int  operator;

    printf("\nPP3: Arithmetic on GF(47) with + * / ^ using letters\n");
    printf("Commands:\n\tabc+bbc\n\tturtle/frog\n\ttiger^one");
    printf("\tOperands are no more than 12 letters and no spaces\n");
    printf("\tCtrl-d to quit\n\n");
    printf("> ");

    // each call to fgets collects one line of input and stores in input_line
    // BEWARE: fgets includes the end-of-line character '\n' in input_line
    while (fgets(input_line, sizeof input_line, stdin) != NULL) {

        // clear for next round
        left_operatorerand[0] = right_operatorerand[0] = answer[0] = '\0';

        // check for valid grammar
        operator = process_input(input_line, left_operatorerand, right_operatorerand);

        if (operator == ADD || operator == MUL
                || operator == DIV || operator == POW) {

            // print parsed input
            printf("'%s'", left_operatorerand);
            printf(" %s ", operation_str[operator]);
            printf("'%s' => ", right_operatorerand);

            // perform pseudo arithmetic
            calc_output(left_operatorerand, operator, right_operatorerand, answer);

            // print result
            printf("'%s'\n\n", answer);
        } else {
            printf("# %s", input_line);
        }
        printf("> ");
    }
    printf("\nGoodbye\n");
    return 0;
}

// Function to calculate the length of a string
int string_length(const char *op) 
{
    int length = 0;
    int i;

    // Iterate through the characters in the input string
    for(i = 0; op[i] != 0; i++) {
        length++;
    }
    
    return (length);
}

// Function to handle character inversion for specific character ranges
int char_invert(int character) 
{
   // Check if the character falls within the range [0, 25]
   if (character >= 0 && character <= 25) {
        character = character + 26;
    }
    // Check if the character falls within the range [25, 46]
    else if (character >= 25 && character < 47) {
        character = character - 26;
    }
    // If the character doesn't fall in the specified ranges, return it as is
    else {
        return character;
    }
    
    return character;
}

// Function to convert the result character to its ASCII representation
char result_convert(char result) 
{
    // Check if the result character falls within the range [0, 25]
    if (result < 26 && result >= 0) {
        result = result + 97; // Convert to lowercase ASCII character
    }
    // Check if the result character falls within the range [26, 51]
    else if (result > 25 && result < 52) {
        result = result + 39; // Convert to uppercase ASCII character
    }
    // If the result character doesn't fall in the specified ranges, return it as is
    else {
        return result;
    }
    
    return result;
}

/* Parse input of the form SOS where S is a string (i.e., the operand) 
 * and O (i.e., the operator) is a character.
 *
 * A string S must consist of up to 12 valid symbols a-z and A-U.
 * The operator O must be one character from: + * / ^
 * Any other characters found in the input, including spaces, are
 * grammatically incorrect and invalidate the input.
 *
 * There must be NO spaces anywhere in the input, including between
 * either SO, OS, or leading or trailing spaces.
 *
 * Input: The input string is collected using fgets.  Recall the end-of-line
 *        character is included in the input string and marks the end of
 *        the input.  This string must not be changed.
 *
 * Output: There are three outputs from this function.
 *
 *   The return value is one of NOOP, ADD, MUL, DIV, POW which are
 *      named constants.  If the input is invalid for any reason
 *      then the output must be NOOP.  Otherwise the return value
 *      corresponds to operand O.
 *
 *   If the input is grammatically correct, then two strings are also
 *   returned, one for each of the left and right operands.  If the input
 *   in invalid the two output strings are undefined.
 */
// Function to parse the input and separate operands and operator
int process_input(const char *input, char *op_left, char *op_right) 
{
    int operator;
    int input_size;
    int i = 0; 
    int k = 0;

    // Calculate the size of the input string
    input_size = string_length(input);

    // Check if the input size exceeds the maximum allowed size
    if (input_size > MAXLINE-2) {
        return NOOP;
    }

    // Parse the input string
    while (input[i] != 0) {

        // Check if the character is a lowercase letter (a-z) or an uppercase letter (A-U)
        if ((input[i] > 96 && input[i] < 123) || (input[i] > 64 && input[i] != 94 && input[i] < 86)) {
            op_left[i] = input[i];

            // Check if the left operand exceeds the maximum allowed size
            if (i > MAXOPER-2) {
                return NOOP;
            }
        }
        // Check if the character is one of the valid operators (+, *, /, ^)
        else if(input[i] == 43 || input[i] == 42 || input[i] == 47 || input[i] == 94) {
            operator = input[i];
            op_left[i] = 0;
            break;
        }
        // If the character is neither a valid operand nor operator, the input is invalid
        else {
            return NOOP;
        }
        i++;
    }

    // Continue parsing to read the right operand
    while (input[i+1] != 0) {

        // Check if the character is a lowercase letter (a-z) or an uppercase letter (A-U)
        if ((input[i+1] > 96 && input[i+1] < 123) || (input[i+1] > 64 && input[i+1] < 86)) {
            op_right[k] = input[i+1];
            k++;

            // Check if the right operand exceeds the maximum allowed size
            if (k > MAXOPER-1) {
                return NOOP;
            }
        }
        // Check if the character is an end-of-line character or null character, indicating the end of the input
        else if(input[i+1] == 10 || input[i+1] == 0) {
            op_right[k] = 0;
            break;
        }
        // If the character is neither a valid operand nor end-of-line/null character, the input is invalid
        else {
            return NOOP;
        }
        i++;
    }

    // Determine the operator based on the parsed character
    switch (operator) {
        case 43: // Addition
            return ADD;
            break;
        case 42: // Multiplication
            return MUL;
            break;
        case 47: // Division
            return DIV;
            break;
        case 94: // Power
            return POW;
            break;
        default:
            return NOOP; // Invalid operator
            break;
    }
    return NOOP; // Default to NOOP if no valid operator is found
}

/* Pseudo mathematical opertions on the two operands work as follows.
 *
 * Each character is converted to an integer in the range 1...46, where a is 0,
 * b is 1, c is 2, ..., z is 25.  The operation is then performed using
 * math on a finite field with no carries.
 *
 * If the two input strings are not the same length, then each output character
 * beyond the length of the shorter string should be a copy of the character
 * from the longer string but with the opposite case.
 *
 * Input: The two operand strings and the operator are assumed to be valid (and
 *        are verified as valid in the parse_input function).
 *
 * Output: The final string generated by the above rules is stored in the
 *         output string named result.  The input strings must not be
 *         changed.
 */
// Function to perform arithmetic operations on operands
void calc_output(const char *l_op, int op, const char *r_op, char *result)
{
    int x = 0, i = 0, j = 0;
    int w, temp, remainder, right_length, left_length, y, z, string_difference;
    char left_operator[MAXOPER]; // Translated left side
    char right_operator[MAXOPER]; // Translated right side

    // Translate the characters of the left operand to a numerical range
    while (l_op[i] != 0) {
        if (l_op[i] < 86) {
            left_operator[i] = l_op[i] - 39;
            i++;
        }
        else {
            left_operator[i] = l_op[i] - 97;
            i++;
        }
    }
    left_operator[i] = 0; // Set the last character to a null character

    // Translate the characters of the right operand to a numerical range
    while (r_op[j] != 0) {
        if (r_op[j] < 86) {
            right_operator[j] = r_op[j] - 39;
            j++;
        }
        else {
            right_operator[j] = r_op[j] - 97;
            j++;
        }
    }
    right_operator[j] = 0; // Set the last character to a null character

    // Calculate the lengths of the left and right operands
    right_length = string_length(r_op);
    left_length = string_length(l_op);

    // Determine the difference in string lengths and the case (x) of the operation
    if (right_length > left_length) {
        x += 0;
        remainder = right_length - left_length;
        string_difference = right_length;
    } 
    else if (right_length < left_length) {
        x += 1;
        remainder = left_length - right_length;
        string_difference = left_length;
    } 
    else {
        x += 2;
        string_difference = left_length;
    }

    if (op == 1) { // Addition operation
        switch (x) {
            case 2: // Same size strings
                for (w = 0; w < right_length; w++) {
                    result[w] = (left_operator[w] + right_operator[w]) % 47;
                }
                result[right_length] = 0;
                break;
            case 1: // Right operand is smaller
                for (w = 0; w < right_length; w++) {
                    result[w] = (left_operator[w] + right_operator[w]) % 47;
                }
                for (w = 0; w <= remainder; w++) {
                    result[right_length + w] = char_invert(left_operator[right_length + w]);
                }
                result[left_length] = 0;
                break;
            case 0: // Left operand is smaller
                for (w = 0; w < left_length; w++) {
                    result[w] = (left_operator[w] + right_operator[w]) % 47;
                }
                for (w = 0; w <= remainder; w++) {
                    result[left_length + w] = char_invert(right_operator[left_length + w]);
                }
                result[right_length] = 0;
                break;
        }

        // Convert the result back to its ASCII representation
        for(i = 0; i < string_difference; i++) {
            result[i] = result_convert(result[i]);
        }        
        result[i+1] = 0;
    }
    if (op == 2) { // Multiplication operation
        switch (x) {
            case 2: // same size strings
                for (w = 0; w < right_length; w++) {
                    result[w] = (left_operator[w] * right_operator[w]) % 47;
                }
                result[right_length] = 0;
                break;
            case 1: // Right operand is smaller
                for (w = 0; w < right_length; w++) {
                    result[w] = (left_operator[w] * right_operator[w]) % 47;
                }
                for (w = 0; w <= remainder; w++) {
                    result[right_length+w] = char_invert(left_operator[right_length+w]);
                }
                result[left_length] = 0;
                break;
            case 0: // Left operand is smaller
                for (w = 0; w < left_length; w++) {
                    result[w] = (left_operator[w] * right_operator[w]) % 47;
                }
                for (w = 0; w <= remainder; w++){
                    result[left_length+w] = char_invert(right_operator[left_length+w]);
                }
                result[right_length] = 0;
                break;
        }

        // Convert the result back to its ASCII representation
        for(i = 0; i < string_difference; i++) {
            result[i] = result_convert(result[i]);
        }
        result[i+1] = 0;
    }
    if (op == 3) { // Inversion operation
        switch (x) {
            case 2:     // same size strings
                for (w = 0; w < right_length; w++) {
                    if (right_operator[w] == 0) {
                        result [w] = 0;
                    }
                    for (z = 0; z < 47; z++) {
                        if ((right_operator[w] * z) % 47 == left_operator[w]) {
                            result [w] = z;
                        }
                    }        
                }
                result[right_length] = 0;
                break;
            case 1: // Right operand is smaller
                for (w = 0; w < right_length; w++) {
                    if (right_operator[w] == 0) {
                        result [w] = 0;
                    }
                    for (z = 0; z < 47; z++) {
                        if ((right_operator[w] * z) % 47 == left_operator[w]) {
                            result [w] = z;
                        }
                    }
                }
                for (w = 0; w <= remainder; w++) {
                    result[right_length+w] = char_invert(left_operator[right_length+w]);
                }
                result[left_length] = 0;
                break;
            case 0: // Left operand is smaller
                for (w = 0; w < left_length; w++) {
                    if (right_operator[w] == 0) {
                        result[w] = 0;
                    }
                    for (z = 0; z < 47; z++) {
                        if ((right_operator[w] * z) % 47 == left_operator[w]) {
                            result[w] = z;
                        }
                    }                        
                }
                for (w = 0; w <= remainder; w++) {
                    result[left_length+w] = char_invert(right_operator[left_length+w]);
                }
                result[right_length] = 0;
                break;
        }
        // Convert the result back to its ASCII representation
        for(i = 0; i < string_difference; i++) { 
            result[i] = result_convert(result[i]);
        }
        result[i+1] = 0;
    }
    if (op == 4) { // Division operation
        switch(x) {
            case 2: // same size strings
                for (w = 0; w < right_length; w++) {
                    temp = 1;
                    for (y = 0; y < right_operator[w]; y++) {
                        temp *= left_operator[w];
                        temp = temp % 47;
                    }
                    result[w] =  temp % 47;
                    if (right_operator[w] == 1) {
                        result[w] = left_operator[w];
                    }
                    if (right_operator[w] == 0) {
                        result[w] = 1;
                    }
                }
                result[right_length] = 0;
                break;
            case 1:  // Right operand is smaller
                for (w = 0; w < right_length; w++) {
                    temp = 1;
                    for (y = 0; y < right_operator[w]; y++) {
                        temp *= left_operator[w];
                        temp = temp % 47;
                    }
                    result[w] =  temp % 47;
                    if (right_operator[w] == 1) {
                        result[w] = left_operator[w];
                    }
                    if (right_operator[w] == 0) {
                        result[w] = 1;
                    }
                }
                for (w = 0; w <= remainder; w++) {
                    result[right_length+w] = char_invert(left_operator[right_length+w]);
                }
                result[left_length] = 0;
                break;
            case 0: // Left operand is smaller
                for (w = 0; w < right_length; w++) {
                    temp = 1;
                    for (y = 0; y < right_operator[w]; y++) {
                        temp *= left_operator[w];
                        temp = temp % 47;
                    }
                    result[w] =  temp % 47;
                    if (right_operator[w] == 1) {
                        result[w] = left_operator[w];
                    }
                    if (right_operator[w] == 0) {
                        result[w] = 1;
                    }
                }
                // Convert the result back to its ASCII representation
                for (w = 0; w <= remainder; w++) {
                    result[left_length+w] = char_invert(right_operator[left_length+w]);
                }
                result[right_length] = 0;
        }
        for(i = 0; i < string_difference; i++) { 
            result[i] = result_convert(result[i]);
        }
        result[i+1] = 0;
    }
}