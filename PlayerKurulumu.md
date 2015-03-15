Player yazılımı, player projesinin bir parçasıdır. Robot kontrolü için donanımdan bağımsız bir platform oluşturmayı sağlar. Projemizde, robotla iletişim kurmak için player kütüphanesini kullanıyoruz.

# Ek Paketler #
Asagidaki ek paketlerin de kurulmasi gerekli:
> libavcodec1d libavformat1d libavutil1d libboost-signals1.34.1 libcv1 libcvaux1 libdc1394-13 libgsm1 libhighgui1 libplayerc++2 libplayerc2 libplayercore2
> libplayerdrivers2 libplayererror2 libplayerjpeg2 libplayertcp2 libplayerxdr2
libboost-thread-dev

# Kurulum #
Player programını Pardus'ta aşağıdaki adımları izleyerek bulabilirsiniz. Diğer Linux dağıtımlarında da benzer şekilde kurulumu yapılabilir. Bazı dağıtımlar, bu yazılımı paket olarak da sağlayabilirler.
  * Önce http://playerstage.sf.net adresinin “download” kısmına girip player-2.0.3.tar.bz2 ve stage-2.0.3.tar.bz2 dosyalarını ev dizinimize indiriyoruz.
  * Konsol/terminal programını açıp ev dizinimizde root yetkisine kavuşmak için “su -” yazıp şifremizi giriyoruz.
  * Ardından arşivleri açıyoruz:
```
tar xvf player-2.0.2.tar.bz2
tar xvf stage-2.0.3.tar.bz2
```
  * Player programını kurmak için gerekli dizine girip, çıkan kamera hatasıya karşılaşmamak için gerekli ayarı yapıp kuruyoruz:
```
cd player-2.0.2
./configure --disable-camerauvc
make install
```
  * Bu işlemler esnasında libGL ile ilgili bir hata geçerse aşağıdaki komutla dosya konumuna bağlantı koyuyoruz:
```
link /usr/lib/libGLw.so /usr/lib/libGL.so
```
Bu adımlardan sonra bir aksilik çıkmazsa player'ın kurulumu tamamlanmış olacak. Yeni yüklenen programın yolunun eklenmemesi nedeniyle oluşan hatayı gidermek için aşağıdaki komutu kullanabiliriz:
```
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
```
Artık, player ile robotumuzu kontrol etmeye başlayabiliriz.