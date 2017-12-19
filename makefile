a.out: main.o
	g++ -Wall -pedantic main.o
rcMatrx.o: main.cpp rcMatrx.h
	g++ -c -Wall -pedantic main.cpp -o
