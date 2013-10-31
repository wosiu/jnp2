OBJS=growingnet.o network.o
CXX=g++

ifeq ($(debuglevel), 1)
	CXXFLAGS=-Wall -g -std=c++0x
else
	CXXFLAGS=-O2 -std=c++0x -DNDEBUG
endif

all: $(OBJS)

growingnet.o: growingnet.cc
	gcc $(CXXFLAGS) -c growingnet.cc -o growingnet.o

network.o: network.cc
	gcc $(CXXFLAGS) -c network.cc -o network.o

clean:
	rm -f $(OBJS)
