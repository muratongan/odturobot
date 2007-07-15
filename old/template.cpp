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
			// Sonarlara Erişim: Hakem.sonar[0] .. Hakem.sonar[15]
			Hakem.Davran(0,0,0); // Parametreler 1: hız, 2: açı, 3: öncelik
			usleep(100000);
		}
	}
	catch (int i)
	{
		cout<<"Hata: "<<i<<endl;
	}
	return 0;
}

