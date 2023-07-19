#include "../../includes/Channel.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Parser.hpp"
#include "../../includes/ReplyCodes.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void User::inviteUser(std::map<int, User>& users, std::string addUserName, std::string channelName,
					  int senderFd) {
	std::map<std::string, bool>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		return send_message_to_server(senderFd, 2, RICK, ERR_NOSUCHCHANNEL,
									  users.find(senderFd)->second.getNickName().c_str());
	}
	if (channelIt->second == false) {
		return send_message_to_server(senderFd, 4, RICK, PRIVMSG, channelName.c_str(), COL,
									  NOTOPER);
	}
	std::map<int, User>::iterator userIt;
	for (userIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->second.getUserName().compare(addUserName) == 0) break;
	}
	if (userIt == users.end()) {
		return send_message_to_server(senderFd, 2, RICK, ERR_NOSUCHNICK, addUserName.c_str());
	}
	if (userIt->second.isInChannel(channelName) == true) {
		return send_message_to_server(senderFd, 2, RICK, ERR_USERONCHANNEL, addUserName.c_str());
	}
	send_message_to_server(senderFd, 4, RICK, PRIVMSG, channelName.c_str(), COL, DIDINV);
	send_message_to_server(userIt->first, 5, RICK, "NOTICE", userIt->second.getNickName().c_str(),
						   COL, INVITED, channelName.c_str());
	userIt->second.joinChannel(userIt->second, channelName, 0);
	std::map<int, User>::iterator usersIt;
	for (usersIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->first != usersIt->first) {
			send_message_to_server(usersIt->first, 4, userIt->second.getNickName(), "JOIN",
								   channelName.c_str(), COL, channelName.c_str());
			send_message_to_server(userIt->first, 6, RICK, RPL_NAMREPLY,
								   userIt->second.getNickName().c_str(), "=", channelName.c_str(),
								   COL, usersIt->second.getNickName().c_str());
		}
		send_message_to_server(userIt->second.getUserFd(), 6, RICK, RPL_NAMREPLY,
							   userIt->second.getNickName().c_str(), "=", channelName.c_str(), COL,
							   "Marvin");
		send_message_to_server(userIt->second.getUserFd(), 3, RICK, RPL_ENDOFNAMES,
							   userIt->second.getNickName().c_str(), channelName.c_str(), COL,
							   ENDOFN);
	}
}

void User::kickUser(std::map<int, User>& users, std::string kickUserName, std::string channelName,
					int senderFd) {
	std::map<std::string, bool>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		return send_message_to_server(senderFd, 4, RICK, ERR_NOSUCHCHANNEL,
									  users.find(senderFd)->second.getNickName().c_str(), COL,
									  NOSUCHCHAN);
	}
	if (channelIt->second == false) {
		return send_message_to_server(senderFd, 4, RICK, PRIVMSG, channelName.c_str(), COL,
									  NOTOPER);
	}
	std::map<int, User>::iterator userIt;
	for (userIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->second.getUserName().compare(kickUserName) == 0) break;
	}
	if (userIt == users.end()) {
		send_message_to_server(senderFd, 3, RICK, ERR_NOSUCHNICK, COL, NOSUCHUSER);
	}
	if (userIt->second.isInChannel(channelName) == false) {
		send_message_to_server(senderFd, 5, RICK, ERR_USERNOTINCHANNEL,
							   users.find(senderFd)->second.getNickName().c_str(),
							   channelName.c_str(), COL, NOTINCHAN);
	}
	userIt->second.leaveChannel(users, userIt->second, channelName, 1);
}

void Server::shutdown(std::string message) {
	std::string adminName = extractArgument(1, message, 3);
	std::string adminPassword = extractArgument(2, message, 3);
	if (adminName == operator_name && adminPassword == operator_password) {
		serverState = false;
		reset = false;
	}
	else if (adminPassword != operator_password)
		std::cout << BADPASS << std::endl;
	else
		std::cout << BADNAME << std::endl;
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/