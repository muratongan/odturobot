/*
** selectserver.c -- keyifli bir cok kullanicili chat sunucu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9034   // dinledigimiz port

#include "camera.h"

using namespace std;

int main(void)
{
    fd_set master;    // ana dosya tanimlayici listesi
    fd_set read_fds;  // select() icin gecici dosya tanimlayici listesi
    struct sockaddr_in myaddr;     // sunucu adresi
    struct sockaddr_in remoteaddr; // istemci adresi
    int fdmax;        // azami dosya tanimlayici numarası
    int listener;     // dinlenen soket tanımlayıcı
    int newfd;        // yeni accept()lenecek soket tanımlayıcı
    char buf[256];    // istemci verisi için tampon
    int nbytes;
    int yes=1;        // setsockopt() SO_REUSEADDR için, aşağıda
    int addrlen;
    int i, j;
    
    Kamera mycam;

    FD_ZERO(&master);    // ana listeyi ve gecici listeyi temizle
    FD_ZERO(&read_fds);

    // dinleyiciyi yarat
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // "adres zaten kullanımda" mesajından kurtul
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,
                                                        sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    // bind
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(PORT);
    memset(&(myaddr.sin_zero), '\0', 8);
    if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(1);
    }






    // dinleyici soketi ana listeye ekle
    FD_SET(listener, &master);

    // en büyük dosya tanimlayicisi hatirla
    fdmax = listener; // so far, it's this one

    // ana döngü
    for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        // mevcut baglantilari tarayip okumaya hazir olanlari tespit et
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // bir tane yakaladik!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof(remoteaddr);
                    if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr,
                                                  (socklen_t*) &addrlen)) == -1) {
                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // ana listeye ekle
                        if (newfd > fdmax) {    // azami miktarı güncelle
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on "
                            "socket %d\n", inet_ntoa(remoteaddr.sin_addr), newfd);
                    }
                } else {
                    // istemciden gelen veri icin gerekeni yap
                    if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
                        // bir hata var ya da istemci baglantiyi kesti
                        if (nbytes == 0) {
                            // baglanti kesilmis
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // ana listeden cikar
                    } else {
                        // istemciden bir miktar veri geldi
                        
                        if (strcmp(buf, "getCircles") == 0)
                        {
                            mycam.capture();
                            int* a = mycam.getCircles();
                            
                            int j;
                            for( j= 0; j < a[0]; j++ )
                                {
                                     cvCircle( mycam.frame, cvPoint(a[3*j+1],a[3*j+2]), 3, CV_RGB(0,255,0), -1, 8, 0 );
                                     cvCircle( mycam.frame, cvPoint(a[3*j+1],a[3*j+2]), a[3*j+3], CV_RGB(255,0,0), 3, 8, 0 );
                                }
                                                    
			    cvNamedWindow("circles", 1);                            
                            cvShowImage( "circles", mycam.frame );
                            if( (cvWaitKey(10) & 255) == 27 ) break;
			    fflush(stdout);
                            int size;
                            size = a[0]*3+1;
                            size = send(i, a, size * sizeof(int), 0);
                        }
                        else if (strcmp(buf, "getLines") == 0)
                        {
                            mycam.capture();
                            int* a = mycam.getLines();
                            
                            int j;
                            for( j= 0; j < a[0]; j++ )
                                {
                                     cvLine( color_dst, cvPoint(a[4*j+1],a[4*j+2]), cvPoint(a[4*j+3],a[4*j+4]), CV_RGB(255,0,0), 3, 8 );
                                }
                            
                            cvNamedWindow( "Hough", 1 );
                            cvShowImage( "Hough", color_dst );
                                                
                            int size;
                            size = a[0]*4+1;
                            send(i, a, size * sizeof(int), 0);
                        }
                        else if (strcmp(buf, "getInfo") == 0)
                        {
                            int* a = mycam.getInfo();
                            send(i, a, 2 * sizeof(int), 0);
                        }

                    }
                }
            }
        }
    }

    return 0;
}
