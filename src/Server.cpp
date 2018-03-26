//============================================================================
// Name        : Server.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <string>
#include <iostream>
#include <algorithm>

#include "outputStrings.h"
#include "DAL.h"

#define MYPORT 336958
#define BACKLOG 10


void sigchld_handler(int s){

    while(wait(NULL) > 0);

}

int main(void) {

    int sockfd, new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;
    struct sigaction sa;
    int yes = 1;
    char *recvbuf;
    char *caddr;
    char* recieved;
    long numbytes;
    DAL user = DAL();


    /*
     *
     * Connection Sequence =============================================================================================
     *
     */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("SOCK_FD=%d\n", sockfd);
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        perror("setsockopt");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    if(bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD, &sa, NULL) == -1){
        perror("sigaction");
        exit(1);
    }

    //Main server function loop
    while(1){
        sin_size = sizeof(struct sockaddr_in);
        //added (socklen_t*) due to type mismatch
        if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t*) &sin_size)) == -1){
            perror("accept");
            exit(1);
        }
        printf("server: got connection from %s\n",  inet_ntoa(their_addr.sin_addr));
        //Creating threads for each client connection
        if(!fork()){
            close(sockfd);
            recvbuf=(char *) calloc(128, sizeof(char));

            std::string send_buf;
            std::string username;
            std::string password;

            /**
             *
             * Login Sequence===========================================================================================
             *
             */
            bool loggedIn = false;
            bool validated = false;
            while(!loggedIn){
                //send Login menu
                send(new_fd, login.c_str(), 127, 0);

                //Receive Selection
                numbytes = recv(new_fd, recvbuf, 127, 0);
                recvbuf[numbytes] = '\0';
                std::string loginChoice = recvbuf;
                std::cout << "[Client] " << loginChoice << "\n";
                /*
                 * If user decides to login
                 */
                if(!loginChoice.compare("A") || !loginChoice.compare("a")){
                    while(!validated) {

                        //recieve username
                        numbytes = recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        std::cout << "[Client]: " << recvbuf << "\n";
                        username = recvbuf;

                        //receive password
                        numbytes = recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        std::cout << "[Client]: " << recvbuf << "\n";
                        password = recvbuf;


                        //Login Successful
                        if(user.exists(username)){
                            user.populate(username);
                            if (!user.readPassword(username).compare(password)) {
                                send_buf = "Success";
                                send(new_fd, send_buf.c_str(), 127, 0);
                                std::cout << "[Server]: " << send_buf << "\n";
                                validated = true;
                                loggedIn = true;
                            }
                            else{ // Login Failure
                                send_buf = "Failure";
                                send(new_fd, send_buf.c_str(), 127, 0);
                                std::cout << "[Server]: " << send_buf << "\n";
                            }
                        }else{ // Login Failure
                            send_buf = "Failure";
                            send(new_fd, send_buf.c_str(), 127, 0);
                            std::cout << "[Server]: " << send_buf << "\n";
                        }


                    }

                /*
                 * If user wants to create a new account
                 */
                }
                else if(!loginChoice.compare("B") || !loginChoice.compare("b")){
                    //receive username and check if user already exists
                    bool exists = true;
                    while(exists){
                        numbytes = recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        std::cout << "[Client]: " << recvbuf << "\n";
                        username = recvbuf;

                        if(!user.exists(username)){
                            send_buf = "Success";
                            send(new_fd, send_buf.c_str(), 127, 0);
                            std::cout << "[Server]: " << send_buf << "\n";
                            exists = false;
                        }
                        else{
                            send_buf = "Failure";
                            send(new_fd, send_buf.c_str(), 127, 0);
                            std::cout << "[Server]: " << send_buf << "\n";
                        }
                    }

                    user.setUsername(username);

                    //receive password
                    numbytes = recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::cout << "[Client]: " << recvbuf << "\n";
                    password = recvbuf;
                    user.setPassword(password);

                    //receive name
                    numbytes = recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::cout << "[Client]: " << recvbuf << "\n";
                    std::string name = recvbuf;
                    user.setName(name);

                    //receive email
                    numbytes = recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::cout << "[Client]: " << recvbuf << "\n";
                    std::string email = recvbuf;
                    user.setEmail(email);

                    //receive phone number
                    numbytes = recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::cout << "[Client]: " << recvbuf << "\n";
                    std::string phone = recvbuf;
                    user.setPhone(phone);

                    user.write();

                    loggedIn = true;
                }
                else{
                    send_buf = "Invalid Choice";
                    std::cout << "[Server]: " << send_buf << "\n";
                }
            }
            /**
             *
             *
             *
             * Main menu operations begin here =========================================================================
             *
             *
             *
             */
            while(true){
                //print menu
                send_buf = menu;
                if(send(new_fd, send_buf.c_str(), 512, 0) == -1){
                    perror("send");
                    close(new_fd);
                    exit(1);
                }
                std::cout <<"[Server]: "<< send_buf << "\n";

                //Recieve client response
                numbytes=recv(new_fd, recvbuf, 127, 0);
                recvbuf[numbytes] = '\0';
                std::cout << "[Client]: " << recvbuf << "\n";

                std::string choice = recvbuf;
                /**
                 *
                 *
                 *
                 * Main menu choices are here
                 *
                 *
                 *
                 */
                if(!choice.compare("A") || !choice.compare("a")) {
                    // A) Modify name
                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    user.setName(recvbuf);
                    user.write();
                    send_buf = "Success";
                    send(new_fd, send_buf.c_str(), 127, 0);
                    std::cout << "Modified name: "<< recvbuf << std::endl;

                }
                else if(!choice.compare("B") || !choice.compare("b")){
                    //B) Modify password
                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    user.setPassword(recvbuf);
                    user.write();
                    send_buf = "Success";
                    send(new_fd, send_buf.c_str(), 127, 0);
                    std::cout << "Modified password: "<< recvbuf << std::endl;
                }
                else if(!choice.compare("C") || !choice.compare("c")){
                    //C) Modify phone number
                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    user.setPhone(recvbuf);
                    user.write();
                    send_buf = "Success";
                    send(new_fd, send_buf.c_str(), 127, 0);
                    std::cout << "Modified phone number: "<< recvbuf << std::endl;
                }
                else if(!choice.compare("D") || !choice.compare("d")){
                    //D) Modify email
                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    user.setEmail(recvbuf);
                    user.write();
                    send_buf = "Success";
                    send(new_fd, send_buf.c_str(), 127, 0);
                    std::cout << "Modified email: "<< recvbuf << std::endl;
                }
                else if(!choice.compare("E") || !choice.compare("e")){
                    //E) Delete a user
                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string check = recvbuf;
                    if(!check.compare("Y") || !check.compare("y")){
                        user.remove();
                        send_buf = "Success";
                        send(new_fd, send_buf.c_str(), 127, 0);
                        _Exit(0);
                    }else{
                        send_buf = "Cancelled";
                        send(new_fd, send_buf.c_str(), 127, 0);
                    }
                }
                else if(!choice.compare("F") || !choice.compare("f")){
                    //F) Add a new appointment
                    send_buf = user.sendAllAppointments(username);
                    send(new_fd, send_buf.c_str(), 512, 0);

                    std::string appDate;
                    std::string appTime;
                    int conflict = 1;
                    //Check for conflicting appointments
                    while(conflict) {
                        numbytes = recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        appDate = recvbuf;

                        numbytes = recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        appTime = recvbuf;

                        conflict = user.conflictCheck(appDate, appTime, username);
                        if (conflict) {
                            send_buf = "Failure";
                            std::cout << send_buf << std::endl;
                            send(new_fd, send_buf.c_str(), 127, 0);
                        } else {
                            send_buf = "Success";
                            std::cout << send_buf << std::endl;
                            send(new_fd, send_buf.c_str(), 127, 0);
                        }

                    }
                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string appReason = recvbuf;

                    user.createAppointment(username, appReason, appDate, appTime);

                    send_buf = "Success added appointment at: " + appDate + " at " + appTime + " for "+ appReason;
                    send(new_fd, send_buf.c_str(), 127, 0);

                    user.write();
                }
                else if(!choice.compare("G") || !choice.compare("g")){
                    //F) Remove an appointment
                    send_buf = user.sendAllAppointments(username);
                    send(new_fd, send_buf.c_str(), 512, 0);

                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string appDate = recvbuf;

                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string appTime = recvbuf;

                    user.removeAppointment(username, appDate, appTime);

                    send_buf = "Success removed appointment at: " + appDate + " " + appTime;
                    send(new_fd, send_buf.c_str(), 127, 0);

                    user.write();
                }
                else if(!choice.compare("H") || !choice.compare("h")){
                    //F) Update an appointment
                    send_buf = user.sendAllAppointments(username);
                    send(new_fd, send_buf.c_str(), 512, 0);

                    std::string appDate;
                    std::string appTime;
                    int conflict = 1;
                    //Check for conflicting appointments
                    while(conflict){
                        numbytes=recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        appDate = recvbuf;

                        numbytes=recv(new_fd, recvbuf, 127, 0);
                        recvbuf[numbytes] = '\0';
                        appTime = recvbuf;

                        conflict = user.conflictCheck(appDate, appTime, username);
                        if(conflict){
                            send_buf = "Failure";
                            std::cout << send_buf << std::endl;
                            send(new_fd, send_buf.c_str(), 127, 0);
                        }else{
                            send_buf = "Success";
                            std::cout << send_buf <<std::endl;
                            send(new_fd, send_buf.c_str(), 127, 0);
                        }

                    }

                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string appReason = recvbuf;

                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string oldAppTime = recvbuf;

                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string oldAppDate = recvbuf;

                    user.removeAppointment(username, oldAppDate, oldAppTime);

                    user.createAppointment(username, appReason, appDate, appTime);

                    send_buf = "Success updated appointment to: " + appDate + " at " + appTime + " for "+ appReason;
                    send(new_fd, send_buf.c_str(), 127, 0);

                    user.write();
                }
                else if(!choice.compare("I") || !choice.compare("i")){
                    //Send appointment at a time
                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string appDate = recvbuf;

                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string appTime = recvbuf;

                    send_buf = user.readAppointment(appDate, appTime);
                    send(new_fd, send_buf.c_str(), 127, 0);

                }
                else if(!choice.compare("J") || !choice.compare("j")){
                    //Send appointment in range
                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string startDate = recvbuf;

                    numbytes=recv(new_fd, recvbuf, 127, 0);
                    recvbuf[numbytes] = '\0';
                    std::string endDate = recvbuf;

                    send_buf = "Found these appointments in range: " + startDate + " to " + endDate +"\n"
                            + user.rangeReturnAppointments(startDate, endDate);
                    send(new_fd, send_buf.c_str(), 512, 0);

                    send_buf = "Success";
                    send(new_fd, send_buf.c_str(), 127, 0);

                }
                else if(!choice.compare("K") || !choice.compare("k")){
                    user.write();
                    close(new_fd);
                    _Exit(0);
                }
                else{}

            }
        }
        close(new_fd);
    }

    user.write();
    return 0;
}
