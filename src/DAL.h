/*
 * DAL.h
 *
 *  Created on: Mar 24, 2018
 *      Author: justin
 */

#ifndef SERVER_DAL_H
#define SERVER_DAL_H


//#include "DAO.h"
#include <string>
#include <map>

struct userAppointment{
    std::string username;
    std::string date;
    std::string reason;
    std::string time;
};

class DAL {


private:
    std::string username;
    std::string password;
    std::string name;
    std::string phone;
    std::string email;
    std::map<std::string, userAppointment> appointmentTable;
    const int REASON_POS = 1;
    const int USERNAME_POS = 0;
    const int DATE_POS = 2;
    const int TIME_POS = 3;

public:
    const std::string REASON = "reason";
    const std::string DATE = "date";
    const std::string TIME = "time";

    void populate(std::string username);
    bool exists(std::string username);

    void removeAppointment(std::string username, std::string date, std::string time);
    std::string sendAllAppointments(std::string username);
    std::string readPassword(std::string);

    //User
    const std::string EMAIL = "email";
    const std::string NAME = "name";
    const std::string PHONE = "phone";

    void print();
    void write();
    void remove();
    std::string readAppointment(std::string date, std::string time);
    int conflictCheck(std::string date, std::string time, std::string username);
    void createAppointment(std::string username, std::string reason, std::string date, std::string time);
    std::string rangeReturnAppointments(std::string start, std::string end);




    const std::string &getUsername() const {
        return username;
    }

    void setUsername(const std::string &username) {
        DAL::username = username;
    }

    const std::string &getPassword() const {
        return password;
    }

    void setPassword(const std::string &password) {
        DAL::password = password;
    }

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        DAL::name = name;
    }

    const std::string &getPhone() const {
        return phone;
    }

    void setPhone(const std::string &phone) {
        DAL::phone = phone;
    }

    const std::string &getEmail() const {
        return email;
    }

    void setEmail(const std::string &email) {
        DAL::email = email;
    }
};


#endif //SERVER_DAL_H
