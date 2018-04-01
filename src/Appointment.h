/*
 * UserAppointment.h
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */

#ifndef APPOINTMENT_H_
#define APPOINTMENT_H_

#include <string>
using std::string;

class Appointment {

private:
	string beginTime;
	string endTime;
	string place;
	string memo;
	string name;


public:
	Appointment();
	virtual ~Appointment();

	void setBeginTime(string beginTime);
	void setEndTime(string endTime);
	void setPlace(string place);
	void setMemo(string memo);
	void setName(string name);


	const string getBeginTime() {
		return beginTime;
	}
	const string getEndTime() {
		return endTime;
	}
	const string getPlace() {
		return place;
	}
	const string getMemo() {
		return memo;
	}
	const string getName() {
		return name;
	}
};

#endif /* APPOINTMENT_H_ */
