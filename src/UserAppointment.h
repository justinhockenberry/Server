/*
 * UserAppointment.h
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */

#ifndef USERAPPOINTMENT_H_
#define USERAPPOINTMENT_H_

using std::string;
class UserAppointment {

private:
	string beginTime;
	string endTime;
	string place;
	string memo;
	string name;


public:
	UserAppointment();
	virtual ~UserAppointment();

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

#endif /* USERAPPOINTMENT_H_ */
