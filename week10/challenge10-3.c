#include <stdio.h>

void f1()
{
    puts("f1 called");
}

void f2()
{
    puts("f2 called");
}

int main(void)
{
    void (*runf[5])(void) = {f1, f2, f1, f2, f1};

    for (int i = 0; i < 5; i++)
    {
        runf[i]();
    } 

}