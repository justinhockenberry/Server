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

struct userAppointment{
	string username;
	string date;
	string reason;
	string time;
};


class User {

private:
	string username;
	string password;
	string name;
	string phone;
	string email;

	map<string, userAppointment> appointmentTable;
	const int REASON_POS = 1;
	const int USERNAME_POS = 0;
	const int DATE_POS = 2;
	const int TIME_POS = 3;

public:
	User();
	virtual ~User();

	const string REASON = "reason";
	const string DATE = "date";
	const string TIME = "time";

	void populate(string username);
	bool exists(string username);

	void removeAppointment(string username, string date, string time);
	string sendAllAppointments(string username);
	string readPassword(string);

	//User
	const string EMAIL = "email";
	const string NAME = "name";
	const string PHONE = "phone";

	void print();
	void write();
	void remove();
	string readAppointment(string date, string time);
	int conflictCheck(string date, string time, string username);
	void createAppointment(string username, string reason, string date, string time);
	string rangeReturnAppointments(string start, string end);

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
