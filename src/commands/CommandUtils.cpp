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

int Server::isJoinedWithActiveMode(Channel& channel, User& user, std::string message) {
	int userCount = channel.getUserCount(), userLimit = channel.getUserLimit();
	if (channel.checkMode("i") == true) {
		send_message_to_server(user.getUserFd(), 5, RICK, ERR_INVITEONLYCHAN,
							   user.getNickName().c_str(), channel.getChannelName().c_str(), COL,
							   INVITENEEDED);
		return INVITEONLY;
	}
	channel.checkMode("k");
	if (userCount < userLimit && channel.checkMode("l") == true) {
	} else if (userCount == userLimit && channel.checkMode("l")) {
		send_message_to_server(user.getUserFd(), 5, RICK, ERR_CHANNELISFULL,
							   user.getNickName().c_str(), channel.getChannelName().c_str(), COL,
							   USERLIMITREACHED);
		return ACTIVEMODEERROR;
	}
	if (channel.checkMode("k") == true) {
		if (Parser::getWordCount(message) == 3) {
			std::string providedPass = extractArgument(2, message, 3);
			if (channel.isPasswordCorrect(providedPass)) {
			} else {
				send_message_to_server(user.getUserFd(), 5, RICK, ERR_BADCHANNELKEY,
									   user.getNickName().c_str(), channel.getChannelName().c_str(),
									   COL, W_CHANPASS);
				return ACTIVEMODEERROR;
			}
		} else {
			send_message_to_server(user.getUserFd(), 5, RICK, ERR_BADCHANNELKEY,
								   user.getNickName().c_str(), channel.getChannelName().c_str(),
								   COL, ERR_TOOMANYTARGETS);
			return ACTIVEMODEERROR;
		}
	}
	if (channel.checkMode("k") == true || channel.checkMode("l") == true) {
		if (channel.checkMode("l") == true) channel.changeUserCount(++userCount);
		user.joinChannel(user, channel.getChannelName(), 0);
		return true;
	}
	return false;
}

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
							   channelName.c_str(), COL, NOTINCHAN);
		return false;
	}
	return true;
}

void Server::loopTroughtTheUsersInChan(std::string channelName, int senderFd, int mode,
									   std::string message, User& user) {
	if (checkIfCanBeExecuted(channelName, senderFd) == false) return;
	std::map<int, User>::iterator userIt = users.begin();
	for (; userIt != users.end(); userIt++) {
		if (userIt->second.isInChannel(channelName) && userIt->second.getUserFd() != senderFd) {
			switch (mode) {
				case 0:
					send_message_to_server(userIt->first, 4, user.getNickName().c_str(), PRIVMSG,
										   channelName.c_str(), COL,
										   extractMessage(message).c_str());
					break;
				case 1:
					send_message_to_server(userIt->first, 4, user.getUserName(), "JOIN",
										   channelName.c_str(), COL, channelName.c_str());
					break;
				case 2:
					send_message_to_server(senderFd, 6, RICK, RPL_NAMREPLY,
										   user.getNickName().c_str(), "=", channelName.c_str(),
										   COL, userIt->second.getNickName().c_str());
				default:
					break;
			}
		}
	}
}

bool User::isInChannel(std::string channelName) {
	return (channels.find(channelName) != channels.end());
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/