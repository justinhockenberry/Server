serverRun: Server.o DAL.o
	g++ -std=c++11 Server.o DAL.o -o serverRun

Client.o: Server.cpp
	g++ -c -std=c++11 Server.cpp

Utilities.o: DAL.cpp
	g++ -c -std=c++11 DAL.cpp

clean:
	rm *.o serverRun
