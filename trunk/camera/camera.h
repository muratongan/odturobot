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
        IplImage* frame;
    public:
        Kamera();
        ~Kamera();
        void capture();
        int* getCircles();
        int* getLines();
        
};
