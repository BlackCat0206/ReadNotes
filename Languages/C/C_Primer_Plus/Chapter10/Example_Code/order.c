/* order.c -- precedence in pointer operations */
#include <stdio.h>
int data[2] = {100, 200};
int moredata[2] = {300, 400};
int main(void)
{
    int * p1, * p2, * p3;
    
    p1 = p2 = data;
    p3 = moredata;
    printf("p1 = %p, p2 = %p, p3 = %p\n", p1, p2, p3);
    printf("  *p1 = %d,   *p2 = %d,     *p3 = %d\n",
           *p1     ,   *p2     ,     *p3);
    printf("*p1++ = %d, *++p2 = %d, (*p3)++ = %d\n",
           *p1++     , *++p2     , (*p3)++);
    printf("  *p1 = %d,   *p2 = %d,     *p3 = %d\n",
           *p1     ,   *p2     ,     *p3);
    printf ("p1 = %p, p2 = %p, p3 = %p\n", p1, p2, p3);
    
    for (int i = 0; i < 2; i++)
        printf("data[%d] = %d; moredata[%d] = %d\n",
               i, data[i], i, moredata[i]);


    return 0;
}
