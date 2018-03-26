/*
 * outputStrings.h
 *
 *  Created on: Mar 24, 2018
 *      Author: justin
 */

#ifndef OUTPUTSTRINGS_H
#define OUTPUTSTRINGS_H

#include <string>

std::string menu = "Please make a selection:\n"
        "A) Modify name\n"
        "B) Modify password\n"
        "C) Modify phone number\n"
        "D) Modify email\n"
        "E) Delete a user\n"
        "F) Add a new appointment\n"
        "G) Remove an appointment\n"
        "H) Update an existing appointment\n"
        "I) Display appointment at a time\n"
        "J) Display appointments in a time range\n"
        "K) Exit\n";
std::string usernameRequest = "";

//"A) Modify name, password, contact phone number, and email\n"
std::string modificationMenu = "Please select what you would like to modify:\n"
        "A) Name\n"
        "B) Password\n"
        "C) Phone Number\n"
        "D) Email\n\n";
std::string modification = "Please enter new value: \n";
std::string modSuccess = "Value successfully updated\n";

//"B) Delete a user\n"
std::string check = "Are you sure you wish to delete your account\nY/N\n";
std::string delSuccess = "Account deleted Good Bye\n";

//"C) Add a new appointment\n"
std::string appDate = "Enter the date of the new appointment:\n";
std::string appTime = "Enter the time of the new appointment:\n";
std::string appReason = "Enter the reason for the new appointment:\n";

//"D) Remove an appointment\n"
std::string removeDate = "Enter the date of the appointment to remove:\n";
std::string removeTime = "Enter the time of the appointment to remove:\n";

//"E) Update an existing appointment\n"
std::string oldDate = "Enter the OLD date of the appointment to update:\n";
std::string oldTime = "Enter the OLD time of the appointment to update:\n";
std::string newDate = "Enter the NEW date of the appointment to update:\n";
std::string newTime = "Enter the NEW time of the appointment to update:\n";
std::string newReason = "Enter the reason for the updated appointment\n";

//"G) Display a user’s the appointment for a specific time or a time range\n"
std::string timeChoice = "Please select an option: \n"
        "A) Display appointment at a specific time\n"
        "B) Display appointments in a specific time range\n\n";

//"H) Exit"
std::string appExit = "Good Bye\n\n";

//Login Menu
std::string login = "Please make a selecion\n"
        "A) Login\n"
"B) Create account\n";


#endif
