/*
 * User.h
 *
 *  Created on: Mar 27, 2018
 *      Author: justin
 */

#ifndef USER_H_
#define USER_H_

#include <string>

class User {

private:
	std::string name;
	std::string password;
	std::string phone;
	std::string email;

public:
	User();
	virtual ~User();

	const std::string &getName();
	void setName(const std::string &name);
	const std::string &getPassword();
	void setPassword(const std::string &password);
	const std::string &getPhone();
	void setPhone(const std::string &phone);
	const std::string &getEmail();
	void setEmail(const std::string &email);

};




#endif /* USER_H_ */
