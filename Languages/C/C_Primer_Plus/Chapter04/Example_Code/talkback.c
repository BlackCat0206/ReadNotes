// talkback.c -- nosy, informative program
#include <stdio.h>
#include <string.h>      // for strlen() prototype
#define DENSITY 62.4     // human density in lbs per cu ft
#define Q "His Hamlet was funny without being vulgar."
int main()
{
    float weight, volume;
    int size, letters;
    char name[40];        // name is an array of 40 chars
    
    printf("Hi! What's your first name?\n");
    scanf("%s", name);
    printf("%s, what's your weight in pounds?\n", name);
    scanf("%f", &weight);
    size = sizeof name;
    letters = strlen(name);
    volume = weight / DENSITY;
    printf("Well, %s, your volume is %2.2f cubic feet.\n",
           name, volume);
    printf("Also, your first name has %d letters,\n",
           letters);
    printf("and we have %d bytes to store it.\n", size);
    
    printf("Is %2.2e the same as %2.2f?\n", 1201.0, 1201.0);

    printf("\"%s\"\n has %d characters.\n",  Q, strlen(Q));

    return 0;
}
