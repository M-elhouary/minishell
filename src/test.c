#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    int index = 0;
    int index1 = 0;
    char *str = envp[0];
    printf("%s\n\n\n",envp[index]);
    while(envp[index])
    {
            printf("%s\n",envp[index]);
            index++;
    }
}


