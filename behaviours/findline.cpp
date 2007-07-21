#include "../camera/camclient.h"
#include "socket.h"

#include <iostream>

using namespace std;

int* getMiddlePoint(int*);

int main(void)
{
    CamClient client(40000);
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
		    float aci = fark * (-0.0025);
		    if(linex>=0 && linex<=framex*2 )
		        Hakem.Davran(0.15, aci , 10);
		    //else Hakem.Davran(0.1, 0 , 10);
		    cerr<<"LİNES"<<line_info[0]<<" LN:"<<linex<<" FR:"<<framex<<" ACI:"<<aci<<endl;
        }
	    else
	    {
		    Hakem.Davran(0.15, 0, 10);
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
    for(int i = 0;i<=line_count;i++) {
//        if (line_info[4*i+2]>line_info[4*i+4])
            x1_total += line_info[4*i+1];
//        else
            x1_total += line_info[4*i+3];
    }
    x_center= (x1_total)/(line_count*2);
    center[0]= x_center;
    center[1]= 0;
    return center;
}
