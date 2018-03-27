/*
 * UserAppointment.h
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */

#ifndef USERAPPOINTMENT_H_
#define USERAPPOINTMENT_H_


class UserAppointment {

private:
	std::string beginTime;
	std::string endTime;
	std::string place;
	std::string memo;
	std::string name;


public:
	UserAppointment();
	virtual ~UserAppointment();


	const std::string &getBeginTime();
	void setBeginTime(const std::string &beginTime);
	const std::string &getEndTime();
	void setEndTime(const std::string &endTime);
	const std::string &getPlace();
	void setPlace(const std::string &place);
	const std::string &geMemo();
	void setMemo(const std::string &memo);
	const std::string &getName();
	void setName(const std::string &name);
};

#endif /* USERAPPOINTMENT_H_ */
