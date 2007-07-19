#include "socket.h"
#include <sstream>

string parcala(string &girdi,string aranan)
{
	int i;
	string parca;
	i = girdi.find(aranan);
	parca = girdi.substr(0,i);
	girdi = girdi.substr(i+aranan.size());
	return parca;
}

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

void Soket::Gonder(string veri)
{
	int bayt;
	bayt = send(soketim,veri.c_str(),veri.size(),0);
	if (bayt==-1) throw 4;
}

string Soket::Al(void)
{
	int bayt,i,j;
	char tampon[1024], tampon2[1024];
	string veri;
	bayt = recv(soketim,tampon,1024,0);
	for (j=0;j<1023;j++)
	if (tampon[j]=='b') 
	{
		break;
	}
	for (i=j;i<1023;i++)
	if (tampon[i]==';') 
	{
		break;
	}
	if (bayt<0) return veri;
	if (bayt==0) throw baglanti_koptu;
	veri=tampon;
	veri=veri.substr(j,i-j+1);
	return veri;
}

void Soket::SonarOku(void)
{
	string alinan,parca;
	int i;
	Gonder("arbitrator.request(\"sonar\");");
	alinan = Al();
	parca = parcala(alinan,"[");
	for (int i=0;i<15;i++)
	{
		parca = parcala(alinan,",");
		sonar[i] = atof(parca.c_str());
	}
	parca = parcala(alinan,"]");
	sonar[15]=atof(parca.c_str());
}

void Soket::Davran(float hiz,float aci,float agirlik)
{
	char bir[1024], iki[1024], uc[1024];
	string veri;
	sprintf(bir,"%f",hiz);
	sprintf(iki,"%f",aci);
	sprintf(uc,"%f",agirlik);
	veri="arbitrator.wspeed("+string(bir)+","+string(iki)+","+string(uc)+");\n";
	Gonder(veri);
}

Soket::~Soket()
{
	close(soketim);
}
