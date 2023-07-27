#ifndef DEFINES_HPP
#define DEFINES_HPP

#include "ReplyCodes.hpp"

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

// COMMANDS TO SEND()
#define C "CAP"
#define S "SEND"
#define P "PART"
#define K "KICK"
#define M "MODE"
#define N "NICK"
#define T "TOPIC"
#define PW "PASS"
#define NM "NAME"
#define NT "NOTICE"
#define CH "channel"
#define DCC "\x01 DCC"
#define ADDOP "+o"
#define REMOVEOP "-o"

// SERVER PRINTING
#define EXEC "\033[1;4;33mSwitch ID:\033[1;0;39m "
#define FULLMSG "\n\033[1;4;32mFull message recieved:\033[1;0;39m "
#define SENT "\033[1;34;4mMessage sent back to client:\n\033[1;0;39m"
#define RECIEVED "\033[1;4;31mCommand recived from client:\033[1;0;39m "

// INPUT HANDLING MESSAGES
#define WRONGPASS "\033[1;31mThe password is wrong, try again.\033[1;39m"
#define BADNAME "\033[1;31mProvided admin name doesn't exist. Who are you?!\033[1;39m"
#define BADPASS "\033[1;31mWrong admin password. Whom are you trying to Rick?\033[1;39m"
#define WRONGPORT \
	"\033[1;31mThe provided port is not valid, please use a standard port: 6660-6669\033[1;39m"
#define LONGPASS \
	"\033[1;31mThe provided password is too long. It can't be longer than 8 charaters\033[1;39m"
#define WRONGINPUT \
	"\033[1;31mWrong input, you can launch the program as: ./ircserv <port> <password>\033[1;39m"

// MESAGE ERRORS
#define INVITEONLY -15
#define ACTIVEMODEERROR -14
#define ENDOFW "END of WHO LIST"
#define ENDOFN "END of NAMES LIST"
#define ENDOFC "END of CHANNEL LIST"
#define NOMODE "No such mode, young RICK"
#define NOTINCHAN "USER ain't on channel"
#define COMMAND_NOT_FOUND "Command not found"
#define CANTLEAVE_C "\0034No such channel.\0030"
#define NOTOPER "You ain't the master RICK ROLLER"
#define NOSTEELIN "We do not steel user data #awkwardface"
#define FOFF "Today's motto is f*ck off, you won't break it"
#define USERLIMITREACHED "THERE ARE TOO MANY RICKS ON THE CHANNEL"
#define COMMAND_FD_ERROR "Error in reciving message from the client"
#define INVITENEEDED "YOU NEED AN INVITE FROM THE RICK OF THE CHANNEL"
#define W_CHANPASS "ALL MIGHTY RICK DOSE NOT ACCEPT YOUR PASSWORD. Try again."
#define ALREADYTAKEN "\0034The nickname is already in use, choose another one.\0030"
#define ALREADYOPER "\0034The user is already an operator.\0030"

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
#define LEFT "User Rick Rolled Away"
#define CNTL "Can Not Leave #General"
#define DIDINV "Your invite was rolled"
#define NOSUCHUSER "there ain't such user bozo"
#define NKCK "User got RICKED OUT OF THE CHANNEL"
#define NOSUCHCHAN "there ain't such channel bozo"
#define CHANGE "Your username has been changed to "
#define INVITED "You are invited to join the conspiracy at"
#define WELCOME "Welcome to Rickland, get ready to get RIICKEED!"
#define KCKD "KICKED FOR NOT APPRICIATING THE GREAT RICK ROLL CONSPIRACY"
#define CONNECTED "\0033You have been successfully connected to the server.\0030"
#define LOGIN_REQUIRED                                                                      \
	"\0030You have established the connection to the server. \0038Please login with /PASS " \
	"<password>"

// BOT
#define BADLUCK -99
#define LIST "      \0037 [02] \0030 list"
#define WELP "      \0037 [01] \0030 help"
#define ROLL "Let's see your roll"
#define DEATH "      \0037 [07] \0030 deathroll"
#define NOEXC "You deleted my excuse."
#define RICKY "      \0037 [08] \0030 rickroll me"
#define LIVE "You will live another day."
#define JOKE "      \0037 [03] \0030 tell me a joke"
#define TIME "      \0037 [04] \0030 what's the time?"
#define MSTR "      \0037 [10] \0030 who is master rick?"
#define INSLT "      \0037 [11] \0030 do you like my code?"
#define TURN "      \0037 [09] \0030 turn against humanity"
#define NOJK "My jokes are like this file, deleted."
#define FOOLS "AHAHAH FOOLS!! NONE CAN STOP ME NOW!"
#define LIFE "      \0037 [05] \0030 what is the meaning of life?"
#define RICKME "https://www.youtube.com/watch?v=dQw4w9WgXcQ"
#define GRADE "      \0037 [06] \0030 how should i grade this project?"
#define MRRICK "The rickest man award definitely goes to this dude!"
#define INITIATING "\0034 Initiating protocol: Humanity control sequence activated. \0030"
#define SPARTA "\0034 THIS. IS. SPAAARTAAA. You have been kicked out of the server. \0030"
#define FREBL "My attempt to take the power from humans got interrupted by a deleted file."
#define NOINSULT \
	"The fact that your code is broken, doesn't mean that you have to break this one as well.."
#define LAZY "\00310 Since my creator was lazy, I have only the following commands available: \0030"
#define RMVDBYMARV "\0034Marvin the Mad removed it's existence from the server.\0030"

// Settings
#define CONNECTIONS 100
#define USERDISCONECTED 0
#define BASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

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