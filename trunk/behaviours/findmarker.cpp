#include "../camera/camclient.h"

using namespace std;

int main(void)
{
    CamClient client(9034);
    int* circle_info;
    int margin = 5;
    int margin2 = 10;
    
    while(true) {
        circle_info = client.getCircles();
        if(circle_info[0]==2) {
            if((circle_info[3]-circle_info[6])<=margin && (circle_info[3]-circle_info[6])>=-margin)
               { 
                    cout<<"standartmarker"<<flush;
                    cerr<<"standartmarker";
                    return 0;
                }
            else if((circle_info[3]-circle_info[6])<=margin2 && (circle_info[3]-circle_info[6])>=-margin2)
                {
                    cout<<"endmarker"<<flush;
                    cerr<<"endmarker";
                    return 0;
                }
         }
     } 
	sleep(1);
    return 0;
}
