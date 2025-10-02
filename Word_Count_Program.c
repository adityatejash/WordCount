// Word Count Program

#include <stdio.h>
#include <ctype.h>
#include <string.h>


// Function prototypes
int Count_Words(char *text);
int Count_Characters(char *text);
int Count_Sentences(char *text);
int Count_Spaces(char *text);
int Count_Lines(char *text);


int main() 
{
    char text[1000];
    printf("\nEnter your text (press Enter twice to finish):\n");
    
    char input[100];
    text[0] = '\0';
    
    // Read multiple lines of input
    while (fgets(input, sizeof(input), stdin)) 
    {
        if (input[0] == '\n') 
            break; // Stop on an empty line
        strcat(text, input); // Append to the main text
    }

    // Display the results
    printf("\nAnalysis of the text:\n");
    printf("Total Words: %d\n", Count_Words(text));
    printf("Total Characters: %d\n", Count_Characters(text));
    printf("Total Sentences: %d\n",Count_Sentences(text));
    printf("Total Spaces: %d\n", Count_Spaces(text));
    printf("Total Lines: %d\n", Count_Lines(text));

    return 0;
}


// Function to count words
int Count_Words(char *text) 
{
    int i, count = 0, flag = 0;
    for (i = 0; text[i] != '\0'; i++) 
    {
        if (isspace(text[i])) 
            flag = 0;
        else if (flag == 0) 
        {
            flag = 1;
            count++;
        }
    }
    return count;
}


// Function to count characters
int Count_Characters(char *text) 
{
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++)
        count++;
    return count;
}


// Function to count spaces
int Count_Spaces(char *text) 
{
    int count = 0;
    for (int i = 0; text[i] != '\0'; i++) 
    {
        if (isspace(text[i]) && text[i] != '\n')
            count++;
    }
    return count;
}


// Function to count sentences
int Count_Sentences(char *text) 
{
    int count = 0; // Start with 1 for the first line
    for (int i = 0; text[i] != '\0'; i++) 
    {
        if (text[i] == '.')
            count++;
    }
    return count;
}


// Function to count lines
int Count_Lines(char *text) 
{
    int count = 0; // Start with 1 for the first line
    for (int i = 0; text[i] != '\0'; i++) 
    {
        if (text[i] == '\n')
            count++;
    }
    return count;
}