#include <iostream>
#include <fstream>
#include <math.h>

#include "LineFinder.h"
#include "socket.h"
#include "kamera.h"

using namespace std;

class LineFollower{
	public:
		LineFollower();
		~LineFollower();
		bool determineDirection();
		void saveCurrentConfiguration(char *);
		void initCamera();
		double getSlope();
		Soket Hakem;
		double prevAngle;
		kamera lineCam;
		point middle;
		LineFinder lineFinder;
};
