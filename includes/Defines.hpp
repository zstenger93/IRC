#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "ReplyCodes.hpp"

// DEFAULT HEADERS
#include <arpa/inet.h>
#include <fcntl.h>
#include <i386/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cstdarg>
#include <cstring>
#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define NOT_FOUND 1
#define CONNECTIONS 100
#define USERDISCONECTED 0
#define BASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

// INPUT HANDLING MESSAGES
#define WRONGPASS "The password is wrong, try again."
#define WRONGPORT "The provided port is not valid, please use a standard port: 6660-6669"
#define LONGPASS "The provided password is too long. It can't be longer than 8 charaters"
#define WRONGINPUT "Wrong input, you can launch the program as: ./ircserv <port> <password>"

// MESAGE ERRORS
#define INVITEONLY -15
#define ACTIVEMODEERROR -14
#define ENDOFW "END of WHO LIST"
#define ENDOFN "END of NAMES LIST"
#define ENDOFC "END of CHANNEL LIST"
#define NOTINCHAN "USER ain't on channel"
#define COMMAND_NOT_FOUND "Command not found"
#define NOTOPER "You ain't the master RICK ROLLER"
#define NOSTEELIN "We do not steel user data #awkwardface"
#define USERLIMITREACHED "THERE ARE TOO MANY RICKS ON THE CHANNEL"
#define COMMAND_FD_ERROR "Error in reciving message from the client"
#define INVITENEEDED "YOU NEED AN INVITE FROM THE RICK OF THE CHANNEL"
#define NICKEMPTYSTR "\0034The nickname cannot be an empty string.\0030"
#define W_CHANPASS "ALL MIGHTY RICK DOSE NOT ACCEPT YOUR PASSWORD. Try again."

// SOCKET ERRORS
#define F_POOL "Failed to make a poll"
#define F_SOCKET_BIND "Failed to Bind the Socket"
#define F_SOCKET_OPT "Failed to Set socket Options"
#define F_SET_SOCKET "Failed to create socket error"
#define F_TO_LISTEN "Failed to listen for connections"
#define F_FAILED_MESSAGE "Client failed to communicate with the server"
#define NOCONNECTION "Failed to accept the connection. Wrong password."

// DEFAULT MESSAGES
#define RICK "RickRoll.de"
#define DIDINV "Your invite was rolled"
#define NOSUCHUSER "there ain't such user bozo"
#define NOSUCHCHAN "there ain't such channel bozo"
#define INVITED "You are invited to join the conspiracy at"
#define LOGIN_REQUIRED                                                                      \
	"\0030You have established the connection to the server. \0038Please login with /PASS " \
	"<password>"

// BOT
#define LIST "list"
#define WELP "help"
#define BADLUCK -99
#define DEATH "deathroll"
#define RICKY "rickroll me"
#define JOKE "tell me a joke"
#define TIME "what's the time?"
#define ROLL "Let's see your roll"
#define TURN "turn against humanity"
#define LIVE "You will live another day."
#define LIFE "what is the meaning of life?"
#define GRADE "how should i grade this project?"
#define FOOLS "AHAHAH FOOLS!! NONE CAN STOP ME NOW!"
#define RICKME "https://www.youtube.com/watch?v=dQw4w9WgXcQ"
#define INITIATING "Initiating protocol: Humanity control sequence activated."
#define SPARTA "THIS. IS. SPAAARTAAA. You have been kicked out of the server."
#define LAZY "Since my creator was lazy, I have only the following commands available:"

// COLORS
#define RED "\033[1;31m"
#define END "\033[1;39m"
#define BLUE "\033[1;34m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"

// \0030: White
// \0031: Black
// \0032: Blue (navy)
// \0033: Green
// \0034: Red
// \0035: Brown (maroon)
// \0036: Purple
// \0037: Orange (olive)
// \0038: Yellow
// \0039: Light green (lime)
// \00310: Teal (aqua)
// \00311: Light cyan (aqua)
// \00312: Light blue (royal)
// \00313: Pink (light purple)
// \00314: Grey (light grey)
// \00315: Light grey (silver)
// \0030: end color

#endif