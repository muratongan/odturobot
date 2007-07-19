#include "../camera/camclient.h"

using namespace std;

int main(void)
{
    CamClient client(9034);
    int* circle_info;
    while(true) {
        circle_info = client.getCircles();
        if(circle_info[0]==2) {
            cout<<"standartmarker"<<flush;
            cerr<<"standartmarker";
            return 0;
        }
        else if(circle_info[0]==3) {
            cout<<"endmarker"<<flush;
            cerr<<"endmarker";
            return 0;
        }
     } 
	sleep(1);
    return 0;
}
