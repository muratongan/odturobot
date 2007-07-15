#include <libplayerc++/playerc++.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>

#define PORT 7777

#define soket_olusturulamadi 1
#define port_kullanilamiyor 2
#define soket_baglanamadi 3
#define soket_dinlenemiyor 4
#define select_hatasi 5

using namespace PlayerCc;

// Robot Değişkenleri
std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);
uint         gPort(PlayerCc::PLAYER_PORTNUM);
uint         gIndex(0);
uint         gDebug(0);
uint         gFrequency(10); // Hz
uint         gDataMode(PLAYER_DATAMODE_PUSH);
bool         gUseLaser(false);

// Ana Fonksiyon
int main(int argc, char **argv)
{
	// Kontrol Değerleri
	double yakin= 0.3;
	double hiz= 0.2;
	double aci= 30;

	double eskihiz= 0;
	double eskiaci= 0;
	double yeniaci= 0;
	double yenihiz= hiz;

	// Soket Değişkenleri
    int dinleyici, hakem;  // sock_fd ile dinle, yen bağlantıyı new_fd ile al
    struct sockaddr_in my_addr;    // adres bilgim
    struct sockaddr_in their_addr; // bağlananın adres bilgisi
	struct timeval tv;
	fd_set okunacak;
    socklen_t sin_size;
	int yes=1;
	char tampon[1024];
	int alinanbayt;

	try
	{
		// Robota Bağlan
		PlayerClient robot (gHostname, gPort);
		Position2dProxy konum (&robot, gIndex);
		SonarProxy sonar (&robot, gIndex);
		std::cout << robot << std::endl;

		// Dinleme Soketini Yarat
	    if ((dinleyici = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw soket_olusturulamadi;

		// Port Kullanılıyor Hatasını Engelle
	    if (setsockopt(dinleyici,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
			throw port_kullanilamiyor;

		// IP Adreslerini Ayarla
	    my_addr.sin_family = AF_INET;         // konak bayt sıralaması
	    my_addr.sin_port = htons(PORT);     // short, ağ bayt sıralaması
	    my_addr.sin_addr.s_addr = INADDR_ANY; // otomatik olarak IP'mi kullan
	    memset(&(my_addr.sin_zero), '\0', 8); // geriye kalan bölgeyi sifirla

		// Dinleme Soketini Gerekli Porta Bağla
	    if (bind(dinleyici, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
			throw soket_baglanamadi;

		// Soketi Dinlemeye Başla
	    if (listen(dinleyici, 3) == -1)
			throw soket_dinlenemiyor;

		sin_size = sizeof(struct sockaddr_in);

		// Select Değişkenleri
/*		tv.tv_sec=1;
		tv.tv_usec=0;
*/
    	while (1)
		{
			try
			{
				// Gelen Bağlantıları Kabul Et
				if ((hakem = accept(dinleyici, (struct sockaddr *)&their_addr,&sin_size))==-1)
					continue;

				// Kümeyi Yeni Sokete Eşitle
/*				FD_ZERO(&okunacak);
				FD_SET(hakem,&okunacak);
*/
				//konum.SetMotorEnable (true);
                std::cout<<"Bağlandı. hakem= "<<hakem<<std::endl;

			// Bloklamayı Engelle
			fcntl(hakem, F_SETFL, O_NONBLOCK);

				// Döngüyü Başlat
				while (1)
				{
					robot.Read();
/*					if (select(hakem+1, &okunacak, NULL, NULL, NULL) == -1) 
						throw 5;
					if (FD_ISSET(hakem,&okunacak))
					{
						alinanbayt=recv(hakem,tampon, sizeof(tampon), 0);
						if (alinanbayt==0)
						{
							close(hakem);
							break;
						}
						else
						{
							yenihiz = atof(tampon);
						}
					}
*/
					alinanbayt=recv(hakem,tampon, sizeof(tampon), 0);
					if (alinanbayt==0)
					{
						close(hakem);
						break;
					}
					if (alinanbayt>0)
					{
						yenihiz = atof(tampon);
						std::cout<<"Yeni hız: "<<yenihiz<<std::endl;
					}

					// İleri Giderken Önünde Bir Şey Varsa Hızı Sıfırla
					if (yenihiz>0.0)
					{
						if (sonar[2]<yakin||sonar[3]<yakin||sonar[4]<yakin||sonar[5]<yakin)
						{
							yenihiz=0;
						}
					}

					// Geri Giderken Arkanda Bir Şey Varsa Hızı Sıfırla
					if (yenihiz<0.0)
					{
						if (sonar[11]<yakin||sonar[12]<yakin||sonar[13]<yakin||sonar[14]<yakin)
						{
							yenihiz=0;
						}
					}

					// Aldığın Kararları Uygula
					konum.SetSpeed(yenihiz,yeniaci);
				}
                std::cout<<"Bağlantı kesildi"<<std::endl<<std::flush;
				//konum.SetMotorEnable (false);
				yenihiz=0;
				konum.SetSpeed(yenihiz,yeniaci);
			}
			catch (int hata)
			{
				std::cerr <<"Hata kodu: "<< hata << std::endl;
				// Burda soket kapanması gerekiyor olabilir
				// Belki motor da kapatılacaktır
				return -1;
			}
		}
	}

	// Soket Hatalarını Yakala
	catch (int hata)
	{
		std::cerr << hata << std::endl;
		return -1;
	}

	// Robot Hatalarını Yakala
	catch (PlayerCc::PlayerError e)
	{
		std::cerr << e << std::endl;
		return -1;
	}
}
