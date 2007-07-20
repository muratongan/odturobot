#include "socket.h"

using namespace std;

int main(void)
{
	static float takip_mesafesi=0.6;
	static float hata_payi=0.02;
	static float oncelik=3;
	static float aci=0.2;
	static float hiz=0.4;
	static int bekleme=100000;
	Soket Hakem(6112);
	try
	{
		while (1)
		{
			Hakem.SonarOku();
			
			if ((Hakem.sonar[7]+Hakem.sonar[8])/2<takip_mesafesi) // Eğer Sağa yakınsam
			{
				if ((Hakem.sonar[3]+Hakem.sonar[4])/2<takip_mesafesi) // Karşıya da yakınsam
				{
					Hakem.Davran(0,-aci,oncelik);
				}
				else
				{
					if ((Hakem.sonar[7]-Hakem.sonar[8])>hata_payi)
					{
						Hakem.Davran(0,-aci,oncelik);
					}
					else if ((Hakem.sonar[7]-Hakem.sonar[8])<(-hata_payi))
					{
						Hakem.Davran(0,aci,oncelik);
					}
					else
					{
						Hakem.Davran(hiz,0,oncelik);
					}
				}
			}
			else if ((Hakem.sonar[0]+Hakem.sonar[15])/2<takip_mesafesi) // Eğer Sola yakınsam
			{
				if ((Hakem.sonar[3]+Hakem.sonar[4])/2<takip_mesafesi) // Karşıya da yakınsam
				{
					Hakem.Davran(0,aci,oncelik);
				}
				else
				{
					if ((Hakem.sonar[0]-Hakem.sonar[15])>hata_payi)
					{
						Hakem.Davran(0,aci,oncelik);
					}
					else if ((Hakem.sonar[0]-Hakem.sonar[15])<(-hata_payi))
					{
						Hakem.Davran(0,aci,oncelik);
					}
					else
					{
						Hakem.Davran(hiz,0,oncelik);
					}
				}
			}
			else
			{
				if ((Hakem.sonar[3]+Hakem.sonar[4])/2<takip_mesafesi) // Karşıya yakınsam
				{
					if ((Hakem.sonar[3]-Hakem.sonar[4])>hata_payi)
					{
						Hakem.Davran(0,aci,oncelik);
					}
					else if ((Hakem.sonar[3]-Hakem.sonar[4])<(-hata_payi))
					{
						Hakem.Davran(0,-aci,oncelik);
					}
					else
					{
						Hakem.Davran(-hiz,0,oncelik);
					}
				}
				else Hakem.Davran(hiz,0,oncelik);
			}
			usleep(bekleme);
		}
	}
	catch (int i)
	{
		cout<<"Hata: "<<i<<endl;
	}
	return 0;
} 
