
// Scrabble Game

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>

// Point values for each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Function prototype
int compute_score(string word);

int main(void)
{
    // Get words from players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Compute scores
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Determine the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }

    return 0;
}

// Function to compute the score of a word
int compute_score(string word)
{
    int score = 0;

    // Iterate through each character in the word
    for (int i = 0; word[i] != '\0'; i++)
    {
        // Check if the character is a letter
        if (isalpha(word[i]))
        {
            // Convert the letter to uppercase and calculate the score
            int index = toupper(word[i]) - 'A';
            score += POINTS[index];
        }
    }

    return score;
}
