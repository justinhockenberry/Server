/*
 * MenuOptions.h
 *
 *  Created on: Mar 31, 2018
 *      Author: justin
 */

#ifndef MENUOPTIONS_H_
#define MENUOPTIONS_H_


#include "User.h"

class MenuOptions {
public:
	MenuOptions();
	virtual ~MenuOptions();

	bool login(int new_fd, char *recvbuf, User &user, bool loggedIn);
	bool newAccount(int new_fd, char *recvbuf, User &user, bool loggedIn);

	void addAppointment(int new_fd, char *recvbuf, User &user);
	void deleteAppointment(int new_fd, char *recvbuf, User &user);
	void updateAppointment(int new_fd, char *recvbuf, User &user);

	void displayAppointTime(int new_fd, char *recvbuf, User &user);
	void displayAppointRange(int new_fd, char *recvbuf, User &user);

	void changeName(int new_fd, char *recvbuf, User &user);
	void changePassword(int new_fd, char *recvbuf, User &user);
	void changePhone(int new_fd, char *recvbuf, User &user);
	void changeEmail(int new_fd, char *recvbuf, User &user);
	void deleteUser(int new_fd, char *recvbuf, User &user);
};

#endif /* MENUOPTIONS_H_ */
