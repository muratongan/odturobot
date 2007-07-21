#include "../camera/camclient.h"

#include <iostream>

using namespace std;

int main(void)
{
    int* line_info;
    CamClient client(9034);
    while(true) {
        line_info = client.getLines();
     } 
}
