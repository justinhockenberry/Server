/*
 * User.h
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */

#ifndef USER_H_
#define USER_H_

#include <string>
#include <map>

using std::string;
using std::map;

struct appointment{
	string beginTime;
	string endTime;
	string place;
	string memo;
	string username;
};


class User {

private:
	string username;
	string password;
	string name;
	string phone;
	string email;

public:

	User();
	virtual ~User();

	string getAppointment(string beginTime);
	string getUserAppointments(string username);
	string getAppointmentRange(string start, string end);
	map<string, appointment> appointmentMap;
	void addAppointment(string username, string memo, string beginTime, string endTime, string place);
	void updateToFile();
	void deleteUserFile();
	void getUserFileInfo(string username);
	void removeAppointment(string username, string beginTime);
	bool findUserFile(string username);
	int findDuplicateUserFiles(string beginTime, string username);

	// Getters
	const string getUsername() {
		return username;
	}
	const string getPassword() {
		return password;
	}
	const string getName() {
		return name;
	}
	const string getPhone() {
		return phone;
	}
	const string getEmail() {
		return email;
	}

	//Setters
	void setUsername(string username);
	void setPassword(string password);
	void setName(string name);
	void setPhone(string phone);
	void setEmail(string email);

};




#endif /* USER_H_ */
