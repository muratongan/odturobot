#include "camclient.h"

int main(void)
{
    Soket client(9034);
    int * array;
    while (1)
    {
        array = client.getCircles();
        for (int i=1; i<array[0]*3+1; i++)
        {
            printf("%d ", array[i]);
        }
        if (array[0]>0) printf("\n");
    }
    return 0;
}

