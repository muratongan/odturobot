#ifndef _LINEFINDER_H
#define _LINEFINDER_H

#include <iostream>
#include <string>

using namespace std;

#define HEIGHT 144
#define WIDTH 176

#define BAND_COUNT 4
#define MIN_BAND_WIDTH 10

#define HORZ 0
#define VERT 1
#define NO_DIRECTION (-1)

#define THRESHOLD 150
#define THRESHOLD_HIGH 220
#define THRESHOLD_LOW 150

#define getColor(direction, i, searchIndex) ((direction == VERT) ? image[i][searchIndex] < THRESHOLD : image[searchIndex][i] < THRESHOLD)

class point {
	public:
		int x;
		int y;
		point();
};

class Line {
	public:
		int xIntercept;
		int yIntercept;
		int direction;
		point start;
		point end;
		Line();
};

class LineFinder {
	private:
		double slope;
		point middlePoint;
		
	 	int image[HEIGHT][WIDTH];
		Line firstLine, secondLine;

		int horz_coords[500];
		int vert_coords[500];	
		int oblique_coords[500];	
		
		int horzCoordsCount;
		int vertCoordsCount;
		int obliqueCoordsCount;
		
	public:
		LineFinder();
		double getSlope();
		point getMiddlePoint();

		void loadImage(char *filename);
		void readImageGraylevel(unsigned char *);
		bool findBoundingLines();
		void printPointCoords();
		void printLineInfo();
		void calculateLineInfo();
		int countRegions(Line &);
};

#endif

