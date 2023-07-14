#include "../../includes/Commands.hpp"

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

bool Server::checkIfCanBeExecuted(std::string channelName, int senderFd) {
	std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		send_message_to_server(senderFd, 4, RICK, ERR_NOSUCHCHANNEL,
							   users.find(senderFd)->second.getNickName().c_str(), COL, NOSUCHCHAN);
		return false;
	}
	if (users.find(senderFd)->second.isInChannel(channelName) == false) {
		send_message_to_server(senderFd, 3, RICK, ERR_USERNOTINCHANNEL,
							   users.find(senderFd)->second.getNickName().c_str(),
							   channelName.c_str(), COL, "USER ain't on channel");
		return false;
	}
	return true;
}

void Server::executeCommmandsToChannel(std::string channelName, User& user, int mode,
									   std::string message) {
	if (checkIfCanBeExecuted(channelName, user.getUserFd()) == false) return;
	switch (mode) {
		case 0:
			send_message_to_server(user.getUserFd(), 4, user.getNickName(), PRIVMSG,
								   channelName.c_str(), COL, message.c_str());
			break;
		case 1:
			break;
		default:
			std::cerr << "Error in the switch" << std::endl;
			break;
	}
}

void Server::loopTroughtTheUsersInChan(std::string channelName, int senderFd, int mode,
									   std::string message, User& user) {
	if (checkIfCanBeExecuted(channelName, senderFd) == false) return;
	std::map<int, User>::iterator userIt = users.begin();
	for (; userIt != users.end(); userIt++) {
		if (userIt->second.isInChannel(channelName) && userIt->second.getUserFd() != senderFd) {
			switch (mode) {
				case 0:
					send_message_to_server(userIt->first, 4, userIt->second.getNickName(), PRIVMSG,
										   channelName.c_str(), COL, message.c_str());
					break;
				case 1:
					// SEND MESSAGE ABOUT JOINING THE CHANNEL
					send_message_to_server(userIt->first, 4, user.getUserName(), "JOIN",
										   channelName.c_str(), COL, channelName.c_str());
					break;
				case 2:
					send_message_to_server(senderFd, 6, RICK, RPL_NAMREPLY,
										   user.getNickName().c_str(), "=", channelName.c_str(),
										   COL, userIt->second.getNickName().c_str());
				default:
					std::cerr << "Error in the switch" << std::endl;
					break;
			}
		}
	}
}

void Server::handleJoin(std::string message, User& user, std::string name) {
	if (name.length() == 0) {
		send_message_to_server(user.getUserFd(), 3, RICK, ERR_NEEDMOREPARAMS, COMMAND, JOIN, COL);
		return;
	}
	std::map<std::string, Channel>::iterator channelIt = channels.find(name);
	if (channelIt == channels.end()) {
		createChannel(user, name);
	}
	if (!isJoinedWithActiveMode(channelIt->second, user, message)) {
		user.joinChannel(user, name);  // ADDS USER TO THE CHANNEL
		loopTroughtTheUsersInChan(
			name, user.getUserFd(), 1, message,
			user);	// LOOPS TROUGHT USERS AND SEND INFORMATION THAT USER JOINED
		channelTopic(message, channelIt->first, user.getUserFd());	// SENDS TOPIC TO THE USER
		// send_message_to_server(user.getUserFd(), 3, std::string prefix, ...); // START OF THE
		// LIST
		loopTroughtTheUsersInChan(
			name, user.getUserFd(), 2, message,
			user);	// LOOPS TROUGHT THE USERS IN CHANNEL AND PRINTS OUT A LIST ELEMENT
		send_message_to_server(user.getUserFd(), 3, RICK, RPL_ENDOFNAMES,
							   user.getNickName().c_str(), name.c_str(), COL,
							   "END of NAMES LIST");  // END OF THE LIST
	}
}

bool Server::isJoinedWithActiveMode(Channel& channel, User& user, std::string message) {
	int userCount = channel.getUserCount();
	int userLimit = channel.getUserLimit();
	if (userCount < userLimit && channel.checkMode("l")) {
		user.joinChannel(user, channel.getChannelName());
		channel.changeUserCount(userCount++);
		return true;
	} else if (userCount == userLimit && channel.checkMode("l")) {
		// CANNOT JOIN THE USER LIMIT IS: USERLIMIT
	}
	if (channel.checkMode("k")) {
		std::string providedPass = extractArgument(2, message, 3);
		if (channel.isPasswordCorrect(providedPass))
			user.joinChannel(user, channel.getChannelName());
		else {
			// WRONG PASSWORD FOR THE CHANNEL
		}
		return true;
	}
	if (channel.checkMode("i")) {
		std::cout << "ONLY INVITE CHANNEL, PROVE YOUR WORTHYNESS TO ODYN" << std::endl;
		// CANNOT JOIN MESSAGE
		return true;
	}
	return false;
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
		//		loopTroughtTheUsersInChan(channelName, userFd, 0, message);
		executeCommmandsToChannel(channelName, userIt->second, 0, message);
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

// HELPER FUNCTION, MOVE IT IDK WHERE
bool User::isInChannel(std::string channelName) {
	return (channels.find(channelName) != channels.end());
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
void User::kickUser(std::map<int, User>& users, std::string kickUserName, std::string channelName,
					int senderFd) {	 // users
	std::map<std::string, bool>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		send_message_to_server(senderFd, 3, RICK, ERR_USERNOTINCHANNEL,
							   users.find(senderFd)->second.getNickName().c_str(),
							   channelName.c_str(), COL, "USER ain't on channel");
		// RETURN NO SUCH CHANNEL ERROR
	}
	if (channelIt->second == false) {
		send_message_to_server(senderFd, 3, RICK, ERR_CHANOPRIVSNEEDED,
							   users.find(senderFd)->second.getNickName().c_str(),
							   channelName.c_str(), COL, "USER ain't an opperator");
	}

	std::map<int, User>::iterator userIt;
	for (userIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->second.getUserName().compare(kickUserName) == 0) break;
	}
	if (userIt == users.end()) {
		send_message_to_server(senderFd, 3, RICK, ERR_USERNOTINCHANNEL,
							   users.find(senderFd)->second.getNickName().c_str(),
							   channelName.c_str(), COL, "USER ain't on channel");
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

void User::inviteUser(std::map<int, User>& users, std::string addUserName, std::string channelName,
					  int senderFd) {  // users
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

void Server::shutdown(std::string message) {
	std::string adminName = extractArgument(1, message, 3);
	std::string adminPassword = extractArgument(2, message, 3);
	if (adminName == operator_name && adminPassword == operator_password) {
		reset = false;
		serverState = false;  // not sure if this is needed
	} else if (adminPassword != operator_password)
		std::cout << "Wrong admin password." << std::endl;
	else
		std::cout << "Provided admin name doesn't exist." << std::endl;	 // ADMIN DOESN'T EXIST
	// this is only server admin function
	// shut down the server
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
		send_message_to_server(userFd, 3, RICK, ERR_NOSUCHCHANNEL, COL, NOSUCHCHAN);
	}
	bool add;
	if (Parser::getWordCount(message) == 2)	 // channelName
	{
		// get every mode and send to user
		const std::map<std::string, bool> modes = channelIt->second.getChannelModes();
		for (std::map<std::string, bool>::const_iterator modeIt = modes.begin();
			 modeIt != modes.end(); modeIt++) {
			if (modeIt->second == true) mode += modeIt->first;
		}
		send_message_to_server(userFd, 4, RICK, RPL_CHANNELMODEIS, channelName.c_str(), COL,
							   mode.c_str());
	} else if (Parser::getWordCount(message) == 3 &&
			   userIt->second.isOperatorInChannel(channelName)) {
		channelName = extractArgument(1, message, 3);
		mode = extractArgument(2, message, 3);
		if (mode[0] == '+')
			add = true;
		else if (mode[0] == '-')
			add = false;
		else {
			send_message_to_server(userIt->first, 3, RICK, ERR_NOSUCHSERVICE, COL,
								   "No such mode bozo");
		}
		if (add) {
			mode = mode.substr(1);
			channelIt->second.addMode(mode, true);
			for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
				 usersIt++) {
				if (usersIt->second.isInChannel(channelName) == true)
					send_message_to_server(userIt->first, 4, RICK, RPL_CHANNELMODEIS,
										   channelName.c_str(), COL, "The mode has been changed");
			}
		} else {
			channelIt->second.addMode(mode, false);
			for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
				 usersIt++) {
				if (usersIt->second.isInChannel(channelName) == true)
					send_message_to_server(userIt->first, 4, RICK, RPL_CHANNELMODEIS, COL,
										   channelName.c_str(), "The mode has been changed");
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
void Server::channelTopic(std::string message, std::string channelName, int userFd) {
	std::map<int, User>::iterator userIt = users.find(userFd);
	std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		send_message_to_server(userFd, 3, RICK, ERR_NOSUCHCHANNEL, COL, NOSUCHCHAN);
	}
	if (Parser::getWordCount(message) == 2) {
		send_message_to_server(userFd, 4, RICK, RPL_TOPIC, channelName.c_str(), COL,
							   channelIt->second.getChannelTopic().c_str());
	}
	if (Parser::getWordCount(message) > 2 && userIt->second.isOperatorInChannel(channelName)) {
		int newTopicStartPos = 5 + 1 + channelName.length() + 1 + 1;
		channelIt->second.setChannelTopic(message.substr(newTopicStartPos));
		for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
			 usersIt++) {
			if (usersIt->second.isInChannel(channelName) == true)
				send_message_to_server(userIt->first, 3, RICK,
									   channelIt->second.getChannelName().c_str(), COL,
									   channelIt->second.getChannelTopic().c_str());
		}
	}
}

// tf it is doing:
// command sent from the client:
// code:
// must have:
// optional:
// error:
void User::ping(std::string message, int userFd) {
	send_message_to_server(userFd, 4, RICK, "PONG", RICK, COL,
						   message.c_str());  // NEED TO BE written what we need to
											  // return, might need to fix the preifx need to parse
											  // the message, right now prints all of it
}

void Server::who(int userFd, std::string message) {
	if (Parser::getWordCount(message) > 2) {
		send_message_to_server(userFd, 1, RICK, ERR_TOOMANYTARGETS);
	}
	std::string userNames = "";
	std::map<int, User>::iterator userIt = users.begin();
	for (; userIt != users.end(); userIt++) {
		if (userIt->second.getUserFd() != userFd) {
			userNames.append(userIt->second.getUserName() + " ");
		}
	}
	send_message_to_server(userFd, 3, RICK, "WHO", COL, userNames.c_str());
}

void Server::whois(int userFd, std::string message) {
	std::string requestedUserName = extractArgument(1, message, 2);
	std::map<int, User>::iterator userIt = users.begin();
	for (; userIt != users.end(); userIt++) {
		if (userIt->second.getNickName().compare(requestedUserName) == 0) {
			send_message_to_server(userFd, 4, RICK, "WHOIS", userIt->second.getNickName().c_str(),
								   COL, userIt->second.getUserName().c_str());
			break;
		}
	}
	if (userIt == users.end()) {
		send_message_to_server(userFd, 3, RICK, ERR_NOSUCHNICK, COL, NOSUCHUSER);
	}
}

// // tf it is doing:
// // command sent from the client:
// // code:
// // must have:
// // optional:
// // error:
void Server::motd(int userFd) {
	std::map<int, User>::iterator userIt = users.find(userFd);
	std::ifstream file("conf/motd.txt");
	std::string line;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			send_message_to_server(userFd, 3, RICK, PRIVMSG, COL, line.c_str());
		}
		file.close();
	} else {
		// ERROR
	}
}

// tf it is doing:
// command sent from the client: WHO
// code: 352
// must have: <code> <Nickname of requesting user> : <chanal> <username> <hostname> <nick of
// requested user> <name> optional: error: 402 no such server 403 no such chanal 421 unknown command
// 451 not registered
// error 431 nonick given
// error :server.example.com 402 Alice :No such server

// // tf it is doing: PONG
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

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/