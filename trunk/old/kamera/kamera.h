// #include <sys/ioctl.h>
#include <fcntl.h>		// open()
#include <unistd.h>		// close()
#include <sys/ioctl.h>	// ioctl()

#include <linux/videodev.h>

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class kamera
{
	private:
		int aygit;
		struct video_capability vc;
		struct video_window vw;
		char* resim;
	public:
		struct video_picture vp;
		kamera(string);
		~kamera();
		void yetenek_yazdir();
		void pencere_yazdir();
		void goruntu_yazdir();
		char *goruntuyu_al();
		int genislik_oku();
		int yukseklik_oku();
		void resim_yakala();
		void ppm_yazdir();
		void dosyaya_ppm_yazdir(char *);
		void parlaklik_ayarla(int);
		void otomatik_parlaklik();
		void maksimum_cozunurluk();
		void minimum_cozunurluk();
		void orta_nokta(int,int,int,int=20);
		void orta_degistir(int,int,int,int=20);
};
