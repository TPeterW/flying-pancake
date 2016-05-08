CC = g++
CPPFLAGS = -O2 -W -Wall -g
SRC = physics.cpp videoBall.cpp
OBJ = videoBall.o physics.o
LDLIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

BIN = videoBall

all: videoBall
	$(CC) -o$(BIN) $(CPPFLAGS) $(SRC) $(LDLIBS)

clean:
	rm -f $(BIN) *.o core*

videoBall: $(OBJ)
		$(CC) -o$(BIN) $(CPPFLAGS) $(SRC) $(LDLIBS)

videoBall.o: videoBall.cpp physics.h
		$(CC) -c videoBall.cpp

physics.o: physics.cpp physics.h
		$(CC) -c physics.cpp

# myprogam: main.o physics.o
#     gcc -o myprogram main.o foo.o
#
# main.o: main.c foo.h
#     gcc -c main.c
#
# foo.o: foo.c foo.h
#     gcc -c foo.c
