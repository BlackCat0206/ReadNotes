#include <stdio.h>

#define MILESTOKM	1.609
#define GALLONETOL  3.785


int main(void)
{
    float miles, gallons;
    printf("Please input your jounray miles and cost gallons:\n");
    scanf("%f %f", &miles, &gallons);
    
    float mg = miles / gallons;
    
    float kl = (gallons * GALLONETOL) / (miles * MILESTOKM);
    
    printf("mi/gal:%.1f, km/L:%.1f\n", mg, kl);
    
    return 0;
}