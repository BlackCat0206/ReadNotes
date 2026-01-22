
#include <stdio.h>

int main(void)
{
    float cups;
    printf("Please input cups:");
    scanf("%f", &cups);
    
    float pints, ounces, tablespoons, teaspoons;
    
    pints = cups / 2.00f;
    ounces = cups * 8.00f;
    tablespoons = ounces * 2e0f;
    teaspoons = tablespoons * 3e0f;
    
    printf("%.2f(c) is %.2f(pt), %.2f(oz), %.2f(tbsp), %.2f(tsp)", cups, pints, ounces, tablespoons, teaspoons);
    
    return 0;
}