#include <stdio.h>
int main()
{

    float num1, num2;
    int x;
    printf("please enter two numbers to do some of mathematical operation \n");
    scanf("%f%f", &num1, &num2);

    printf("if you want to calculate sum enter 1 \n ");
    printf("if you want to calculate subtraction enter 2 \n ");
    printf("if you want to calculate multiblication enter 3 \n ");
    printf("if you want to calculate division enter 4 \n");

    scanf("%d", &x);

    switch (x)
    {
    case 1:
        printf(" the sumation of two numbers equal to %f \n", num1 + num2);
        break;

    case 2:
        printf(" the subtraction of two numbers equal to %f \n", num1 - num2);
        break;
    case 3:
        printf(" the multiplication of two numbers equal to %f \n", num1 * num2);
        break;
    case 4:
        printf(" the division of two numbers equal to %f \n", num1 / num2);
        break;
    default:
        printf("please enter the right number \n");
    }

    return 0;
}