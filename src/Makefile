serverRun: Server.o User.o MenuOptions.o md5.o
	g++ -std=c++11 Server.o User.o MenuOptions.o md5.o -o serverRun

Server.o: Server.cpp
	g++ -c -std=c++11 Server.cpp

User.o: User.cpp
	g++ -c -std=c++11 User.cpp
	
MenuOptions.o: MenuOptions.cpp
	g++ -c -std=c++11 MenuOptions.cpp
	

md5.o: md5.cpp
	g++ -c -std=c++11 md5.cpp

clean:
	rm *.o serverRun
