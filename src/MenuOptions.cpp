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
#include "md5.h"

MenuOptions::MenuOptions() {
}

MenuOptions::~MenuOptions() {
}

bool MenuOptions::login(int new_fd, char *recvbuf, User &user, bool loggedIn) {

	string send_buf;
	string username;
	string password;
	long bytes;

	bytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	cout << "Username: " << recvbuf << "\n";
	username = recvbuf;

	bytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	cout << "Password: " << recvbuf << "\n";
	password = recvbuf;

	string encryptedPassword = encryptPassword(password);

	if(user.findUserFile(username)){
		user.getUserFileInfo(username);
		if (!user.getPassword().compare(encryptedPassword)) {
			send_buf = "Success";
			send(new_fd, send_buf.c_str(), 127, 0);
			cout << "Login: " << send_buf << "\n";
			loggedIn = true;
		}
		else{
			send_buf = "Failure";
			send(new_fd, send_buf.c_str(), 127, 0);
			cout << "Login: " << send_buf << "\n";
		}
	}else{
		send_buf = "Failure";
		send(new_fd, send_buf.c_str(), 127, 0);
		cout << "Login: " << send_buf << "\n";
	}

	return loggedIn;

}

bool MenuOptions::newAccount(int new_fd, char *recvbuf, User &user, bool loggedIn) {

	bool exists = true;
	string send_buf;
	string username;
	string password;
	long bytes;

	while(exists){
		bytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[bytes] = '\0';
		cout << "New Account Username: " << recvbuf << "\n";
		username = recvbuf;

		if(!user.findUserFile(username)){
			send_buf = "New Username accepted";
			send(new_fd, send_buf.c_str(), 127, 0);
			cout << send_buf << "\n";
			exists = false;
		}
		else{
			send_buf = "New Username failed";
			send(new_fd, send_buf.c_str(), 127, 0);
			cout << send_buf << "\n";
		}
	}

	user.setUsername(username);

	bytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	cout << "New Account Password: " << recvbuf << "\n";
	password = recvbuf;

	string encryptedPassword = encryptPassword(password);

	user.setPassword(encryptedPassword);

	bytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	cout << "New Account Name: " << recvbuf << "\n";
	string name = recvbuf;
	user.setName(name);

	bytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	cout << "New Account Name: " << recvbuf << "\n";
	string email = recvbuf;
	user.setEmail(email);

	bytes = recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	cout << "New Account Phone Number: " << recvbuf << "\n";
	string phone = recvbuf;
	user.setPhone(phone);

	user.updateToFile();

	return loggedIn = true;
}
string MenuOptions::encryptPassword(string password) {

	string secretString = "SuperSecretHashingString";
	string encryptedPassword = md5(password + secretString);
	return encryptedPassword;
}

void MenuOptions::addAppointment(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	string username;
	long bytes;

	send_buf = user.getUserAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	string beginTime;
	string endTime;
	int conflict = 1;


	while(conflict) {
		bytes = recv(new_fd, recvbuf, 127, 0);
		recvbuf[bytes] = '\0';
		beginTime = recvbuf;

		bytes=recv(new_fd, recvbuf, 127, 0);
		recvbuf[bytes] = '\0';
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


	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string memo = recvbuf;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string place = recvbuf;

	user.addAppointment(username, memo, beginTime, endTime, place);

	send_buf = "Added appointment at: " + beginTime + " to " + endTime + " at " + place +" for "+ memo;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.updateToFile();

}

void MenuOptions::deleteAppointment(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	string username;
	long bytes;

	send_buf = user.getUserAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string beginTime = recvbuf;

	user.removeAppointment(username, beginTime);

	send_buf = "Deleted appointment at: " + beginTime;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.updateToFile();

}

void MenuOptions::updateAppointment(int new_fd, char *recvbuf, User &user) {
	string send_buf;
	string username;
	long bytes;

	send_buf = user.getUserAppointments(username);
	send(new_fd, send_buf.c_str(), 512, 0);

	string beginTime;
	string endTime;
	int conflict = 1;

	while(conflict){
		bytes=recv(new_fd, recvbuf, 127, 0);
		recvbuf[bytes] = '\0';
		beginTime = recvbuf;

		bytes=recv(new_fd, recvbuf, 127, 0);
		recvbuf[bytes] = '\0';
		endTime = recvbuf;

		conflict = user.findDuplicateUserFiles(beginTime, username);
		if(conflict){
			send_buf = "Update failed";
			cout << send_buf << endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		}else{
			send_buf = "Update successful";
			cout << send_buf <<endl;
			send(new_fd, send_buf.c_str(), 127, 0);
		}

	}

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string memo = recvbuf;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string place = recvbuf;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string oldBeginTime = recvbuf;


	user.removeAppointment(username, oldBeginTime);

	user.addAppointment(username, memo, beginTime, endTime, place);

	send_buf = "Updated appointment to: " + beginTime + " to " + endTime +" at " + place + " for "+ memo;
	send(new_fd, send_buf.c_str(), 127, 0);

	user.updateToFile();
}

void MenuOptions::displayAppointTime(int new_fd, char *recvbuf, User &user) {
	string send_buf;
	long bytes;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string beginTime = recvbuf;

	send_buf = user.getAppointment(beginTime);
	send(new_fd, send_buf.c_str(), 127, 0);
}

void MenuOptions::displayAppointRange(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long bytes;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string startDate = recvbuf;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string endDate = recvbuf;

	send_buf = "Found these appointments in range: " + startDate + " to " + endDate +"\n"
			+ user.getAppointmentRange(startDate, endDate);
	send(new_fd, send_buf.c_str(), 512, 0);

	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
}
void MenuOptions::changeName(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long bytes;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	user.setName(recvbuf);
	user.updateToFile();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	cout << "Changed name to: "<< recvbuf << endl;
}
void MenuOptions::changePassword(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long bytes;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	user.setPassword(recvbuf);
	user.deleteUserFile();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	cout << "Changed  password to: "<< recvbuf << endl;

}
void MenuOptions::changePhone(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long bytes;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	user.setPhone(recvbuf);
	user.updateToFile();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	cout << "Changed phone number to: "<< recvbuf << endl;

}
void MenuOptions::changeEmail(int new_fd, char *recvbuf, User &user) {

	string send_buf;
	long bytes;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	user.setEmail(recvbuf);
	user.updateToFile();
	send_buf = "Success";
	send(new_fd, send_buf.c_str(), 127, 0);
	cout << "Changed email to: "<< recvbuf << endl;
}
void MenuOptions::deleteUser(int new_fd, char *recvbuf, User &user) {
	string send_buf;
	long bytes;

	bytes=recv(new_fd, recvbuf, 127, 0);
	recvbuf[bytes] = '\0';
	string check = recvbuf;

	if(!check.compare("Y") || !check.compare("y")){
		user.deleteUserFile();
		send_buf = "Deleted user account";
		send(new_fd, send_buf.c_str(), 127, 0);
		_Exit(0);
	}else{
		send_buf = "Deletion cancelled";
		send(new_fd, send_buf.c_str(), 127, 0);
	}
}



























