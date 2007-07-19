#include "camera.h"

Kamera::Kamera()
{
  device = cvCaptureFromCAM( CV_CAP_ANY );
  if( !device ) {
    fprintf( stderr, "ERROR: capture is NULL \n" );
    getchar();
    throw 0;
  }
}

void Kamera::capture()
{
    frame = cvQueryFrame( device );
    if( !frame ) {
      fprintf( stderr, "ERROR: frame is null...\n" );
      getchar();
      throw 0;
    }
}

Kamera::~Kamera()
{
    cvReleaseCapture( &device );
}

int *Kamera::getCircles()
{
    int count_circle;
    int* circle_info;

    IplImage* gray = cvCreateImage( cvGetSize(frame), 8, 1 );
    CvMemStorage* storage = cvCreateMemStorage(0);
    cvCvtColor( frame, gray, CV_BGR2GRAY );
    cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 ); // smooth it, otherwise a lot of false circles may be detected
    CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->height/4, 150, 50 );
    
    count_circle=circles->total;     //number of circles in the image
    circle_info=(int*)malloc(sizeof(int)*(count_circle*3+1));
    circle_info[0]=count_circle;    
    
    int i = 0;int t=1;
    for(i = 0; i < count_circle; i++ )
        {
             float* p = (float*)cvGetSeqElem( circles, i );
             circle_info[t] = cvRound(p[0]);   // x coordinate of the circle
             circle_info[t+1] = cvRound(p[1]); // y coordinate of the circle
             circle_info[t+2] = cvRound(p[2]); // radious of the circle
             t=t+3;
             
        }
    return circle_info;
}

int *Kamera::getLines()
{
    int count_line;
    int *line_info;   
    
    IplImage* dst = cvCreateImage( cvGetSize(frame), 8, 1 );
    IplImage* color_dst = cvCreateImage( cvGetSize(frame), 8, 3 );
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    cvCanny( frame, dst, 50, 200, 3 );
    cvCvtColor( dst, color_dst, CV_GRAY2BGR );
    lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );

    count_line=lines->total;         //number of lines in the image
    line_info=(int*)malloc(sizeof(int)*(count_line*4+1));
    line_info[0]=count_line;    

    int i = 0;int t = 1;
    for( i = 0; i < MIN(lines->total,100); i++ )
        {
            float* line = (float*)cvGetSeqElem(lines,i);
            float rho = line[0];
            float theta = line[1];
            CvPoint pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            
            line_info[t] = pt1.x;     // x coordinate of the point1
            line_info[t+1] = pt1.y;   // y coordinate of the point1
            line_info[t+2] = pt2.x;   // x coordinate of the point2
            line_info[t+3] = pt2.y;   // y coordinate of the point2
            t = t+4;   
        }
    return line_info;
}

int *Kamera::getInfo()
{ 
    return cvGetSize(frame)
}
