all: network_test2
#network_test1

network_test2: network_test2.o network.o
	g++ network_test2.o network.o -o network_test2

network_test2.o: private/network_test2.c
	gcc -Wall -O2 -ggdb -c private/network_test2.c -o network_test2.o

network_test1: network_test1.o network.o
	g++ network_test1.o network.o -o network_test1

network_test1.o: private/network_test1.c
	gcc -Wall -O2 -ggdb -c private/network_test1.c -o network_test1.o

network.o: network.cc
	gcc -O2 -ggdb -std=c++0x -c network.cc -o network.o

clean:
	rm -f *.o network_test1 network_test2
