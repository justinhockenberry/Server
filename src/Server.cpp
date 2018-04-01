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
			bool validated = false;
			while(!loggedIn){

				std::string login = "\n***********************************\n"
						"Welcome to the Calendar\n"
						"	Everyone's Quacking about!\n"
						"    _      _      _    \n"
						"  >(.)__ <(.)__ =(.)__ \n"
						"   (___/  (___/  (___/ \n"
						"\n"
				        "1) Login\n"
						"2) Open new account\n"
						"***********************************\n";
				if(send(new_fd, login.c_str(), 512, 0) == -1){
					perror("send");
					close(new_fd);
					exit(1);
				}
				std::cout <<"[Server]: "<< login << "\n";

				//Receive Selection
				numbytes = recv(new_fd, recvbuf, 127, 0);
				recvbuf[numbytes] = '\0';
				std::string loginChoice = recvbuf;
				std::cout << "[Client] " << loginChoice << "\n";


				if(!loginChoice.compare("1")){
					while(!loggedIn) {

						loggedIn = menu.login(new_fd, recvbuf, user, loggedIn);

					}
				}
				else if(!loginChoice.compare("2")){

					loggedIn = menu.newAccount(new_fd, recvbuf, user, loggedIn);

				}
				else{
					string send_buf = "Invalid Choice";
					std::cout << "[Server]: " << send_buf << "\n";
				}
			}


			while(true){

				string mainMenu = "\n***********************************\n"
						"Main Menu:\n"
				        "1)  Add a new appointment\n"
				        "2)  Remove an appointment\n"
						"3)  Update an existing appointment\n"
						"4)  Display appointment at a time\n"
						"5)  Display appointments in a time range\n"
						"6)  Change name\n"
						"7)  Change password\n"
						"8)  Change phone number\n"
						"9)  Change email\n"
						"10) Delete a user\n"
						"11) Exit\n"
						"***********************************\n";
				string send_buf = mainMenu;
				if(send(new_fd, send_buf.c_str(), 512, 0) == -1){
					perror("send");
					close(new_fd);
					exit(1);
				}
				std::cout <<"[Server]: "<< send_buf << "\n";

				//Recieve client response
				numbytes=recv(new_fd, recvbuf, 127, 0);
				recvbuf[numbytes] = '\0';
				std::cout << "[Client]: " << recvbuf << "\n";

				std::string choice = recvbuf;

				if(!choice.compare("1")){

					menu.addAppointment(new_fd, recvbuf, user);
				}
				else if(!choice.compare("2")){

					menu.deleteAppointment(new_fd, recvbuf, user);
				}
				else if(!choice.compare("3")){

					menu.updateAppointment(new_fd, recvbuf, user);

				}
				else if(!choice.compare("4")){

					menu.displayAppointTime(new_fd, recvbuf, user);
				}
				else if(!choice.compare("5")){

					menu.displayAppointRange(new_fd, recvbuf, user);
				}
				else if(!choice.compare("6")) {

					menu.changeName(new_fd, recvbuf, user);
				}
				else if(!choice.compare("7")){

					menu.changePassword(new_fd, recvbuf, user);
				}
				else if(!choice.compare("8")){

					menu.changePhone(new_fd, recvbuf, user);
				}
				else if(!choice.compare("9")){

					menu.changeEmail(new_fd, recvbuf, user);
				}
				else if(!choice.compare("10")){

					menu.deleteUser(new_fd, recvbuf, user);
				}
				else if(!choice.compare("11") || !choice.compare("k")){
					user.write();
					close(new_fd);
					_Exit(0);
				}
				else{}

			}
		}
		close(new_fd);
	}

	user.write();
	return 0;
}
