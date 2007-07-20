#include "../camera/camclient.h"

using namespace std;

int main(void)
{
    CamClient client(9034);
    int* circle_info;
    int margin = 10;
    int margin2 = 50;
    sleep(1);
    while(true) {
        circle_info = client.getCircles();
        if(circle_info[0]==2) {
            cerr<<circle_info[3]<<" "<<circle_info[6]<<endl;
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
    return 0;
}
