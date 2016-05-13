CC = g++
CPPFLAGS = -O2 -W -Wall -g
VBSRC = physics.cpp videoBall.cpp
PGSRC = physics.cpp pong.cpp
VBOBJ = videoBall.o physics.o
PGOBJ = pong.o physics.o
LDLIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

LINK = physics
VBALL = videoBall
PONG = pong

all: videoBall pong
# all: pong

clean:
	rm -rf $(VBALL) $(PONG) *.o core* *.dSYM

videoBall: $(VBOBJ)
		$(CC) -o$(VBALL) $(CPPFLAGS) $(VBSRC) $(LDLIBS)
		
pong: $(PGOBJ)
		$(CC) -o$(PONG) $(CPPFLAGS) $(PGSRC) $(LDLIBS)

videoBall.o: videoBall.cpp physics.h
		$(CC) -c videoBall.cpp

physics.o: physics.cpp physics.h
		$(CC) -c physics.cpp

pong.o: pong.cpp physics.h
		$(CC) -c pong.cpp

# myprogam: main.o physics.o
#     gcc -o myprogram main.o foo.o
#
# main.o: main.c foo.h
#     gcc -c main.c
#
# foo.o: foo.c foo.h
#     gcc -c foo.c
