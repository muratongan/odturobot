# --- macros
CC=g++
CFLAGS=  -g
OBJECTS= LineFollower.o LineFinder.o kamera.o socket.o
LIBS = -lm


# --- targets
all:    lineFollower
lineFollower:	$(OBJECTS) 
	$(CC)  -o lineFollower  $(OBJECTS) $(LIBS)

finder: all
	rm LineFinder.o
	all
       
LineFinder.o:
	$(CC) $(CFLAGS) -c LineFinder.cpp

kamera.o:
	$(CC) $(CFLAGS) -c kamera.cpp

socket.o:
	$(CC) $(CFLAGS) -c socket.cpp

LineFollower.o:	LineFinder.o socket.o kamera.o LineFollower.cpp
	$(CC) $(CFLAGS) -c LineFollower.cpp

# --- remove binary and executable files
clean:
	rm -f lineFollower $(OBJECTS)

casual:
	g++ LineFollower.cpp -o lineFollower -g -lm -I../../cclub/robot/ ../../cclub/robot/socket.cpp LineFinder.cpp kamera.cpp
