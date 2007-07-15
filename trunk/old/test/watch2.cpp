#include <libplayerc++/playerc++.h>
#include <iostream>
#include <string>

using namespace PlayerCc;

// Robot Değişkenleri
std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);
uint         gPort(PlayerCc::PLAYER_PORTNUM);
uint         gIndex(0);
uint         gDebug(0);
uint         gFrequency(10); // Hz
uint         gDataMode(PLAYER_DATAMODE_PUSH);
bool         gUseLaser(false);

// Program Değişkenleri
double yakin=0.3;
double hiz=0.3;
double aci=0;

double yenihiz;
double yeniaci;

// Ana Fonksiyon
int main()
{
	try
	{
		// Robotu Yarat
		PlayerClient robot (gHostname, gPort);
		Position2dProxy konum (&robot, gIndex);
		SonarProxy sonar (&robot, gIndex);
		std::cout << robot << std::endl;
		konum.SetMotorEnable (true);
		
		// Karar Döngüsü
   		while (1)
		{
			// Sonarları Oku
			robot.Read();

			// Karar Al
			yenihiz=hiz;
			yeniaci=aci;

			// İleri Giderken Önünde Bir Şey Varsa Hızı Sıfırla
			if (yenihiz>0)
			{
				if (sonar[2]<yakin||sonar[3]<yakin||sonar[4]<yakin||sonar[5]<yakin)
				{
					yenihiz=0;
				}
			}

			// Geri Giderken Arkanda Bir Şey Varsa Hızı Sıfırla
			if (yenihiz<0)
			{
				if (sonar[11]<yakin||sonar[12]<yakin||sonar[13]<yakin||sonar[14]<yakin)
				{
					yenihiz=0;
				}
			}

			// Aldığın Kararları Uygula
			konum.SetSpeed(yenihiz,yeniaci);
		}
	}
	catch (PlayerCc::PlayerError e)
	{	
		std::cerr << e << std::endl;	
		return -1;
	}
	catch (std::string hata)
	{
		std::cerr<<hata<<std::endl;
		return -1;
	}
}
