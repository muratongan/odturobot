//opencv lib
#include <cv.h>         
#include <highgui.h>    
#include <math.h>

#include <iostream>

using namespace std;

class Kamera
{
    private:
        CvCapture* device;
    public:
        Kamera();
        ~Kamera();
        IplImage* frame;
        void capture();
        int* getCircles();
        int* getLines();
	int* getInfo();        
};
