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
using std::cout;
using std::endl;


#include "MenuOptions.h"
#include "User.h"


MenuOptions::MenuOptions() {
}

MenuOptions::~MenuOptions() {
}

bool MenuOptions::login(int new_fd, char *recvbuf, User &user, bool loggedIn) {

	string send_buf;
	string username;
	string password;
	long numbytes;

	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	cout << "[Client]: " << recvbuf << "\n";
	username = recvbuf;

	//receive password
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	cout << "[Client]: " << recvbuf << "\n";
	password = recvbuf;

	//Login Successful
	if(user.findUserFile(username)){
		user.getUserFileInfo(username);
		if (!user.getPassword().compare(password)) {
			send_buf = "Success";
			send(new_fd, send_buf.c_str(), 127, 0);
			cout << "[Server]: " << send_buf << "\n";
			loggedIn = true;
		}
		else{ // Login Failure
			send_buf = "Failure";
			send(new_fd, send_buf.c_str(), 127, 0);
			cout << "[Server]: " << send_buf << "\n";
		}
	}else{ // Login Failure
		send_buf = "Failure";
		send(new_fd, send_buf.c_str(), 127, 0);
		cout << "[Server]: " << send_buf << "\n";
	}

	return loggedIn;

}

bool MenuOptions::newAccount(int new_fd, char *recvbuf, User &user, bool loggedIn) {

	bool exists = true;
	string send_buf;
	string username;
	string password;
	long numbytes;

	while(exists){
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		cout << "[Client]: " << recvbuf << "\n";
		username = recvbuf;

		if(!user.findUserFile(username)){
			send_buf = "Success";
			send(new_fd, send_buf.c_str(), 127, 0);
			cout << "[Server]: " << send_buf << "\n";
			exists = false;
		}
		else{
			send_buf = "Failure";
			send(new_fd, send_buf.c_str(), 127, 0);
			cout << "[Server]: " << send_buf << "\n";
		}
	}

	user.setUsername(username);

	//receive password
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	cout << "[Client]: " << recvbuf << "\n";
	password = recvbuf;
	user.setPassword(password);

	//receive name
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	cout << "[Client]: " << recvbuf << "\n";
	string name = recvbuf;
	user.setName(name);

	//receive email
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	cout << "[Client]: " << recvbuf << "\n";
	string email = recvbuf;
	user.setEmail(email);

	//receive phone number
	numbytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	cout << "[Client]: " << recvbuf << "\n";
	string phone = recvbuf;
	user.setPhone(phone);

	user.updateToFile();

	return loggedIn = true;
}

void MenuOptions::addAppointment(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	string username;
	long numbytes;

	send_buf = user.getUserAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	string beginTime;
	string endTime;
	int conflict = 1;
	//Check for conflicting appointments
	while(conflict) {
		numbytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		beginTime = recvbuf;

		numbytes=recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		endTime = recvbuf;


		conflict = user.findDuplicateUserFiles(beginTime, username);
		if (conflict) {
			send_buf = "Failure";
			cout << send_buf << endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		} else {
			send_buf = "Success";
			cout << send_buf << endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		}

	}


	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string memo = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string place = recvbuf;

	user.addAppointment(username, memo, beginTime, endTime, place);

	send_buf = "Success added appointment at: " + beginTime + " to " + endTime + " at " + place +" for "+ memo;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.updateToFile();

}

void MenuOptions::deleteAppointment(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	string username;
	long numbytes;

	send_buf = user.getUserAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string beginTime = recvbuf;

	user.removeAppointment(username, beginTime);

	send_buf = "Success removed appointment at: " + beginTime;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.updateToFile();

}

void MenuOptions::updateAppointment(int new_fd, char *recvbuf, User &user) {
	string send_buf;
	string username;
	long numbytes;

	send_buf = user.getUserAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	string beginTime;
	string endTime;
	int conflict = 1;
	//Check for conflicting appointments
	while(conflict){
		numbytes=recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		beginTime = recvbuf;

		numbytes=recv(new_fd, recvbuf, 127, 0);
		recvbuf[numbytes] = '\0';
		endTime = recvbuf;

		conflict = user.findDuplicateUserFiles(beginTime, username);
		if(conflict){
			send_buf = "Failure";
			cout << send_buf << endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		}else{
			send_buf = "Success";
			cout << send_buf <<endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		}

	}

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string memo = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string place = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string oldBeginTime = recvbuf;


	user.removeAppointment(username, oldBeginTime);

	user.addAppointment(username, memo, beginTime, endTime, place);

	send_buf = "Success updated appointment to: " + beginTime + " to " + endTime +" at " + place + " for "+ memo;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.updateToFile();
}

void MenuOptions::displayAppointTime(int new_fd, char *recvbuf, User &user) {
	string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string beginTime = recvbuf;

//	numbytes=recv(new_fd, recvbuf, 127, 0);
//	recvbuf[numbytes] = '\0';
//	string appTime = recvbuf;

	send_buf = user.getAppointment(beginTime);
	send(new_fd, send_buf.c_str(), 127, 0);
}

void MenuOptions::displayAppointRange(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string startDate = recvbuf;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string endDate = recvbuf;

	send_buf = "Found these appointments in range: " + startDate + " to " + endDate +"\n"
			+ user.getAppointmentRange(startDate, endDate);
	send(new_fd, send_buf.c_str(), 512, 0);

	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
}
void MenuOptions::changeName(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	user.setName(recvbuf);
	user.updateToFile();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	cout << "Modified name: "<< recvbuf << endl;
}
void MenuOptions::changePassword(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	user.setPassword(recvbuf);
	user.deleteUserFile();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	cout << "Modified password: "<< recvbuf << endl;

}
void MenuOptions::changePhone(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	user.setPhone(recvbuf);
	user.updateToFile();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	cout << "Modified phone number: "<< recvbuf << endl;

}
void MenuOptions::changeEmail(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	user.setEmail(recvbuf);
	user.updateToFile();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	cout << "Modified email: "<< recvbuf << endl;
}
void MenuOptions::deleteUser(int new_fd, char *recvbuf, User &user) {
	string send_buf;
	long numbytes;

	numbytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[numbytes] = '\0';
	string check = recvbuf;

	if(!check.compare("Y") || !check.compare("y")){
		user.deleteUserFile();
		send_buf = "Success";
		send(new_fd, send_buf.c_str(), 127, 0);
		_Exit(0);
	}else{
		send_buf = "Cancelled";
		send(new_fd, send_buf.c_str(), 127, 0);
	}
}



























