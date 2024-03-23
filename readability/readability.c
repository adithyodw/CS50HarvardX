#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

// Function prototypes
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int compute_index(int letters, int words, int sentences);

int main(void)
{
    // Get input text from the user
    string text = get_string("Text: ");

    // Count letters, words, and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Compute the Coleman-Liau index
    int index = compute_index(letters, words, sentences);

    // Print the grade level
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

    return 0;
}

// Function to count letters in a text
int count_letters(string text)
{
    int count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }
    }

    return count;
}

// Function to count words in a text
int count_words(string text)
{
    int count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isspace(text[i]))
        {
            count++;
        }
    }

    // Add one to account for the last word
    return count + 1;
}

// Function to count sentences in a text
int count_sentences(string text)
{
    int count = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }

    return count;
}

// Function to compute the Coleman-Liau index
int compute_index(int letters, int words, int sentences)
{
    // Calculate average letters per 100 words
    float L = (float) letters / words * 100;

    // Calculate average sentences per 100 words
    float S = (float) sentences / words * 100;

    // Coleman-Liau index formula
    return round(0.0588 * L - 0.296 * S - 15.8);
}
