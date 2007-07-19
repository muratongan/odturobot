#include "camclient.h"

int main(void)
{
    client = Soket(9034);
    int * array;
    array = client.getLines();
    printf("%d\n", array[0]);
    for (int i=1; i<array[0]+1; i++)
    {
        printf("%d ", array[i]);
    }
    printf("%d\n");
    return 0;
}

