#include "../camera/camclient.h"
#include "socket.h"

#include <iostream>

using namespace std;

int* getMiddlePoint(int*);

int main(void)
{
    CamClient client(9034);
    Soket Hakem(6112);
    int linex, framex;
    int* line_info;
    int line_count;
    float fark;

    while(true) {

        line_info = client.getLines();
        if(line_info[0]>0)
            {   

                    linex= *(getMiddlePoint(line_info));
                    framex= *(client.getInfo()) / 2;
                    fark = linex - framex;
			/*
                    if(mycenter[0]/2>center[0]) {       // line solda
                        Hakem.Davran(0.2, 0.2, 10);
                    }
                    else if(mycenter[0]/2<center[0]) {  // line sagda
                        Hakem.Davran(0.2, -0.2, 10);
                    }
                    else{    // ilerle
                        Hakem.Davran(0.2, 0, 10);
                    }
			*/
		float aci = fark * (-0.001);
		Hakem.Davran(0.2, 0 , 10);
		cerr<<linex<<" "<<framex<<" "<<aci<<", ";
            }
	else
	{
		Hakem.Davran(0.1, 0, 10);
	}
     } 
    return 0;
}

int* getMiddlePoint(int *line_info){
    int x_center,y_center;
    int x1_total=0,y1_total=0,x2_total=0,y2_total=0;
    int* center;
    center = (int *) malloc(2 * sizeof(int));
    int line_count = line_info[0];
    for(int i = 1;i<=line_count;i=i+4) {
        x1_total = x1_total+line_info[i];
        y1_total = y1_total+line_info[i+1];
        x2_total = x2_total+line_info[i+2];
        y2_total = y2_total+line_info[i+3];
    }
    x_center= (x1_total + x2_total)/(line_count * 2);
    y_center= (y1_total + y2_total)/(line_count * 2);
    center[0]= x_center;
    center[1]= y_center;
    return center;
}
