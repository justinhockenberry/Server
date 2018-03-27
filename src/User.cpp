/*
 * User.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */

#include "User.h"

User::User() {
	// TODO Auto-generated constructor stub

}

User::~User() {
	// TODO Auto-generated destructor stub
}

const std::string &getName() const {
	return User::name;
}
void setName(const std::string &name) {
	User::name = name;
}
const std::string &getPassword() const {
	return User::password;
}
void setPassword(const std::string &password) {
	User::password = password;
}
const std::string &getPhone() const {
	return User::phone;
}
void setPhone(const std::string &phone) {
	User::phone = phone;
}
const std::string &getEmail() const {
	return User::email;
}
void setEmail(const std::string &email) {
	User::email = email;
}

