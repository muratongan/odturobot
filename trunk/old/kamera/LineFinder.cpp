#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "LineFinder.h"

using namespace std;

/*-----------point class----------*/
point::point() {
	x = 0;
	y = 0;
}

Line::Line() {
	xIntercept = 0;
	yIntercept = 0;
	direction = NO_DIRECTION;
}


/*---------line finder class---------*/
LineFinder::LineFinder() {
	slope = 0;
	horzCoordsCount=0;
	vertCoordsCount=0;

	for(int i=0, searchIndex=0; searchIndex < HEIGHT/2; searchIndex += MIN_BAND_WIDTH, i+=2)
	{
		horz_coords[i] = searchIndex * 2;
	    horz_coords[i+1] = 	HEIGHT - searchIndex * 2 - 1;
		horzCoordsCount++;
	}
	
	for(int i=0, searchIndex=0; searchIndex < WIDTH/2; searchIndex += MIN_BAND_WIDTH, i+=2)
	{
		vert_coords[i] = searchIndex * 2;
	    vert_coords[i+1] = 	WIDTH - searchIndex * 2- 1;
		vertCoordsCount++;
	}
	
	//horizontal - vertical 
	//alt yarim
		oblique_coords[0] = HEIGHT - 1;
	    oblique_coords[1] = WIDTH - 1;
		
		oblique_coords[2] = HEIGHT - 1;
	    oblique_coords[3] = 0;
		
		oblique_coords[4] = HEIGHT - 20 - 1;
	    oblique_coords[5] = WIDTH - 20 - 1;
		
		oblique_coords[6] = HEIGHT - 20 - 1;
	    oblique_coords[7] = 20 - 1;
		
		oblique_coords[8] = HEIGHT - 40 - 1;
	    oblique_coords[9] = WIDTH - 40 - 1;
		
		oblique_coords[10] = HEIGHT - 40 - 1;
	    oblique_coords[11] = 40 - 1;

	//ust yarim	
		oblique_coords[12] = 0;
	    oblique_coords[13] = WIDTH - 1;
		
		oblique_coords[14] = 0;
	    oblique_coords[15] = 0;
		
		oblique_coords[16] = 20 - 1;
	    oblique_coords[17] = WIDTH - 20 - 1;
		
		oblique_coords[18] = 20 - 1;
	    oblique_coords[19] = 20 - 1;
		
		oblique_coords[20] = 40 - 1;
	    oblique_coords[21] = WIDTH - 40 - 1;
		
		oblique_coords[22] = 40 - 1;
	    oblique_coords[23] = 40 - 1;
		
		obliqueCoordsCount = 24;	
}



void LineFinder::loadImage(char *filename) {
	char dummychar;
	int dummyint;
	unsigned char red, green, blue;

	ifstream imageFile(filename);
	imageFile >> dummychar;	//P
	imageFile >> dummyint; //6
	imageFile >> dummyint; //640
	imageFile >> dummyint; //480
	imageFile >> dummyint; //255

	int position = imageFile.tellg();
	imageFile.close();
	
	imageFile.open(filename, ios::binary);
	imageFile.seekg(position);
	for(int i=0; i<HEIGHT; i++)
		for(int j=0; j<WIDTH; j++)
		{
			imageFile >> red >> green >> blue;
			image[i][j] = (red + green + blue) / 3;
		}
}

void LineFinder::readImageGraylevel(unsigned char *resim) {
	for(int i=0; i<HEIGHT; i++)
		for(int j=0; j<WIDTH; j++)
		{
			image[i][j] = (int)(resim[i*3*WIDTH + 3*j] 
					+ resim[i*3*WIDTH + 3*j + 1] 
					+ resim[i*3*WIDTH + 3*j + 2]) / 3;
			//printf("%u\n", resim[0]);
			//printf("%u\n", resim[1]);
			//printf("%u\n", resim[2]);
			//cout << resim[0] + resim[1] + resim[2]<< endl;
			//cout << image[0][0] << endl;
			//exit(0);
	
		}
/*
	fstream outfile("resim.ppm", ios::out);
	for (int i=0; i<HEIGHT; i++)
		for(int j=0; j<WIDTH; j++)
		{
			outfile << image[i][j] << endl;
		}
	exit(0);
*/
}

//bu line'i direction'ini ve x ya da yInterceptini set edip gondermek gerekiyor.
int LineFinder::countRegions(Line &line) {
	int currentColor = 0; //0: white	1: black	
	int currentRegionWidth = 0;
	int last = (line.direction == HORZ) ? WIDTH : HEIGHT;
	int regionCount = 1;
	int searchIndex;
	int tmpColor;
	int tmpIndex;
	int floor_flag = 0;

	for(int i=0; i<last; i++) {
		floor_flag = 0;
		//cout << currentRegionWidth << endl;
		//printf("%d\n", image[searchIndex][i]);
		if (line.direction == HORZ)
		{
			tmpColor = image[line.yIntercept][i] < THRESHOLD;
			if (image[line.yIntercept][i] < THRESHOLD_HIGH && image[line.yIntercept][i] > THRESHOLD_LOW)
			{
				floor_flag = 1;
			}
		}
		else
		{
			tmpColor = image[i][line.xIntercept] < THRESHOLD;
			if (image[i][line.xIntercept] < THRESHOLD_HIGH && image[i][line.xIntercept] > THRESHOLD_LOW)
				floor_flag = 1;
		}
			
		if (currentColor == tmpColor /*|| floor_flag == 1*/)
		{
			currentRegionWidth++;
		}
		else
		{
			//cout << "found a different region" << endl;
			currentColor = !currentColor;
			if (currentRegionWidth > MIN_BAND_WIDTH)
			{
				//cout << "wide" << endl;
				regionCount++;
				//exit(0);
			}
			else
			{
				//cout << "not wide" << endl;	
				//cout << "found: " << currentRegionWidth << endl;
				//cout << "expected: " << MIN_BAND_WIDTH << endl;
			}
			if (regionCount == 2)
			{
				tmpIndex = i;
			}
			if (regionCount >= 3)
			{
				if (line.direction == HORZ)
				{
					line.xIntercept = -1;
				    line.start.x = tmpIndex;
				    line.start.y = line.yIntercept;
				    line.end.x = i;
				    line.end.y = line.yIntercept;
				}
				else
				{
					line.yIntercept = -1;
				    line.start.x = line.xIntercept;
				    line.start.y = tmpIndex;
				    line.end.x = line.xIntercept;
				    line.end.y = i;
				}
			}
			currentRegionWidth = 0;
		}
		
	}

	cout << "countRegions found " << regionCount - 2 << " regions" << endl;	
	return regionCount - 2; //bastaki ve sondaki beyaz regionlari at.
}

bool LineFinder::findBoundingLines() { 
	 for(int i=0; i < horzCoordsCount || i < vertCoordsCount || i < obliqueCoordsCount; i+=2)
	 {
		 if (i < horzCoordsCount) //horizontal - horizontal
		 {
		     firstLine.direction = HORZ;
			 firstLine.yIntercept = horz_coords[i];
		     secondLine.direction = HORZ;
			 secondLine.yIntercept = horz_coords[i+1];
			 
			 if (countRegions(firstLine) >= BAND_COUNT
					 && countRegions(secondLine) >= BAND_COUNT)
				{
					/*
					cout << "horizontal" << endl;
		 			cout << "first line at: " << firstLine.yIntercept << endl; 
		 			cout << "second line at: " << secondLine.yIntercept << endl; 
					*/
					return true;
				}
		 }
	
		 if (i < vertCoordsCount) //vertical - vertical
		 { 
			 firstLine.direction = VERT;
			 firstLine.xIntercept = vert_coords[i];
			 secondLine.direction = VERT;
			 secondLine.xIntercept = vert_coords[i+1];
			 
			 if (countRegions(firstLine) >= BAND_COUNT
					 && countRegions(secondLine) >= BAND_COUNT)
			 	{
					/*
					cout << "vertical" << endl;
					cout << "first line at: " << firstLine.xIntercept << endl; 
					cout << "second line at: " << secondLine.xIntercept << endl; 
					*/
					return true;
				 }
		}

		//go oblique
		 if (i < obliqueCoordsCount) //horizontal - vertical
		 { 
			 firstLine.direction = HORZ;
			 firstLine.yIntercept = oblique_coords[i];
			 secondLine.direction = VERT;
			 secondLine.xIntercept = oblique_coords[i+1];
			 
			 if (countRegions(firstLine) >= BAND_COUNT
					 && countRegions(secondLine) >= BAND_COUNT)
			 	{
					/*
					cout << "oblique" << endl;
					cout << "first line (horizontal) at: " << firstLine.yIntercept << endl; 
					cout << "second line (vertical) at: " << secondLine.xIntercept << endl; 
					cout << "i: " << i << endl;
					*/
					return true;
				 }
		}

	}	 
	//did i find two lines? error if not.
	//cout << "I couldn't find two suitable regions." << endl;
	return false; 
}

void LineFinder::printPointCoords() {
	cout << "-------points--------" << endl;
	cout << "first line (start point) :\tx: " << firstLine.start.x << endl;
	cout << "            	          \ty: " << firstLine.start.y << endl;
	cout << "first line (end point) :\tx: " << firstLine.end.x << endl;
	cout << "                        \ty: " << firstLine.end.y << endl;
	cout << "second line (start point) :\tx: " << secondLine.start.x << endl;
	cout << "                           \ty: " << secondLine.start.y << endl;
	cout << "second line (end point) :\tx: " << secondLine.end.x << endl;
	cout << "                         \ty: " << secondLine.end.y << endl;
}

void LineFinder::printLineInfo() {
	cout << "-------line info--------" << endl;
	cout << "slope(degrees): " << (slope * 180) / M_PI << endl;
	cout << "middle point:\tx: " << middlePoint.x << endl;
	cout << "             \ty: " << middlePoint.y << endl;
}

double LineFinder::getSlope() {
	return slope;
}

point LineFinder::getMiddlePoint() {
	return middlePoint;
}

void LineFinder::calculateLineInfo() {
	point firstLinesMiddle, secondLinesMiddle;
	
	middlePoint.x = (firstLine.start.x + firstLine.end.x + secondLine.start.x + secondLine.end.x) / 4;
	middlePoint.y = (firstLine.start.y + firstLine.end.y + secondLine.start.y + secondLine.end.y) / 4;
	
	firstLinesMiddle.x = (firstLine.start.x + firstLine.end.x) / 2;
	firstLinesMiddle.y = HEIGHT - (firstLine.start.y + firstLine.end.y) / 2;
	secondLinesMiddle.x = (secondLine.start.x + secondLine.end.x) / 2;
	secondLinesMiddle.y = HEIGHT - (secondLine.start.y + secondLine.end.y) / 2;


	/*cout << firstLinesMiddle.x << endl;
	cout << firstLinesMiddle.y << endl;
	cout << secondLinesMiddle.x << endl;
	cout << secondLinesMiddle.y << endl;
*/
	//y ters cunku +y ekseninde 0 var.
	int tmpx, tmpy;

	if (secondLinesMiddle.y > firstLinesMiddle.y)
	{
		tmpy = secondLinesMiddle.y - firstLinesMiddle.y;
		tmpx = secondLinesMiddle.x - firstLinesMiddle.x;
	}
	else
	{
		tmpy = firstLinesMiddle.y - secondLinesMiddle.y;
		tmpx = firstLinesMiddle.x - secondLinesMiddle.x;
	}
/*	
	cout << tmpy << endl;
	cout << tmpx << endl;
*/
	slope = atan2(tmpy, tmpx); 
}

/*
int main(int argc, char **argv) {
	LineFinder lineFinder; 
	lineFinder.loadImage(argv[1]);
	//cout << "region count is " << lineFinder.countRegions(HORZ, HORZ_UPMID) << endl;
	int direction;
	if (!lineFinder.findBoundingLines())
	{
		return 0;
	}

	lineFinder.printPointCoords();
	
	lineFinder.calculateLineInfo();
	lineFinder.printLineInfo();
	return 0;
}
*/
