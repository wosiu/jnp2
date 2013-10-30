OBJS=growingnet.o network.o
CXX=g++

ifeq ($(debuglevel), 1)
	CXXFLAGS=-Wall -g -std=c++0x
else
	CXXFLAGS=-O2 -std=c++0x -DNDEBUG
endif

all: $(OBJS)

#all: network_test2 network_test1

#network_test2: network_test2.o network.o
#	g++ network_test2.o network.o -o network_test2

#network_test2.o: private/network_test2.c
#	gcc -Wall -O2 -ggdb -c private/network_test2.c -o network_test2.o

#network_test1: network_test1.o growingnet.o network.o
#	g++ network_test1.o growingnet.o network.o -o network_test1

#network_test1.o: private/network_test1.c
#	gcc -Wall -O2 -ggdb -c private/network_test1.c -o network_test1.o

growingnet.o: growingnet.cc
	gcc $(CXXFLAGS) -c growingnet.cc -o growingnet.o

network.o: network.cc
	gcc $(CXXFLAGS) -c network.cc -o network.o

clean:
	rm -f $(OBJS)
