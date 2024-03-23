#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Get the number of cents owed
    int cents = get_cents();

    // Calculate the number of quarters
    int quarters = calculate_quarters(cents);
    cents -= quarters * 25;

    // Calculate the number of dimes
    int dimes = calculate_dimes(cents);
    cents -= dimes * 10;

    // Calculate the number of nickels
    int nickels = calculate_nickels(cents);
    cents -= nickels * 5;

    // Calculate the number of pennies
    int pennies = calculate_pennies(cents);

    // Print the total number of coins
    printf("%i\n", quarters + dimes + nickels + pennies);
}

// Prompts the user for the number of cents owed
int get_cents(void)
{
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);
    return cents;
}

// Calculates the number of quarters owed
int calculate_quarters(int cents)
{
    return cents / 25;
}

// Calculates the number of dimes owed
int calculate_dimes(int cents)
{
    return cents / 10;
}

// Calculates the number of nickels owed
int calculate_nickels(int cents)
{
    return cents / 5;
}

// Calculates the number of pennies owed
int calculate_pennies(int cents)
{
    return cents;
}
