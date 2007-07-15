#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;

#define soket_olusturulamadi 1
#define port_kullanilamiyor 2
#define soket_baglanamadi 3
#define soket_gonderilemiyor 4
#define baglanti_koptu 5

class Soket
{
	private:
		int soketim;		
	    struct sockaddr_in my_addr;    // adres bilgim
	    struct sockaddr_in their_addr; // bağlananın adres bilgisi
	public:
		Soket(int);
		void Gonder(string);
		void SonarOku(void);
		string Al(void);
		void Davran(float,float,float);
		~Soket();
		float sonar[16];
};
