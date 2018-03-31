/*
 * MenuOptions.cpp
 *
 *  Created on: Mar 31, 2018
 *      Author: justin
 */

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstdlib>
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

using std::string;

#include "MenuOptions.h"
#include "User.h"


MenuOptions::MenuOptions() {
}

MenuOptions::~MenuOptions() {
}

bool MenuOptions::login(int new_fd, char *recvbuf, User user, bool loggedIn) {

	std::string send_buf;
	std::string username;
	std::string password;
	long numbytes;

	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::cout << "[Client]: " << recvbuf << "\n";
	username = recvbuf;

	//receive password
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::cout << "[Client]: " << recvbuf << "\n";
	password = recvbuf;

	//Login Successful
	if(user.exists(username)){
		user.populate(username);
		if (!user.readPassword(username).compare(password)) {
			send_buf = "Success";
			send(new_fd, send_buf.c_str(), 127, 0);
			std::cout << "[Server]: " << send_buf << "\n";
			loggedIn = true;
		}
		else{ // Login Failure
			send_buf = "Failure";
			send(new_fd, send_buf.c_str(), 127, 0);
			std::cout << "[Server]: " << send_buf << "\n";
		}
	}else{ // Login Failure
		send_buf = "Failure";
		send(new_fd, send_buf.c_str(), 127, 0);
		std::cout << "[Server]: " << send_buf << "\n";
	}

	return loggedIn;

}

bool MenuOptions::newAccount(int new_fd, char *recvbuf, User user, bool loggedIn) {

	bool exists = true;
	std::string send_buf;
	std::string username;
	std::string password;
	long numbytes;

	while(exists){
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		std::cout << "[Client]: " << recvbuf << "\n";
		username = recvbuf;

		if(!user.exists(username)){
			send_buf = "Success";
			send(new_fd, send_buf.c_str(), 127, 0);
			std::cout << "[Server]: " << send_buf << "\n";
			exists = false;
		}
		else{
			send_buf = "Failure";
			send(new_fd, send_buf.c_str(), 127, 0);
			std::cout << "[Server]: " << send_buf << "\n";
		}
	}

	user.setUsername(username);

	//receive password
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::cout << "[Client]: " << recvbuf << "\n";
	password = recvbuf;
	user.setPassword(password);

	//receive name
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::cout << "[Client]: " << recvbuf << "\n";
	std::string name = recvbuf;
	user.setName(name);

	//receive email
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::cout << "[Client]: " << recvbuf << "\n";
	std::string email = recvbuf;
	user.setEmail(email);

	//receive phone number
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::cout << "[Client]: " << recvbuf << "\n";
	std::string phone = recvbuf;
	user.setPhone(phone);

	user.write();

	return loggedIn = true;
}

void MenuOptions::addAppointment(int new_fd, char *recvbuf, User user) {

	std::string send_buf;
	std::string username;
	long numbytes;

	send_buf = user.sendAllAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	std::string appDate;
	std::string appTime;
	int conflict = 1;
	//Check for conflicting appointments
	while(conflict) {
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		appDate = recvbuf;

		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		appTime = recvbuf;

		conflict = user.conflictCheck(appDate, appTime, username);
		if (conflict) {
			send_buf = "Failure";
			std::cout << send_buf << std::endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		} else {
			send_buf = "Success";
			std::cout << send_buf << std::endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		}

	}
	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string appReason = recvbuf;

	user.createAppointment(username, appReason, appDate, appTime);

	send_buf = "Success added appointment at: " + appDate + " at " + appTime + " for "+ appReason;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.write();

}

void MenuOptions::deleteAppointment(int new_fd, char *recvbuf, User user) {

	std::string send_buf;
	std::string username;
	long numbytes;

	send_buf = user.sendAllAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string appDate = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string appTime = recvbuf;

	user.removeAppointment(username, appDate, appTime);

	send_buf = "Success removed appointment at: " + appDate + " " + appTime;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.write();

}

void MenuOptions::updateAppointment(int new_fd, char *recvbuf, User user) {
	std::string send_buf;
	std::string username;
	long numbytes;

	send_buf = user.sendAllAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	std::string appDate;
	std::string appTime;
	int conflict = 1;
	//Check for conflicting appointments
	while(conflict){
		numbytes=recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		appDate = recvbuf;

		numbytes=recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		appTime = recvbuf;

		conflict = user.conflictCheck(appDate, appTime, username);
		if(conflict){
			send_buf = "Failure";
			std::cout << send_buf << std::endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		}else{
			send_buf = "Success";
			std::cout << send_buf <<std::endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		}

	}

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string appReason = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string oldAppTime = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string oldAppDate = recvbuf;

	user.removeAppointment(username, oldAppDate, oldAppTime);

	user.createAppointment(username, appReason, appDate, appTime);

	send_buf = "Success updated appointment to: " + appDate + " at " + appTime + " for "+ appReason;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.write();
}

void MenuOptions::displayAppointTime(int new_fd, char *recvbuf, User user) {
	std::string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string appDate = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string appTime = recvbuf;

	send_buf = user.readAppointment(appDate, appTime);
	send(new_fd, send_buf.c_str(), 127, 0);
}

void MenuOptions::displayAppointRange(int new_fd, char *recvbuf, User user) {

	std::string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string startDate = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string endDate = recvbuf;

	send_buf = "Found these appointments in range: " + startDate + " to " + endDate +"\n"
			+ user.rangeReturnAppointments(startDate, endDate);
	send(new_fd, send_buf.c_str(), 512, 0);

	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
}
void MenuOptions::changeName(int new_fd, char *recvbuf, User user) {

	std::string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	user.setName(recvbuf);
	user.write();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	std::cout << "Modified name: "<< recvbuf << std::endl;
}
void MenuOptions::changePassword(int new_fd, char *recvbuf, User user) {

	std::string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	user.setPassword(recvbuf);
	user.write();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	std::cout << "Modified password: "<< recvbuf << std::endl;

}
void MenuOptions::changePhone(int new_fd, char *recvbuf, User user) {

	std::string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	user.setPhone(recvbuf);
	user.write();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	std::cout << "Modified phone number: "<< recvbuf << std::endl;

}
void MenuOptions::changeEmail(int new_fd, char *recvbuf, User user) {

	std::string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	user.setEmail(recvbuf);
	user.write();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	std::cout << "Modified email: "<< recvbuf << std::endl;
}
void MenuOptions::deleteUser(int new_fd, char *recvbuf, User user) {
	std::string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	std::string check = recvbuf;
	if(!check.compare("Y") || !check.compare("y")){
		user.remove();
		send_buf = "Success";
		send(new_fd, send_buf.c_str(), 127, 0);
		_Exit(0);
	}else{
		send_buf = "Cancelled";
		send(new_fd, send_buf.c_str(), 127, 0);
	}
}



























