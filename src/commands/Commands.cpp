#include "../../includes/Commands.hpp"

#include <string>

#include "../../includes/Channel.hpp"
#include "../../includes/Parser.hpp"
#include "../../includes/ReplyCodes.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

// tf it is doing: JOINING creating channals, storing them localy
// command sent from client JOIN
// code: 999 or no specific code
// must have: <channel>
// optional: key
// error: 403 no such chanal
// error: 405 err too many chanells
// error: 473 invite only channel
// error 474 banned from channel
// error 475 bad channel password

void Server::handleJoin(User& user, std::string name) {
	if (name.length() == 0) {
		send_message_to_server(user.getUserFd(), 3, RICK, ERR_NEEDMOREPARAMS, COMMAND, JOIN, COL);
		return;
	}
	std::map<std::string, Channel>::iterator it = channels.find(name);
	if (it == channels.end()) {
		createChannel(user, name);
		// send_message_to_server(user.getUserFd(), 3, user.getNickName(), JOIN, COL, name.c_str());
		user.joinChannel(user, name);
		return;
	}
	// if channel is invite only
	user.joinChannel(user, name);
}

// tf it is doing: sending a bloody message
// command sent from the client: PRIVMSG
// code: no code
// must have: target,  <channel> or <nick>
// optional:
// error: 401 no such nick
// error: 404 client is not a members of the target channel
// error: 412 client did not provide any text to send
void Server::sendMessage(std::string message, std::map<int, User>& users, int userFd) {
	std::map<int, User>::iterator userIt = users.find(userFd);
	if (extractArgument(1, message, -1)[0] != '#') {
		std::string messageTo = extractArgument(1, message, -1);
		if (messageTo.empty() == true)
			send_message_to_server(userIt->first, 3, RICK, ERR_NEEDMOREPARAMS, COL);
		std::map<int, User>::iterator receiverIt = users.begin();
		for (; receiverIt != users.end(); receiverIt++) {
			if (receiverIt->second.getUserName().compare(messageTo) == 0) {
				send_message_to_server(receiverIt->first, 5, userIt->second.getNickName().c_str(),
									   PRIVMSG, receiverIt->second.getNickName().c_str(), COL,
									   message.c_str());
				return;
			}
		}
		if (receiverIt == users.end()) {
			send_message_to_server(userIt->first, 3, RICK, ERR_NOSUCHNICK, COL, NOSUCHUSER);
			return;
		}
	} else {
		std::string channelName = extractArgument(1, message, -1);
		std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);
		if (channelIt != channels.end()) {
			for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
				 usersIt++) {
				if (usersIt->second.isInChannel(channelName) == true) {
					send_message_to_server(usersIt->first, 4, userIt->second.getNickName().c_str(),
										   PRIVMSG, usersIt->second.getNickName().c_str(), COL,
										   message.c_str());
				}
			}
		} else {
			send_message_to_server(userIt->first, 4, RICK, ERR_NOSUCHCHANNEL, COL, NOSUCHCHAN);
			return;
		}
	}
}

// tf it is doing: leaves the channal
// command sent from the client: PART
// code: not specified can use 999
// must have: channel
// optional:
// error:  403 channel dose not exist
// error 442 client is not a mmbers of specific client
// error 461 need more params
// error 421 the PART command is not recognised as a part of the server
void User::leaveChannel(std::map<int, User>& users, User& user, std::string channelName) {
	std::map<std::string, bool>::iterator channel = channels.find(channelName);
	if (channel == channels.end()) {
		send_message_to_server(user.getUserFd(), 4, RICK, "403 :", user.getNickName().c_str(),
							   channelName.c_str(), CANTLEAVE_C);
		return;
	}
	channels.erase(channel);
	// send to user
	send_message_to_server(user.getUserFd(), 4, user.getNickName(), RPL_ENDOFNAMES,
						   channelName.c_str(), COL, LEFTCHANNEL);
	// send to everyone else on the channel
	for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++) {
		if (usersIt->second.isInChannel(channelName) == true)
			;  // send() everyone on the channel
	}
}

bool User::isInChannel(std::string channelName) {
	return (channels.find(channelName) == channels.end());
}

// tf it is doing: Kicks user from the channal
// command sent from the client: KICK
// code: KICK
// must have: <user that kicks> KICK <channal> <user that is kicked>
// optional:
// error: 461 need more params
// error 403 nosuchchannel
// error 442 notonchannel
// error 476 badchanmask
void User::kickUser(std::map<int, User>& users, std::string kickUserName,
					std::string channelName) {	// users
	std::map<std::string, bool>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		// RETURN NO SUCH CHANNEL ERROR
	}
	if (channelIt->second == false) {
		// THE USER IS NOT OPERATOR ERROR
	}

	std::map<int, User>::iterator userIt;
	for (userIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->second.getUserName().compare(kickUserName) == 0) break;
	}
	if (userIt == users.end()) {
		// NO SUCH USER ERROR
	}

	if (userIt->second.isInChannel(channelName) == false) {
		// KICKUSER IS NOT IN THE CHANNEL
	}

	userIt->second.leaveChannel(users, userIt->second, channelName);
	// send to user that he has been kicked from the channel
	for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++) {
		if (usersIt->second.isInChannel(channelName) == true)
			;  // SEND TO CHANNEL USER KICKED KICKEDUSER FROM THE CHANNEL
	}
}

// tf it is doing: invite to the channal
// command sent from the client: INVITE
// code: INVITE
// must have: :<user who invited> INVITE <invited user> :<channel>
// optional:
// error::
// :<server> <error code> <client nickname> <channel> :<error message>
// error: 461 need more params
// error: 401 nickname dose not exist
// error: 442 user already on channel
// error: 482 dose not have invite privilages

void User::inviteUser(std::map<int, User>& users, std::string addUserName,
					  std::string channelName) {  // users
	std::map<std::string, bool>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		// RETURN NO SUCH CHANNEL ERROR
	}
	if (channelIt->second == false) {
		// THE USER IS NOT OPERATOR ERROR
	}

	std::map<int, User>::iterator userIt;
	for (userIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->second.getUserName().compare(addUserName) == 0) break;
	}
	if (userIt == users.end()) {
		// NO SUCH USER ERROR
	}

	if (userIt->second.isInChannel(channelName) == true) {
		// USER ALREADY IN THE CHANNEL
	}

	userIt->second.channels.insert(std::make_pair(channelName, false));
	// USER HAS BEEN INVITED AND ADDED TO THE CHANNEL
}

void Server::shutdown() {
	// this is only server admin function
	// shut down the server
	serverState = false;  // not sure if this is needed
	reset = false;
}

// tf it is doing:
// command sent from the client: change NICK of the user
// code: 001
// must have: <code> <nick>
// optional:
// error: 431 nickname not given
// error: 432 nickname is invalid
// error: 433 nickname already taken
void User::setNick(std::map<int, User>::iterator it, std::string newNickname) {
	// needs channel name after username
	if (newNickname.length() != 0) {
		// check if someone is already using this nick ??
		nickName = newNickname;
		// nickName = "\0037" + newNickname + "\0030";
		send_message_to_server(it->first, 2, RICK, nickName.c_str(), NICKCHANGED);
		// do we send msg to other users that one changed it's nickname?
	} else {
		send_message_to_server(it->first, 2, RICK, nickName.c_str(), NICKEMPTYSTR);
	}
}

// tf it is doing: displays all aviable channels
// command sent from the client:
// code: 321 322 323 321 indicates the start of the list 322 is middle elements in the list and 323
// indicates end of the list (CONUFSION) must have: <code> <nick> : <channel> optional: <user count>
// <topic> error: 402 NOSERVER so no list error: 481 no priviliages error: 416 too many matches for
// the list command??? error 437 list command not aviable?? error 451 user nor registered
void Server::listChannels(std::string userName) {
	// list the available channels? or what you have joined to?
	std::map<int, User>::iterator userIt = users.begin();
	for (; userIt != users.end(); userIt++) {
		if (userIt->second.getUserName().compare(userName) == 0) break;
	}
	if (userIt == users.end()) {
		// NO SUCH USER EXCEPTION. probably not needed
	}

	for (std::map<std::string, Channel>::iterator channelIt = channels.begin();
		 channelIt != channels.end(); channelIt++) {
		/// SEND to user channelIt->first;
	}
}

// tf it is doing:
// command sent from the client:
// code:
// must have:
// optional:
// error:
// NEED TO DEBUG THIS, IT GET'S SEGFAULT ON JOIN CHANNEL
void Server::mode(std::string message, int userFd) {  // channelName
	// show the mode of the channel. i guess it should take the channel name as arg
	std::string channelName = extractArgument(1, message, -1);
	std::string mode = "";
	std::map<int, User>::iterator userIt = users.find(userFd);
	std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		// NO SUCH CHANNEL ERROR
	}
	bool add;
	if (Parser::getWordCount(message) == 3)	 // channelName
	{
		// get every mode and send to user
		const std::map<std::string, bool> modes = channelIt->second.getChannelModes();
		for (std::map<std::string, bool>::const_iterator modeIt = modes.begin();
			 modeIt != modes.end(); modeIt++) {
			if (modeIt->second == true) mode += modeIt->first;
		}
		// send() send and write the mode to the user only
	} else if (Parser::getWordCount(message) == 4 &&
			   userIt->second.isOperatorInChannel(channelName)) {
		channelName = extractArgument(1, message, 3);
		mode = extractArgument(2, message, 3);
		if (mode[0] == '+')
			add = true;
		else if (mode[0] == '-')
			add = false;
		else {
			// ERROR
		}
		if (add) {
			mode = mode.substr(1);
			channelIt->second.addMode(mode, true);
			for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
				 usersIt++) {
				if (usersIt->second.isInChannel(channelName) == true)
					;  // msg ? everyone on the channel
			}
		} else {
			channelIt->second.addMode(mode, false);
			for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
				 usersIt++) {
				if (usersIt->second.isInChannel(channelName) == true)
					;  // msg ? everyone on the channel
			}
		}
	}
}

// tf it is doing:
// command sent from the client:
// code:
// must have:
// optional:
// error:
void User::oper() {
	// change the mode of the channel
}

// tf it is doing:
// command sent from the client:
// code:
// must have:
// optional:
// error:
void Server::channelTopic(std::string message, std::string channelName, int userFd) {
	std::map<int, User>::iterator userIt = users.find(userFd);
	std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		// no such channel
	}
	if (Parser::getWordCount(message) == 2) {
		// send this to the user only
		// channelIt->second.getChannelTopic()
	}
	if (Parser::getWordCount(message) > 2 && userIt->second.isOperatorInChannel(channelName)) {
		int newTopicStartPos = 5 + 1 + channelName.length() + 1 + 1;
		channelIt->second.setChannelTopic(message.substr(newTopicStartPos));
		for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
			 usersIt++) {
			if (usersIt->second.isInChannel(channelName) == true)
				;  // msg ? everyone on the channel
		}
	}
}

// tf it is doing:
// command sent from the client:
// code:
// must have:
// optional:
// error:
void User::ping() {
	// change the topic of the channel
}

void User::who() {}

// // tf it is doing:
// // command sent from the client:
// // code:
// // must have:
// // optional:
// // error:
void User::motd(User& user) {
	// std::ifstream file("conf/motd.txt");
	// std::string line;
	// send_message_to_server(user.getUserFd(), 1, MOTD);
	// while (std::getline(file, line)) {
	// 	std::cout << line << std::endl;
	// 	send_message_to_server(user.getUserFd(), 1, line.c_str());
	// }
	// file.close();
}

// tf it is doing:
// command sent from the client: WHO
// code: 352
// must have: <code> <Nickname of requesting user> : <chanal> <username> <hostname> <nick of
// requested user> <name> optional: error: 402 no such server 403 no such chanal 421 unknown command
// 451 not registered
// error 431 nonick given
// error :server.example.com 402 Alice :No such server

// MISSING COMMANDS NOTICE, OPER

// void User::WHO
// {

// }

// // tf it is doing: PONG
// // command sent from the client: PING
// // code: 999 or PING
// // must have: PING: PONG
// // optional:
// // error:
// void User:: Ping
// {

// }

// // tf it is doing: sends s
// // command sent from the client: PING
// // code: 999 or PING
// // must have: PING: PONG
// // optional:
// // error:
// void User:: Ping
// {

// }

// // tf it is doing: OPER
// // command sent from the client: OPER <username> <password>
// // code: 381
// // must have: <code> <nick>
// // optional:
// // error 461 not enought parameters
// error 464 password incorrect
// void User:: Ping
// {

// }

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/