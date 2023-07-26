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
		if (user.isInvitedToChannel(channel.getChannelName()) == false) {
			send_message_to_server(user.getUserFd(), 5, RICK, ERR_INVITEONLYCHAN,
								   user.getNickName().c_str(), channel.getChannelName().c_str(),
								   COL, INVITENEEDED);
			return INVITEONLY;
		}
	}
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
			send_message_to_server(user.getUserFd(), 4, RICK, ERR_NEEDMOREPARAMS,
								   user.getNickName().c_str(),
								   extractArgument(0, message, -1).c_str(), COL);
			return ACTIVEMODEERROR;
		}
	}
	if (channel.checkMode("k") == true || channel.checkMode("l") == true ||
		channel.checkMode("i") == true) {
		if (channel.checkMode("l") == true) channel.changeUserCount(++userCount);
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
/*
case 0:

case 1:

case 2:

case 3:

case 4:

*/
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
					send_message_to_server(userIt->first, 4, user.getNickName(), "JOIN",
										   channelName.c_str(), COL, channelName.c_str());
					break;
				case 2:
					if (userIt->second.isOperatorInChannel(channelName)) {
						std::string name = "@" + userIt->second.getNickName();
						send_message_to_server(senderFd, 6, RICK, RPL_NAMREPLY,
											   user.getNickName().c_str(), "=", channelName.c_str(),
											   COL, name.c_str());
					} else
						send_message_to_server(senderFd, 6, RICK, RPL_NAMREPLY,
											   user.getNickName().c_str(), "=", channelName.c_str(),
											   COL, userIt->second.getNickName().c_str());
					break;
				case 3:
					send_message_to_server(userIt->second.getUserFd(), 4, RICK, M,
										   channelName.c_str(), ADDOP, message.c_str());
					break;
				case 4:
					send_message_to_server(userIt->second.getUserFd(), 4, RICK, M,
										   channelName.c_str(), REMOVEOP, message.c_str());
					break;
				// case 5:
				// 	send_message_to_server(userIt->second.getUserFd(), 3, user.getNickName(), M,
				// 						   channelName.c_str(), message.c_str());
				// 	break;
				default:
					break;
			}
		}
	}
}

bool User::isInChannel(std::string channelName) {
	return (channels.find(channelName) != channels.end());
}

void Server::addModeO(User& user, std::string msg) {
	if (Parser::getWordCount(msg) != 4) return;

	std::string channelName = extractArgument(1, msg, 4);
	std::string targetUser = extractArgument(3, msg, 4);

	if (userExists(targetUser) == false)
		return send_message_to_server(user.getUserFd(), 3, RICK, ERR_NOSUCHNICK, COL, NOSUCHUSER);

	User& secondUser = getUser(targetUser);

	if (secondUser.isInChannel(channelName) == false)
		return send_message_to_server(user.getUserFd(), 3, RICK, ERR_USERNOTINCHANNEL,
									  users.find(user.getUserFd())->second.getNickName().c_str(),
									  channelName.c_str(), COL, NOTINCHAN);

	if (secondUser.isOperatorInChannel(channelName) == true)
		return send_message_to_server(user.getUserFd(), 4, RICK, PRIVMSG, channelName.c_str(), COL,
									  ALREADYOPER);

	secondUser.setOperatorPrivilage(channelName, true);
	send_message_to_server(user.getUserFd(), 4, RICK, M, channelName.c_str(), ADDOP,
						   targetUser.c_str());
	loopTroughtTheUsersInChan(channelName, user.getUserFd(), 3, targetUser, user);
}

bool Server::isModeValid(std::string mode) {
	if (mode.size() != 2) return false;
	if (mode[1] == 'i' || mode[1] == 'o' || mode[1] == 'k' || mode[1] == 't' || mode[1] == 'l')
		return true;
	return false;
}

void Server::addMode(Channel& channel, User& user, std::string mode, std::string msg) {
	mode = mode.substr(1);
	if (mode.compare("o") == 0)
		addModeO(user, msg);
	if (mode.compare("k") == 0 || mode.compare("l") == 0) {
		if (Parser::getWordCount(msg) != 4)
			return send_message_to_server(user.getUserFd(), 2, RICK, ERR_NEEDMOREPARAMS, COL);
	}
	else if (mode.compare("k") == 0)
		channel.setChannelPassword(extractArgument(3, msg, 4));
	else if (mode.compare("l") == 0)
		channel.setChannelUserLimit(std::atoi(extractArgument(3, msg, 4).c_str()));
	else if ((mode.compare("i") == 0 || mode.compare("t") == 0) && Parser::getWordCount(msg) == 3) {
	}
	channel.addMode(mode, true);
	for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++) {
		if (usersIt->second.isInChannel(channel.getChannelName()) == true)
			send_message_to_server(usersIt->first, 3, user.getNickName().c_str(), M,
								   channel.getChannelName().c_str(), mode.c_str());
	}
}

void Server::removeMode(Channel& channel, User& user, std::string mode, std::string msg) {
	channel.addMode(mode.substr(1), false);
	for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++) {
		if (usersIt->second.isInChannel(channel.getChannelName()) == true) {
			if (mode == "-o") {
				std::string userToRemoveFrom = extractArgument(3, msg, 4);
				if (usersIt->second.getNickName().compare(userToRemoveFrom) == 0) {
					usersIt->second.setOperatorPrivilage(channel.getChannelName(), false);
					loopTroughtTheUsersInChan(channel.getChannelName(), user.getUserFd(), 4,
											  usersIt->second.getNickName(), user);
					send_message_to_server(user.getUserFd(), 4, RICK, M,
										   channel.getChannelName().c_str(), REMOVEOP,
										   usersIt->second.getNickName().c_str());
					break;
				}
			} else {
				send_message_to_server(usersIt->second.getUserFd(), 4, user.getNickName(), M,
									   channel.getChannelName().c_str(), mode.c_str(),
									   usersIt->second.getNickName().c_str());
			}
		}
	}
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/