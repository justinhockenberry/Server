/*
 * User.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */


#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "User.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::stringstream;
using std::vector;
using std::cout;



User::User() {


}

User::~User() {

}

void User::populate(string username) {

	string filename = username+".txt";

	ifstream infile(filename.c_str());
	getline(infile, this->username);
	getline(infile, this->password);
	getline(infile, this->name);
	getline(infile, this->phone);
	getline(infile, this->email);

	string entry;
	while(getline(infile, entry)){

		stringstream stream(entry);
		vector<string> fields;
		string token;

		while(getline(stream, token, ';')){
			fields.push_back(token);
		}

		userAppointment newAppointment;

		newAppointment.username = fields[USERNAME_POS];
		newAppointment.beginTime = fields[BEGIN_POS];
		newAppointment.endTime = fields[END_POS];
		newAppointment.memo = fields[MEMO_POS];
		newAppointment.place = fields[PLACE_POS];


		appointmentTable[fields[BEGIN_POS]] = newAppointment;

	}

	infile.close();

}

void User::write() {
	string filename = this->username + ".txt";
	ofstream ofstream(filename.c_str());
	ofstream << this->username << "\n" << this->password <<  "\n" << this->name << "\n" << this->phone << "\n"
			<< this->email << "\n";
	    for(std::pair<string , userAppointment>  i: appointmentTable){
	        ofstream << this->username << ";" << i.second.beginTime << ";" << i.second.endTime << ";" << i.second.memo << ";" << i.second.place <<"\n";
	    }
	ofstream.close();
}


string User::readPassword(string) {
	return this->password;
}


bool User::exists(string username) {
	string filename = username + ".txt";
	ifstream infile(filename.c_str());
	return infile.is_open();
}


void User::remove() {
	string fileName = this->username + ".txt";
	std::remove(fileName.c_str());
}


int User::conflictCheck(string beginTime, string username) {
	return appointmentTable.count(beginTime);
}


void User::createAppointment(string username, string memo, string beginTime, string endTime, string place) {
	appointmentTable[beginTime].memo = memo;
	appointmentTable[beginTime].beginTime = beginTime;
	appointmentTable[beginTime].endTime = endTime;
	appointmentTable[beginTime].place = place;
	appointmentTable[beginTime].username = username;
}


void User::removeAppointment(string username, string beginTime) {
	appointmentTable.erase(beginTime);
}


string User::sendAllAppointments(string username) {
	string appointments;
	    for(std::pair<string, userAppointment> appointment : appointmentTable ){

	        appointments += (appointment.second.memo + " from " + appointment.second.beginTime + " to " + appointment.second.endTime
	                         + " located at " + appointment.second.place + "\n");
	    }

	return appointments;
}


string User::readAppointment(string beginTime) {

	return "On " + this->appointmentTable[beginTime].beginTime +" at " +
			this->appointmentTable[beginTime].place + " for " +
			this->appointmentTable[beginTime].memo;

}


string User::rangeReturnAppointments(string start, string end) {
	string appointments = "";
	stringstream startStream(start);
	stringstream endStream(end);
	string startTime;
	string endTime;
	string startMonth;
	string endMonth;
	string startDay;
	string endDay;
	string startYear;
	string endYear;

	getline(startStream, startTime, ' ');
	getline(startStream, startMonth, '/');
	getline(startStream, startDay, '/');
	getline(startStream, startYear);

	getline(endStream, endTime, ' ');
	getline(endStream, endMonth, '/');
	getline(endStream, endDay, '/');
	getline(endStream, endYear);

	    int startDate = stoi(startDay) + stoi(startMonth) * 100 + stoi(startYear) *10000;
	    int endDate = stoi(endDay) + stoi(endMonth) * 100 + stoi(endYear) *10000;

	    for(std::pair<string, userAppointment> appointment : this->appointmentTable ){

	        stringstream date(appointment.second.beginTime);

	        string time;
	        string day;
	        string month;
	        string year;

	        getline(date, time, ' ');
	        getline(date, month, '/');
	        getline(date, day, '/');
	        getline(date, year);

	        int dateValue = stoi(day) + stoi(month) * 100 + stoi(year) *10000;

	        if(dateValue >= startDate && dateValue <= endDate){
	            appointments += (appointment.second.memo + " from " + appointment.second.beginTime
	                             + " to " + appointment.second.endTime + "\n");
	        }

	    }
	cout << appointments;
	return appointments;
}


void User::setUsername(string input) {
	username = input;
}
void User::setPassword(string input) {
	password = input;
}
void User::setName(string input) {
	name = input;
}
void User::setPhone(string input) {
	phone = input;
}
void User::setEmail(string input) {
	email = input;
}

