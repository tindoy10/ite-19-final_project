#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
int romanToDecimal(char *roman);                                                    // Converts Roman numeral to decimal
void performOperation(char *roman1, char operator, char *roman2, FILE *outputFile); // Performs arithmetic
void decimalToWords(double num, char *words);                                       // Converts decimal number to words
void processFile(const char *inputFile, const char *outputFile);                    // Processes input and output files
int isValidCharacter(char ch, int isRoman);                                         // Merged validation for Roman numeral or operator

// Main function
int main() {
    char inputFile[] = "Input.txt";   // Input file name
    char outputFile[] = "Output.txt"; // Output file name

    // Process input and generate output
    processFile(inputFile, outputFile);

    printf("Processing complete. Results saved in %s.\n", outputFile);
    return 0;
}

// Function to convert Roman numeral to decimal
int romanToDecimal(char *roman) {
    int values[256] = {0};  // Initialize array to store Roman numeral values
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0;

    for (int i = 0; i < strlen(roman); i++) {
        int currentValue = values[toupper(roman[i])];
        int nextValue = (i + 1 < strlen(roman)) ? values[toupper(roman[i + 1])] : 0;

        if (currentValue < nextValue) {
            total -= currentValue; // Subtract if current < next
        } else {
            total += currentValue; // Add otherwise
        }
    }
    return total;
}


// Function to perform arithmetic operations
void performOperation(char *roman1, char operator, char *roman2, FILE *outputFile) {
    double num1 = (double)romanToDecimal(roman1);
    double num2 = (double)romanToDecimal(roman2);
    double result;

    switch (operator) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                fprintf(outputFile, "Error: Division by zero\n");
                return;
            }
            break;
        default:
            fprintf(outputFile, "Error: Invalid operator '%c'\n", operator);
            return;
    }

    char words[512];
    decimalToWords(result, words);

    // Write only the words result to the output file
    fprintf(outputFile, "%s\n", words);
}

// Function to convert decimal number to words
void decimalToWords(double num, char *words) {
    // Handle the special case for zero
    if (num == 0) {
        strcpy(words, "Zero");
        return;
    }

    // Handle negative numbers
    char negativeBuffer[512] = "";
    if (num < 0) {
        strcat(negativeBuffer, "Negative ");
        num = -num; // Make the number positive for further processing
    }

    char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};

    char buffer[512] = "";

    // Convert the integer part of the number
    int integerPart = (int)num;
    if (integerPart >= 1000) {
        strcat(buffer, units[integerPart / 1000]);
        strcat(buffer, " Thousand ");
        integerPart %= 1000;
    }
    if (integerPart >= 100) {
        strcat(buffer, units[integerPart / 100]);
        strcat(buffer, " Hundred ");
        integerPart %= 100;
    }
    if (integerPart >= 20) {
        strcat(buffer, tens[integerPart / 10]);
        strcat(buffer, " ");
        integerPart %= 10;
    } else if (integerPart >= 10) {
        strcat(buffer, teens[integerPart - 10]);
        strcat(buffer, " ");
        integerPart = 0;
    }
    if (integerPart > 0) {
        strcat(buffer, units[integerPart]);
        strcat(buffer, " ");
    }

    // Combine the negative prefix with the number in words
    strcat(negativeBuffer, buffer);
    strcpy(words, negativeBuffer);  // Copy the result to the output string
}

// Function to process input and output files
void processFile(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");

    if (!input || !output) {
        printf("Error: Unable to open input or output file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) {
        char roman1[64], roman2[64];
        char operator;

        if (sscanf(line, "%s %c %s", roman1, &operator, roman2) == 3) {
            // Validate Roman numerals and operator
            if (isValidCharacter(roman1[0], 1) && isValidCharacter(roman2[0], 1) && isValidCharacter(operator, 0)) {
                performOperation(roman1, operator, roman2, output);
            } else {
                fprintf(output, "Invalid input: %s", line);
            }
        } else {
            fprintf(output, "Invalid input: %s", line);
        }
    }

    fclose(input);
    fclose(output);
}

// Merged validation function for Roman numerals and operators
int isValidCharacter(char ch, int isRoman) {
    if (isRoman) {
        // Check for valid Roman numeral symbols
        return (ch == 'I' || ch == 'V' || ch == 'X' || ch == 'L' || ch == 'C' || ch == 'D' || ch == 'M');
    } else {
        // Check for valid arithmetic operators
        return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
    }
}