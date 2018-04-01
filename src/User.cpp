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
		newAppointment.date = fields[DATE_POS];
		newAppointment.time = fields[TIME_POS];
		newAppointment.reason = fields[REASON_POS];

		appointmentTable[fields[DATE_POS]+fields[TIME_POS]] = newAppointment;

	}

	infile.close();

}


//void User::print() {
//
//	cout << this->username << "\n" << this->password <<  "\n" << this->name << "\n" << this->phone << "\n"
//			<< this->email << "\n";
//	//    for(pair<string , userAppointment>  i: appointmentTable){
//	//        cout << i.second.date << " " << i.second.time << " " << i.second.reason << "\n";
//	//    }
//}

void User::write() {
	string filename = this->username + ".txt";
	ofstream ofstream(filename.c_str());
	ofstream << this->username << "\n" << this->password <<  "\n" << this->name << "\n" << this->phone << "\n"
			<< this->email << "\n";
	    for(std::pair<string , userAppointment>  i: appointmentTable){
	        ofstream << this->username << ";" << i.second.reason << ";" << i.second.date << ";" << i.second.time << "\n";
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


int User::conflictCheck(string date, string time, string username) {
	return appointmentTable.count(date+time);
}


void User::createAppointment(string username, string reason, string date, string time) {
	appointmentTable[date+time].reason = reason;
	appointmentTable[date+time].date = date;
	appointmentTable[date+time].time = time;
	appointmentTable[date+time].username= username;
}


void User::removeAppointment(string username, string date, string time) {
	appointmentTable.erase(date+time);
}


string User::sendAllAppointments(string username) {
	string appointments;
	    for(std::pair<string, userAppointment> appointment : appointmentTable ){

	        appointments += (appointment.second.reason + " on " + appointment.second.date
	                         + " at " + appointment.second.time + "\n");
	    }

	return appointments;}


string User::readAppointment(string date, string time) {

	return "On " + this->appointmentTable[date+time].date +" at " +
			this->appointmentTable[date+time].time + " for " +
			this->appointmentTable[date+time].reason;

}


string User::rangeReturnAppointments(string start, string end) {
	string appointments = "";
	stringstream startStream(start);
	stringstream endStream(end);
	string startMonth;
	string endMonth;
	string startDay;
	string endDay;
	string startYear;
	string endYear;

	getline(startStream, startMonth, '/');
	getline(startStream, startDay, '/');
	getline(startStream, startYear);

	getline(endStream, endMonth, '/');
	getline(endStream, endDay, '/');
	getline(endStream, endYear);

	    int startDate = stoi(startDay) + stoi(startMonth) * 100 + stoi(startYear) *10000;
	    int endDate = stoi(endDay) + stoi(endMonth) * 100 + stoi(endYear) *10000;

	    for(std::pair<string, userAppointment> appointment : this->appointmentTable ){
	        stringstream date(appointment.second.date);
	        string day;
	        string month;
	        string year;

	        getline(date, month, '/');
	        getline(date, day, '/');
	        getline(date, year);

	        int dateValue = stoi(day) + stoi(month) * 100 + stoi(year) *10000;

	        if(dateValue >= startDate && dateValue <= endDate){
	            appointments += (appointment.second.reason + " on " + appointment.second.date
	                             + " at " + appointment.second.time + "\n");
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

