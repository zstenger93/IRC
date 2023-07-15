#ifndef DEFINES_HPP
#define DEFINES_HPP

#define CONNECTIONS 100
#define USERDISCONECTED 0
#define NOT_FOUND 1
#define BASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

// DEFAULT HEADERS

#include <arpa/inet.h>
#include <fcntl.h>
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

// INPUT HANDLING MESSAGES

#define WRONGINPUT "Wrong input, you can launch the program as: ./ircserv <port> <password>"
#define WRONGPORT "The provided port is not valid, please use a standard port: 6660-6669"
#define WRONGPASS "The password is wrong, try again."
#define LONGPASS "The provided password is too long. It can't be longer than 8 charaters"

// USER ERRORS

// #define USEREXIST "This username already exist, please provide another one" not used
#define NICKNAMEEXIST "\0034This nickname already exist, please provide another one\0030"
#define NICKEMPTYSTR "\0034The nickname cannot be an empty string.\0030"
#define ALREADYJOINED "\0034You have already joined to this server.\0030"

// MESAGE ERRORS
#define COMMAND_FD_ERROR "Error in reciving message from the client"
#define COMMAND_NOT_FOUND "Command not found"

// SOCKET ERRORS
#define F_FAILED_MESSAGE "Client failed to communicate with the server"
#define F_SET_SOCKET "Failed to create socket error"
#define F_SOCKET_BIND "Failed to Bind the Socket"
#define F_SOCKET_OPT "Failed to Set socket Options"
#define F_TO_LISTEN "Failed to listen for connections"
#define NOCONNECTION "Failed to accept the connection. Wrong password."
#define F_POOL "Failed to make a poll"

// ACTION MESSAGES

#define NICKCHANGED "\0033Your nickname has been changed.\0030"
#define CONNECTED "\0033You have been successfully connected to the server.\0030"
#define LOGIN_REQUIRED                                                                      \
	"\0030You have established the connection to the server. \0038Please login with /PASS " \
	"<password>"
#define INVITED " has been invited to the server"
#define KICKED " has been kicked out of the server"
#define TOPIC_VIEW " channel topic is "
#define TOPIC_CHANGE " channel topic has been changed to "
#define NOCHANNEL "The channel you are trying to connect doesn't exist"
#define CANNOTMSG "Message cannot be sent because the targeted user doesn't exist"
#define INVALID_C_NAME "\0034Invalid channel name.\0030"
#define CANTLEAVE_C "\0034No such channel.\0030"
#define LEFTCHANNEL "has left the channel."
#define NOSUCHUSER "there ain't such user bozo"
#define NOSUCHCHAN "there ain't such channel bozo"
// DEFAULT MESSAGES

#define RICK "RickRoll.de"
#define CREATEDCHANNEL " channel has been created."
#define JOINEDCHANNEL " You have joined to the channel."
#define MODE_I "This channel mode is invitation only"
#define MODE_I_SET "This channel has been set to invitation only"
#define MODE_I_REMOVE "This channel is not invitation only anymore"
#define MODE_T_SET "The channel topic has been set"
#define MODE_T_REMOVE "The channel topic has been removed"
#define MODE_K_SET "A password has been set to this channel"
#define MODE_K_REMOVE "Password protection has been removed from the channel"
#define MODE_O_GIVE " has given operator privilage to "
#define MODE_O_TAKE " has removed operator privilages by "
#define MODE_L "The user limit on this channel is "
#define MODE_L_SET "The user limit on this channel has been set to "
#define MODE_L_REMOVE "The user limit on this channel has been removed"

// BONUS

#define MISSING_FILE "The file you are trying to send doesn't exist"
#define HELP "help -c: list channels\nhelp -m display channel mode\nhelp -g: tell me a joke\n"

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

// MOTD

#define MOTD "When I was young I used to think that USB is a country next to USA"

#endif