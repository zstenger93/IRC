#ifndef DEFINES_HPP
#define DEFINES_HPP

// DEFAULT HEADERS

#include <exception>
#include <iostream>
#include <algorithm>

// INPUT HANDLING MESSAGES

#define WRONGINPUT "Wrong input, you can launch the program as: ./ircserv <port> <password>"
#define WRONGPORT "The provided port is not valid, please use a standard port: 6660-6669"
#define WRONGPASS "The password is wrong, try again.\nOnly numbers and letters are accepted."
#define LONGPASS "The provided password is too long. It can't be longer than 8 charaters"

// USER ERRORS

#define USEREXIST "This username already exist, please provide another one"
#define NICKNAMEEXIST "This nickname already exist, please provide another one"

// ACTION MESSAGES

#define INVITED " has been invited to the server"
#define KICKED " has been kicked out of the server"
#define TOPIC_VIEW " channel topic is "
#define TOPIC_CHANGE " channel topic has been changed to "
#define NOCHANNEL "The channel you are trying to connect doesn't exist"
#define CANNOTMSG "Message cannot be sent because the targeted user doesn't exist"

// DEFAULT MESSAGES

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
#define YELLOW "\033[1;33m"

#endif