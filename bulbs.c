#include <cs50.h>
#include <stdio.h>

// Function prototype
void print_bulb(int bit);

int main(void)
{
    // Get the message from the user
    string message = get_string("Message: ");

    // Iterate through each character in the message
    for (int i = 0; message[i] != '\0'; i++)
    {
        char character = message[i];

        // Convert the character to an 8-bit binary number
        for (int j = 7; j >= 0; j--)
        {
            int bit = (character >> j) & 1; // Shift and mask to get each bit
            print_bulb(bit);
        }

        // Print a newline after each byte
        printf("\n");
    }

    return 0;
}

// Function to print a bulb (emoji) based on the bit value
void print_bulb(int bit)
{
    if (bit)
    {
        printf("🟡"); // On
    }
    else
    {
        printf("⚫"); // Off
    }
}
