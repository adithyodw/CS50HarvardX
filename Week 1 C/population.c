#include <stdio.h>

int main(void)
{
    // Prompt the user for a start size
    int start_size;
    do
    {
        printf("Start size (must be at least 9): ");
        scanf("%d", &start_size);
    }
    while (start_size < 9);

    // Prompt the user for an end size
    int end_size;
    do
    {
        printf("End size (must be at least the start size): ");
        scanf("%d", &end_size);
    }
    while (end_size < start_size);

    // Calculate the number of years it will take for the population to reach the end size
    int years = 0;
    while (start_size < end_size)
    {
        int births = start_size / 3;
        int deaths = start_size / 4;
        start_size = start_size + births - deaths;
        years++;
    }

    // Print the number of years it will take for the population to reach the end size
    printf("Years: %d\n", years);

    return 0;
}
