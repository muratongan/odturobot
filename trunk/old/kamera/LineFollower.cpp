#include "LineFollower.h"

using namespace std;

//Hakem(6112)ydi...
LineFollower::LineFollower():Hakem(6112),lineCam("/dev/video0") {
}

LineFollower::~LineFollower() {
}

void LineFollower::initCamera() {
	lineCam.minimum_cozunurluk();
	lineCam.otomatik_parlaklik();
	lineCam.goruntu_yazdir();
}

bool LineFollower::determineDirection() {
	//lineCam.goruntu_yazdir();
	lineCam.otomatik_parlaklik();
	lineCam.resim_yakala();
	//iyi olabilir bu: lineCam.orta_degistir(25,0,0,10);
	
	lineFinder.readImageGraylevel((unsigned char*) lineCam.goruntuyu_al());
	if (! lineFinder.findBoundingLines())
		return false;
	lineFinder.calculateLineInfo();
	//lineFinder.printLineInfo();
	return true;
}

void LineFollower::saveCurrentConfiguration(char *filename) {
	lineCam.dosyaya_ppm_yazdir(filename);
}

double LineFollower::getSlope() {
	return lineFinder.getSlope();
}

int main(int argc, char **argv) {
	LineFollower lineFollower;
	double hiz, aci, saturatedAngle, acisal_hiz;
	lineFollower.initCamera();
	sleep(1);
	int can_start = 0;	
	int has_lost = 2;
	
	while(true) {
		if (lineFollower.determineDirection())
		{
			can_start = 1;
			if (has_lost >= 2)
				lineFollower.middle = lineFollower.lineFinder.getMiddlePoint();
			has_lost++;
			cout << "middle x: " << lineFollower.middle.x << endl;
			//cout << "slope is: " << lineFollower.getSlope() - M_PI/2 << endl;
			aci = (lineFollower.getSlope() - M_PI/2) * 180.0 / M_PI;
			if (aci < 0)
				aci *= -1;
			
			//if (aci > 45)
			//	aci = 45;

			//hiz = (0.2 / 45.0) * (45.0 - aci);
			hiz = 0.12;
			acisal_hiz = 0.2;
			saturatedAngle = lineFollower.getSlope() - M_PI/2;

			// middle point goruntunun ortasinda. egimi takip et
			if (lineFollower.middle.x > 60 && lineFollower.middle.x < 116)	
			{
				cout << "aci: " << aci << endl;
				if (aci > 30) {
					acisal_hiz = 0.4;
					hiz = 0.1;
				}

				if (saturatedAngle > 0)			
					lineFollower.Hakem.Davran(hiz, acisal_hiz, 3); //sola
				else
					lineFollower.Hakem.Davran(hiz, -1 * acisal_hiz, 3); //saga
				//cout << "angle is: " << saturatedAngle << endl;
			}

			else //middle point kenarda. ortala.
			{
				hiz = 0.12;
				acisal_hiz = 0.12;
				if (aci > 30)
					acisal_hiz = 0.4;
				
				if (lineFollower.middle.x < 88)			
					lineFollower.Hakem.Davran(hiz, acisal_hiz, 3); //sola
				else
					lineFollower.Hakem.Davran(hiz, -1 * acisal_hiz, 3); //saga	
			}
			lineFollower.prevAngle = saturatedAngle;
		}
		else
		{
			has_lost = 0;
			if (!can_start)
			{      lineFollower.saveCurrentConfiguration("control.ppm");
				cout << "Please place the robot to the correct place!" << endl;
				return 0;
			}
			acisal_hiz = 0.2;
			//hiz = 0.6
			cout << "line is out of sight" << endl;
			//if (lineFollower.prevAngle > 0)			
			if (lineFollower.middle.x < 88)	

				lineFollower.Hakem.Davran(0.12, acisal_hiz, 3); //sola
			
			else
				lineFollower.Hakem.Davran(0.12, -1 * acisal_hiz, 3); //saga
			
			
			//lineFollower.Hakem.Davran(0, 0, 5); //saga
			lineFollower.saveCurrentConfiguration("tmp.ppm");
			//return 0;
		}
		usleep(5000);
	}
	return 0;
}
