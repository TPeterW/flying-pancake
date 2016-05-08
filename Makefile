CC = g++
CPPFLAGS = -O2 -W -Wall -g
SRC = videoBall.cpp physics.cpp
LDLIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

LINK = physics
BIN = videoBall
all:
	$(CC) $(CPPFLAGS) $(SRC) -o$(BIN) $(LDLIBS)  
clean:
	rm -f $(BIN) *.o core*
