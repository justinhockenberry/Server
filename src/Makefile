serverRun: Server.o User.o
	g++ -std=c++11 Server.o User.o -o serverRun

Client.o: Server.cpp
	g++ -c -std=c++11 Server.cpp

Utilities.o: User.cpp
	g++ -c -std=c++11 User.cpp

clean:
	rm *.o serverRun
