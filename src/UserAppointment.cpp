/*
 * UserAppointment.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */

#include <string>

#include "UserAppointment.h"



UserAppointment::UserAppointment() {
	// TODO Auto-generated constructor stub

}

UserAppointment::~UserAppointment() {
	// TODO Auto-generated destructor stub
}

const std::string &getBeginTime() {
	return UserAppointment::beginTime;
}
void setBeginTime(const std::string &beginTime) {
	UserAppointment::beginTime = beginTime;
}
const std::string &getEndTime() {
	return UserAppointment::endTime;
}
void setEndTime(const std::string &endTime) {
	UserAppointment::endTime = endTime;
}
const std::string &getPlace() {
	return UserAppointment::place;
}
void setPlace(const std::string &place) {
	UserAppointment::place = place;
}
const std::string &geMemo() {
	return UserAppointment::memo;
}
void setMemo(const std::string &memo) {
	UserAppointment::memo = memo;
}
const std::string &getName() {
	return UserAppointment::name;
}
void setName(const std::string &name) {
	UserAppointment::name = name;
}







