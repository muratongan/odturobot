#include "socket.h"

using namespace std;

int main(void)
{
	Soket Hakem(6112);
	try
	{
		while (1)
		{
			Hakem.SonarOku();
			int i, minsonar=2;
			float mindeger, aci, hiz;
			mindeger=Hakem.sonar[2];
			for (i=3;i<6;i++)
			{
				if (mindeger>Hakem.sonar[i])
				{
					mindeger=Hakem.sonar[i];
					minsonar=i;
				}
			}
			switch (minsonar)
			{
				case 2: aci= 0.4; break;
				case 3: aci= 0.2; break;
				case 4: aci= -0.2; break;
				case 5: aci= -0.4; break;
			}
			hiz=0.4;
			Hakem.Davran(hiz,aci,mindeger);
			usleep(100000);
		}
	}
	catch (int i)
	{
		cout<<"Hata: "<<i<<endl;
	}
	return 0;
}

