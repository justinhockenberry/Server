/*
 * UserAppointment.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */

#include "Appointment.h"

#include <string>


using string;

UserAppointment::UserAppointment() {
	// TODO Auto-generated constructor stub

}

UserAppointment::~UserAppointment() {
	// TODO Auto-generated destructor stub
}



void UserAppointment::setBeginTime(string input) {
	beginTime = input;
}
void UserAppointment::setEndTime(string input) {
	endTime = input;
}
void UserAppointment::setMemo(string input) {
	memo = input;
}
void UserAppointment::setName(string input) {
	name = input;
}





