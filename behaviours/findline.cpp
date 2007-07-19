#include "../camera/camclient.h"
#include "socket.h"

#include <iostream>

using namespace std;

int* getMiddlePoint(int*);

int main(void)
{
    CamClient client(9034);
    Soket Hakem(6112);
    int* center;
    int* mycenter;
    int* line_info;
    int line_count;
	cerr << "findline basladi\n" << flush;
    
	cerr << "findline davrandi\n" << flush;

    while(true) {

        line_info = client.getLines();
        if(line_info[0]>0)
            {
	cerr << "findline cizgi buldu\n" << flush;
		Hakem.Davran(-0.2, 0, 10);

           /*     center= getMiddlePoint(line_info);
                mycenter= client.getInfo();
                if(mycenter[0]/2>center[0]) {       // line solda
                    Hakem.Davran(0.2, 0.2, 10);
                }
                else if(mycenter[0]/2<center[0]) {  // line sagda
                    Hakem.Davran(0.2, -0.2, 10);
                }
                else{    // ilerle
                    Hakem.Davran(0.2, 0, 10);
                }*/
             //   cout<<"line_detected";
             //   return 0;
            }
     } 
    return 0;
}

int* getMiddlePoint(int *line_info){
    int x_center,y_center;
    int x1_total=0,y1_total=0,x2_total=0,y2_total=0;
    int center[2];
    int line_count = line_info[0];
    for(int i = 1;i<=line_count;i=i+4) {
        x1_total = x1_total+line_info[i];
        y1_total = y1_total+line_info[i+1];
        x2_total = x2_total+line_info[i+2];
        y2_total = y2_total+line_info[i+3];
    }
    x_center= x1_total/4;
    y_center= y1_total/4;
    center[0]= x_center;
    center[1]= y_center;
    
}
