#include "kamera.h"

kamera::kamera(string dosyaadi)
{
	resim=NULL;
	
	aygit = open(dosyaadi.c_str(),O_RDONLY);				// kamerayı aç
	if (aygit<0) throw 1; // Aygıt açılamadı
	
	if (ioctl(aygit, VIDIOCGCAP, &vc) < 0) throw 2;			// kamera bilgilerini al
	if (ioctl(aygit, VIDIOCGWIN, &vw) < 0) throw 3;			// pencere bilgilerini al
	if (ioctl(aygit, VIDIOCGPICT, &vp) < 0) throw 4;		// resim bilgilerini al
	
	if (vc.type!=1) throw 5; // Alet görüntü yakalama aleti değil
	if (vp.palette!=4) throw 6; // Renk Paleti 24bit RGB değil
}

kamera::~kamera()
{
	close(aygit);
}

void kamera::yetenek_yazdir(void)
{
	cout<<"Aygıt Adı: "<<vc.name<<endl;
	cout<<"Aygıt Türü: "<<vc.type;
	switch (vc.type)
	{
		case 1: cout<<" (Görüntü Yakalama Aygıtı)"<<endl;break;
		default: cout<<" (Bilinmeyen Tür)"<<endl;
	}
	cout<<"Kanal Sayısı: "<<vc.channels<<endl;
	cout<<"Ses Aygıtı Sayısı: "<<vc.audios<<endl;
	cout<<"Maksimum Boyut: "<<vc.maxwidth<<"x"<<vc.maxheight<<endl;
	cout<<"Minimum Boyut: "<<vc.minwidth<<"x"<<vc.minheight<<endl;
}

void kamera::pencere_yazdir(void)
{
	cout<<"x,y: "<<vw.x<<","<<vw.y<<endl;
	cout<<"Boyut: "<<vw.width<<"x"<<vw.height<<endl;
	cout<<"Chromakey: "<<vw.chromakey<<endl;
	cout<<"Flags: "<<vw.flags<<endl;
	cout<<"Clipcount: "<<vw.clipcount<<endl;
}

void kamera::goruntu_yazdir(void)
{
	cout<<"Parlaklık: "<<vp.brightness<<endl;
	cout<<"Hue: "<<vp.hue<<endl;
	cout<<"Renk: "<<vp.colour<<endl;
	cout<<"Karşıtlık: "<<vp.contrast<<endl;
	cout<<"Beyazlık: "<<vp.whiteness<<endl;
	cout<<"Derinlik: "<<vp.depth<<endl;
	cout<<"Palet: "<<vp.palette;
	switch (vp.palette)
	{
		case 4: cout<<" (24bit RGB)"<<endl;break;
		default: cout<<" (Bilinmeyen Tür)"<<endl;
	}
}

int kamera::genislik_oku(void)
{
	return vw.width;
}

int kamera::yukseklik_oku(void)
{
	return vw.height;
}

void kamera::resim_yakala(void)
{
	int boyut = vw.width*vw.height;
	if (resim)
	{
		delete []resim;
		resim = NULL;
	}
	resim = new char[boyut*3];
	if (!resim) throw 7; // Bellek Dolu
	
	int okunan=0;
// 	while (okunan<boyut*3)
	{
		okunan+=read(aygit,resim+okunan,boyut*3-okunan);
	}
 	if (okunan<boyut*3) terminate();
	
	int gecici;	// BGR'yi RGB'ye cevir
	for (int i=0;i<vw.width*vw.height;i++)
	{
		gecici = resim[3*i];
		resim[3*i] = resim[3*i+2];
		resim[3*i+2] = gecici;
	}
}

void kamera::ppm_yazdir(void)
{
	if (!resim) resim_yakala();
	cout<<"P6"<<endl<<vw.width<<" "<<vw.height<<" 255"<<endl;
	for (int i=0;i<vw.width*vw.height*3;i++) printf("%c",resim[i]);
}

void kamera::dosyaya_ppm_yazdir(char *filename)
{
	ofstream fout(filename);
	if (!resim) resim_yakala();
	fout<<"P6"<<endl<<vw.width<<" "<<vw.height<<" 255"<<endl;
	int position = fout.tellp();
	fout.close();

	fout.open(filename, ios::binary|ios::app);
	fout.seekp(position);
	for (int i=0;i<vw.width*vw.height*3;i++) 
		fout << resim[i];
	fout.close();
}

void kamera::parlaklik_ayarla(int yeni)
{
	vp.brightness = yeni;
	if (ioctl(aygit, VIDIOCSPICT, &vp)==-1) throw 8; // Parlaklık ayarlanamadı
}

void kamera::otomatik_parlaklik(void)
{
	bool oldumu=false;
	do
	{
		resim_yakala();
		long int toplam=0L;
		long int ortalama,fark;
		for (int i=0;i<vw.width*vw.height*3;i++) toplam += (unsigned char) resim[i];
		//cout << "toplam: " << toplam << endl;
		ortalama = (toplam / (vw.width*vw.height*3));
		//cout << "1st average: " << ortalama << endl;
		fark = (128L-ortalama);
		if (abs(fark)<10 || abs(vp.brightness-32000)>18000) oldumu=true;	// Düzenlenebilir Satır
		if (!oldumu && fark > 0) parlaklik_ayarla(vp.brightness+10*fark); // Farkın belli bir katı/oranı alınabilir
		if (!oldumu && fark < 0) parlaklik_ayarla(vp.brightness-10*fark); // Farkın belli bir katı/oranı alınabilir
		//cout << "2nd average: " << ortalama << endl;
	} while (!oldumu);
}

void kamera::orta_nokta(int kirmizi, int yesil, int mavi, int tolerans)
{
	if (!resim) resim_yakala();
	
	long toplamx=0,toplamy=0,toplam=0;
	for (int i=0;i<vw.width*vw.height;i++)
	{
		if ( (abs(resim[3*i]-kirmizi)<tolerans) && (abs(resim[3*i+1]-yesil)<tolerans) && (abs(resim[3*i+2]-mavi)<tolerans) )
		{
			toplamx += i % vw.width;
			toplamy += i / vw.height;
			toplam++;
		}
	}
	cout<<"toplamx: "<<toplamx<<endl<<"toplamy: "<<toplamy<<endl<<"toplam:  "<<toplam<<endl<<"ortax:   "<<toplamx/toplam<<endl<<"ortay:   "<<toplamy/toplam<<endl;
}

void kamera::orta_degistir(int kirmizi, int yesil, int mavi, int tolerans)
{
	if (!resim) resim_yakala();
	
	long toplamx=0,toplamy=0,toplam=0;
	for (int i=0;i<vw.width*vw.height;i++)
	{
		if ( (abs(resim[3*i]-kirmizi)<tolerans) && (abs(resim[3*i+1]-yesil)<tolerans) && (abs(resim[3*i+2]-mavi)<tolerans) )
		{
			resim[3*i] = 255;
			resim[3*i+1]=0;
			resim[3*i+2]=0;
		}
	}
}

void kamera::maksimum_cozunurluk()
{
	vw.width=vc.maxwidth;
	vw.height=vc.maxheight;
	if (ioctl(aygit, VIDIOCSWIN, &vw)==-1) throw 8; // Parlaklık ayarlanamadı
	if (ioctl(aygit, VIDIOCGWIN, &vw)==-1) throw 8; // Parlaklık ayarlanamadı
}

void kamera::minimum_cozunurluk()
{
	vw.width=vc.minwidth;
	vw.height=vc.minheight;
	if (ioctl(aygit, VIDIOCSWIN, &vw)==-1) throw 8; // Parlaklık ayarlanamadı
	if (ioctl(aygit, VIDIOCGWIN, &vw)==-1) throw 8; // Parlaklık ayarlanamadı
}


char *kamera::goruntuyu_al() {
	return resim;
}

/*
int main(void)
{
	kamera kameram("/dev/video0");

 	kameram.maksimum_cozunurluk();

	kameram.otomatik_parlaklik();
	kameram.vp.brightness+=30000;
	kameram.vp.contrast+=30000;
	
	kameram.resim_yakala();
	//kameram.orta_degistir(25,0,0,10);
	kameram.ppm_yazdir();
	
	return 0;
}
*/
