#include <stdio.h>

void Temperatures(double f_temp)
{
    const double f_c_rate1 = 5.0;
    const double f_c_rate2 = 9.0;
    const double f_c_offset = 32.0;

    const double c_k_offset = 273.16;

    double c_temp = (f_c_rate1 / f_c_rate2) * (f_temp - f_c_offset);
    double k_temp = c_temp + c_k_offset;

    printf("Fahrenheit temperature: %.2f\n", f_temp);
    printf("Celsius temperature: %.2f\n", c_temp);
    printf("Kelvin temperature: %.2f\n", k_temp);

    return;
}

int main(void)
{
    double f_temp;

    printf("Please input a Fahrenheit temperature: ");
    
    while (1 == scanf("%lf", &f_temp))
    {
        Temperatures(f_temp);
        printf("Please input a Fahrenheit temperature (q to quit): ");
    }
    printf("Done!\n");
    return 0;
}