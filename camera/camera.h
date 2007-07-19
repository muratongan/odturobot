using namespace std;

class Kamera
{
    private:
        CvCapture* capture;
        IplImage* frame;
    public:
        Kamera();
        ~Kamera();
        void capture();
        int* getCircle();
        int* getLine()
        
}
