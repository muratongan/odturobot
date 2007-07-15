#include "socket.h"

using namespace std;

Soket Hakem(6112);

void SagaDon()
{
	Hakem.Davran(0,-0.5,2);
}

void SolaDon()
{
	Hakem.Davran(0,0.5,2);
}

void DuzGit()
{
	Hakem.Davran(0.3,0,2);
}

int main(void)
{
	bool sagda_nesne_var;
	bool sagda_yakin_nesne_var;
	bool karsimda_nesne_var;
	try
	{
		while (1)
		{
			Hakem.SonarOku();

			karsimda_nesne_var=false;
			sagda_nesne_var=false;
			sagda_yakin_nesne_var=false;

			if ((Hakem.sonar[2]<0.35)|| (Hakem.sonar[3]<0.3)|| (Hakem.sonar[4]<0.3)|| (Hakem.sonar[5]<0.35)|| (Hakem.sonar[1]<0.4)|| (Hakem.sonar[6]<0.4))
				karsimda_nesne_var=true;
			if ((Hakem.sonar[7]<0.8)||(Hakem.sonar[8]<0.8))
				sagda_nesne_var=true;
			if ((Hakem.sonar[7]<0.3)||(Hakem.sonar[8]<0.3))
				sagda_yakin_nesne_var=true;

			if(karsimda_nesne_var)
			{
					SolaDon();
			}
			else if (sagda_nesne_var)
			{
				float aci;
				aci = Hakem.sonar[7]-Hakem.sonar[8];
				aci = max((float)0,aci);
				aci = 10*aci;
				aci = min((float)1,aci);
				Hakem.Davran(0.2,-aci,2);
			}
			else
			{
				DuzGit();
			}
			usleep(100000);
		}
	}
	catch (int i)
	{
		cout<<"Hata: "<<i<<endl;
	}
	return 0;
}

