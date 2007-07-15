#include "socket.h"

using namespace std;

int main(void)
{
	Soket Hakem(6112);
	bool sagda_nesne_var;
	bool sagda_yakin_nesne_var;
	bool karsimda_cisim_var;
	try
	{
		while (1)
		{
			Hakem.SonarOku();

			if ((Hakem.sonar[2]<0.4)||(Hakem.sonar[3]<0.4)||(Hakem.sonar[4]<0.4)||(Hakem.sonar[5]<0.4))
			{
				Hakem.Davran(0.2,0.2,2);
			}
			else if ((Hakem.sonar[8]<0.8)||(Hakem.sonar[7]<0.8))
			{
				if ((Hakem.sonar[8]<0.5)&&(Hakem.sonar[7]>1)) Hakem.Davran(0.2,-0.2,2);
				Hakem.Davran(0.2,(0.4-min(Hakem.sonar[7],Hakem.sonar[8])),2);
/*				if ((Hakem.sonar[6]<0.3)||(Hakem.sonar[7]<0.3)) Hakem.Davran(0.2,0.2,2);
				else if ((Hakem.sonar[7]>0.5)&&(Hakem.sonar[7]>0.5)) Hakem.Davran(0.2,-0.2,2);
				else Hakem.Davran(0.4,0,2);*/
			}
			else Hakem.Davran(0.4,0,2);


			usleep(100000);
		}
	}
	catch (int i)
	{
		cout<<"Hata: "<<i<<endl;
	}
	return 0;
}

