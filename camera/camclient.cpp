#include "camclient.h"
#include <sstream>

CamClient::CamClient(int port)
{

		// CamClientini Yarat
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

		// Dinleme CamClientini Gerekli Porta Bağla
		if (connect(soketim, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) 
		{
	        perror("connect");
	        exit(1);
	    }
}

void CamClient::sendAll(char* veri)
{
    int giden = 0;
    int boyut = strlen(veri) + 1;
    while (giden<boyut)
    {
        giden += send(soketim, veri, boyut-giden, 0);
    }
}

int* CamClient::getCircles()
{
    int * array, alinan;
    char* istek = "getCircles";
    sendAll(istek);
    array = (int *) malloc(1 * sizeof(int));
    recv(soketim, array, sizeof(int), 0);
    if (array[0] > 0)
    {
        array = (int *) realloc(array, (1 + array[0] * 3) * sizeof(int));
        alinan = recv(soketim, array+1, array[0] * 3 * sizeof(int), 0);
    }
    return array;
}

int* CamClient::getLines()
{
    int * array;
    char* istek = "getLines";
    sendAll(istek);
    array = (int *) malloc(sizeof(int));
    recv(soketim, array, sizeof(int), 0);
	if (array[0]>0)
{
    array = (int *) realloc(array, (1 + array[0] * 4) * sizeof(int));
    recv(soketim, array+1, array[0] * 4 * sizeof(int), 0);
}
    return array;
}

int* CamClient::getInfo()
{
    int *array;
    char* istek = "getInfo";
    sendAll(istek);
    array = (int *) malloc(2 * sizeof(int));
    recv(soketim, array, 2 * sizeof(int), 0);
    return array;
}

CamClient::~CamClient()
{
	close(soketim);
}
