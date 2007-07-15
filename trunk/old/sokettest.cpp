#include "socket.h"

int main(void)
{
	string alinan;
	try
	{
		Soket Soketim(6112);
		while (1)
		{
			Soketim.SonarOku();


			// asıl kod 
			cout<<"sonar[14]="<<Soketim.sonar[14]<<", sonar[15]="<<Soketim.sonar[15]<<endl;
			Soketim.Gonder("arbitrator.wspeed(0.4,0,1.2);\n");
			// kod bitti


			usleep(100000);
		}
		return 0;
	}
	catch (int hata)
	{
		cout<<"hata: "<<hata<<endl;
	}
}
