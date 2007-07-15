#include "socket.h"

int main(void)
{
	string alinan;
	char sayi[128];
	float sag=0,sol=0;
	try
	{
		Soket Soketim(6112);
		while (1)
		{
			Soketim.SonarOku();

			// asıl kod 
		if(Soketim.sonar[2]<0.5||Soketim.sonar[3]<0.5||Soketim.sonar[4]<0.5||Soketim.sonar[5]<0.5)
  		{
  			sag=Soketim.sonar[6]+Soketim.sonar[7]+Soketim.sonar[8]+Soketim.sonar[9];
  			sol=Soketim.sonar[1]+Soketim.sonar[0]+Soketim.sonar[15]+Soketim.sonar[14];
  		}
  
  		if(sag>0||sol>0)
 		 {
			sprintf(sayi,"%f",0.3);
 			if(sag>=sol) Soketim.Gonder("arbitrator.wspeed(0.3,"+string(sayi)+",1);\n");
			else{	sprintf(sayi,"%f",-0.3);
  		 	Soketim.Gonder("arbitrator.wspeed(0.3,"+string(sayi)+",1);\n");}
  	
  		}
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
