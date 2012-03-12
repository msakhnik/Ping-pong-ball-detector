NAME = ball-detector

CXX = g++

CFLAGS = -Wall -g


INCLUDES = $(shell pkg-config --cflags opencv)

LIBS = $(shell pkg-config --libs opencv)

all : $(NAME)

#################

$(NAME): main.o BallDetector.o
	$(CXX) -o $(NAME) main.o   $(CFLAGS) BallDetector.o $(LIBS)

main.o: main.cpp
	$(CXX) -c $(CFLAGS) $(INCLUDES) $(LIBS) main.cpp

BallDetector.o: BallDetector.h BallDetector.cpp
	$(CXX) -c $(CFLAGS) $(INCLUDES) $(LIBS)  BallDetector.cpp  BallDetector.h

clean:
	rm -f *.o $(NAME)
