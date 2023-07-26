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
		if (userIt->second.getNickName().compare(addUserName) == 0) break;
	}
	if (userIt == users.end()) {
		return send_message_to_server(senderFd, 2, RICK, ERR_NOSUCHNICK, addUserName.c_str());
	}
	if (userIt->second.isInChannel(channelName) == true) {
		return send_message_to_server(senderFd, 2, RICK, ERR_USERONCHANNEL, addUserName.c_str());
	}
	userIt->second.invitedChannels.push_back(channelName);
	send_message_to_server(senderFd, 4, RICK, PRIVMSG, channelName.c_str(), COL, DIDINV);
	// send_message_to_server(userIt->first, 3, RICK, RPL_INVITING,
	// 					   userIt->second.getNickName().c_str(), channelName.c_str());
	std::map<int, User>::iterator senderIt;
	senderIt = users.find(senderFd);
	send_message_to_server(userIt->first, 3, senderIt->second.getNickName(), "INVITE",
						   userIt->second.getNickName().c_str(), channelName.c_str());
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
		if (userIt->second.getNickName().compare(kickUserName) == 0) break;
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
	} else if (adminPassword != operator_password)
		std::cout << BADPASS << std::endl;
	else
		std::cout << BADNAME << std::endl;
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/