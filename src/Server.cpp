//============================================================================
// Name        : Server.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <string>
#include <iostream>
#include <algorithm>

#include "User.h"
#include "MenuOptions.h"

#define MYPORT 336958
#define BACKLOG 10
using std::cout;
using std::endl;

void sigchld_handler(int s){

	while(wait(NULL) > 0);

}

int main(void) {

	int sockfd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	struct sigaction sa;
	int yes = 1;
	char *recvbuf;
	char *caddr;
	char* recieved;
	long numbytes;

	User user= User();
	MenuOptions menu = MenuOptions();


	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	printf("SOCK_FD=%d\n", sockfd);
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		perror("setsockopt");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8);

	if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1){
		perror("bind");
		exit(1);
	}

	if(listen(sockfd, BACKLOG) == -1){
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if(sigaction(SIGCHLD, &sa, NULL) == -1){
		perror("sigaction");
		exit(1);
	}

	//Main server function loop
	while(1){
		sin_size = sizeof(struct sockaddr_in);
		//added (socklen_t*) due to type mismatch
		if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t*) &sin_size)) == -1){
			perror("accept");
			exit(1);
		}
		printf("[Server] got connection from %s\n",  inet_ntoa(their_addr.sin_addr));
		//Creating threads for each client connection
		if(!fork()){
			close(sockfd);
			recvbuf=(char *) calloc(128, sizeof(char));
			bool loggedIn = false;

			while(!loggedIn){

				string loginScreen = "\n***********************************\n"
						"Welcome to the Calendar\n"
						"	Everyone's Quacking about!\n"
						"    _      _      _    \n"
						"  >(.)__ <(.)__ =(.)__ \n"
						"   (___/  (___/  (___/ \n"
						"\n"
				        "1. Login\n"
						"2. New account\n"
						"3. Exit\n"
						"***********************************\n";
				if(send(new_fd, loginScreen.c_str(), 512, 0) == -1){
					perror("send");
					close(new_fd);
					exit(1);
				}
				cout << loginScreen << "\n";

				//Receive Selection
				numbytes = recv(new_fd, recvbuf, 127, 0);
				recvbuf[numbytes] = '\0';
				string loginSelection = recvbuf;
				cout << "[Client] " << loginSelection << "\n";


				if(!loginSelection.compare("1")){
					while(!loggedIn) {

						loggedIn = menu.login(new_fd, recvbuf, user, loggedIn);
					}
				}
				else if(!loginSelection.compare("2")){

					loggedIn = menu.newAccount(new_fd, recvbuf, user, loggedIn);
				}
				else if(!loginSelection.compare("3")){

					close(new_fd);
					_Exit(0);
				}
				else{
					cout << "Invalid Choice"<< endl;
				}
			}

			while(true){

				string mainMenu = "\n***********************************\n"
						"Main Menu:\n"
				        "1.  Add appointment\n"
				        "2.  Remove appointment\n"
						"3.  Update existing appointment\n"
						"4.  Display appointment at a time\n"
						"5.  Display appointments in a time range\n"
						"6.  Change name\n"
						"7.  Change password\n"
						"8.  Change phone number\n"
						"9.  Change email\n"
						"10. Delete a user\n"
						"11. Exit\n"
						"***********************************\n";

				if(send(new_fd, mainMenu.c_str(), 512, 0) == -1){
					perror("send");
					close(new_fd);
					exit(1);
				}

				numbytes=recv(new_fd, recvbuf, 127, 0);
				recvbuf[numbytes] = '\0';

				string selection = recvbuf;

				if(!selection.compare("1")){

					menu.addAppointment(new_fd, recvbuf, user);
				}
				else if(!selection.compare("2")){

					menu.deleteAppointment(new_fd, recvbuf, user);
				}
				else if(!selection.compare("3")){

					menu.updateAppointment(new_fd, recvbuf, user);

				}
				else if(!selection.compare("4")){

					menu.displayAppointTime(new_fd, recvbuf, user);
				}
				else if(!selection.compare("5")){

					menu.displayAppointRange(new_fd, recvbuf, user);
				}
				else if(!selection.compare("6")) {

					menu.changeName(new_fd, recvbuf, user);
				}
				else if(!selection.compare("7")){

					menu.changePassword(new_fd, recvbuf, user);
				}
				else if(!selection.compare("8")){

					menu.changePhone(new_fd, recvbuf, user);
				}
				else if(!selection.compare("9")){

					menu.changeEmail(new_fd, recvbuf, user);
				}
				else if(!selection.compare("10")){

					menu.deleteUser(new_fd, recvbuf, user);
				}
				else if(!selection.compare("11") || !selection.compare("k")){
					user.updateToFile();
					close(new_fd);
					_Exit(0);
				}
			}
		}
		close(new_fd);
	}

	user.updateToFile();
	return 0;
}
