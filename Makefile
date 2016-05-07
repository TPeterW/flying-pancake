CC = g++
CPPFLAGS = -O2 -W -Wall -g
LDLIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video

BIN = videoBall
all: $(BIN)

clean:
	rm -f $(BIN) *.o core*
