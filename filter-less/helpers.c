#include "helpers.h"
#include <math.h>
// Prototype of the last function
int C255(int color);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float avrgColors;
    int orgR, orgG, orgB;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            orgR = image[i][j].rgbtRed;
            orgG = image[i][j].rgbtGreen;
            orgB = image[i][j].rgbtBlue;
            // Find the average colors
            avrgColors = round((orgR + orgG + orgB) / 3.0);

            image[i][j].rgbtRed = avrgColors;
            image[i][j].rgbtGreen = avrgColors;
            image[i][j].rgbtBlue = avrgColors;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int orgR, orgG, orgB, sphR, sphG, sphB;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Value the sph variables and then set in the pixels
            // Using the said formula
            sphB = C255(round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue));
            sphG = C255(round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue));
            sphR = C255(round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue));

            image[i][j].rgbtBlue = sphB;
            image[i][j].rgbtGreen = sphG;
            image[i][j].rgbtRed = sphR;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_row[width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp_row[j] = image[i][j];
        }
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp_row[width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = temp_row[width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = temp_row[width - 1 - j].rgbtBlue;
        }
    }
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            temp_image[h][w] = image[h][w];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int count = 0;
            float sumR = 0;
            float sumG = 0;
            float sumB = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // Check if pixel is outside rows
                    if (i + k < 0 || i + k >= height)
                    {
                        continue;
                    }
                    // Check if pixel is outside columns
                    if (j + l < 0 || j + l >= width)
                    {
                        continue;
                    }
                    // Otherwise add to sums
                    sumR += temp_image[i + k][j + l].rgbtRed;
                    sumG += temp_image[i + k][j + l].rgbtGreen;
                    sumB += temp_image[i + k][j + l].rgbtBlue;
                    count++;
                }
            }

            image[i][j].rgbtRed = (int) round(sumR / count);
            image[i][j].rgbtGreen = (int) round(sumG / count);
            image[i][j].rgbtBlue = (int) round(sumB / count);
        }
    }
}

// If it reaches 255, it will return 255
int C255(int color)
{
    if (color > 255)
    {
        return 255;
    }
    else
    {
        return color;
    }
}
