#include "socket.h"
#include <sstream>

Soket::Soket(int port)
{

		// Soketini Yarat
	    if ((soketim = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw soket_olusturulamadi;

		// Port Kullanılıyor Hatasını Engelle
		int yes;
		yes=1;
	    if (setsockopt(soketim,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
			throw port_kullanilamiyor;

		// IP Adreslerini Ayarla
	    their_addr.sin_family = AF_INET;         // konak bayt sıralaması
	    their_addr.sin_port = htons(port);     // short, ağ bayt sıralaması
	    their_addr.sin_addr.s_addr = INADDR_ANY; // otomatik olarak IP'mi kullan
	    memset(&(their_addr.sin_zero), '\0', 8); // geriye kalan bölgeyi sifirla

		// Dinleme Soketini Gerekli Porta Bağla
		if (connect(soketim, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) 
		{
	        perror("connect");
	        exit(1);
	    }
}

int* Soket::getCircles()
{
    int * array;
    char* istek = "getCircles";
    send(soketim, istek, sizeof(istek), 0);
    array = malloc(1, sizeof(int))
    recv(soketim, array[0], sizeof(int), 0);
    array = malloc(1 + array[0] * 3, sizeof(int));
    recv(soketim, number, sizeof(int), 0);
    return array;
}

int* Soket::getLines()
{
    int * array;
    char* istek = "getLines";
    send(soketim, istek, sizeof(istek), 0);
    array = malloc(1, sizeof(int))
    recv(soketim, array[0], sizeof(int), 0);
    array = malloc(1 + array[0] * 4, sizeof(int));
    recv(soketim, number, sizeof(int), 0);
    return array;
}

Soket::~Soket()
{
	close(soketim);
}
