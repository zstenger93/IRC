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

void Server::handleJoin(std::string message, User& user, std::string name) {
	int op = 0;

	if (name.length() == 0)
		return send_message_to_server(user.getUserFd(), 3, RICK, ERR_NEEDMOREPARAMS, COMMAND, JOIN,
									  COL);
	std::map<std::string, Channel>::iterator channelIt = channels.find(name);
	if (channelIt == channels.end()) {
		createChannel(user, name);
		channelIt = channels.find(name);
		channelIt->second.setChannelTopic(WELCOME);
		op = 1;
	}
	if (!isJoinedWithActiveMode(channelIt->second, user, message)) {
		user.joinChannel(user, name, op);
		loopTroughtTheUsersInChan(name, user.getUserFd(), 1, message, user);
		channelTopic(message, channelIt->first, user.getUserFd());
		loopTroughtTheUsersInChan(name, user.getUserFd(), 2, message, user);
		send_message_to_server(user.getUserFd(), 6, RICK, RPL_NAMREPLY, user.getNickName().c_str(),
							   "=", name.c_str(), COL, "Marvin");
		send_message_to_server(user.getUserFd(), 3, RICK, RPL_ENDOFNAMES,
							   user.getNickName().c_str(), name.c_str(), COL, ENDOFN);
		listChannels(user.getNickName());
	}
}

void Server::sendFiles(std::map<int, User> users, std::string message, int userFd) {
	std::map<int, User>::iterator userIt = users.find(userFd);
	std::string messageTo = extractArgument(1, message, 8);

	if (messageTo.empty() == true)
		return send_message_to_server(userIt->first, 2, RICK, ERR_NEEDMOREPARAMS, COL);
	std::map<int, User>::iterator receiverIt = users.begin();
	for (; receiverIt != users.end(); receiverIt++)
		if (receiverIt->second.getNickName().compare(messageTo) == 0) break;
	if (receiverIt == users.end())
		return send_message_to_server(userIt->first, 3, RICK, ERR_NOSUCHNICK, COL, NOSUCHUSER);

	std::string FileName = extractArgument(4, message, -1);
	std::string IpAdress = extractArgument(5, message, -1);
	std::string PortNumber = extractArgument(6, message, -1);
	std::string FileSize = extractArgument(7, message, -1);

	if (FileName.empty() == true || IpAdress.empty() == true || PortNumber.empty() == true ||
		FileSize.empty() == true)
		send_message_to_server(userIt->first, 2, RICK, ERR_NEEDMOREPARAMS, COL);
	send_message_to_server(receiverIt->second.getUserFd(), 10, userIt->second.getNickName(), NT,
						   receiverIt->second.getNickName().c_str(), COL, DCC, S, FileName.c_str(),
						   IpAdress.c_str(), PortNumber.c_str(), FileSize.c_str(), "\x01");
}

void Server::sendMessage(std::string message, std::map<int, User>& users, int userFd, int pollId,
						 pollfd uPoll[CONNECTIONS], int uCount) {
	std::map<int, User>::iterator userIt = users.find(userFd);

	if (extractArgument(1, message, -1)[0] != '#') {
		std::string messageTo = extractArgument(1, message, -1);
		if (messageTo.empty() == true)
			return send_message_to_server(userIt->first, 3, RICK, ERR_NEEDMOREPARAMS, COL);
		if (messageTo.compare("Marvin") == 0) {
			std::string msg = message.substr(12);
			bot.runAi(userFd, msg, userIt->second, users, pollId, uPoll, uCount);
			return;
		}
		std::map<int, User>::iterator receiverIt = users.begin();
		for (; receiverIt != users.end(); receiverIt++) {
			if (receiverIt->second.getNickName().compare(messageTo) == 0)
				return send_message_to_server(
					receiverIt->first, 4, userIt->second.getNickName().c_str(), PRIVMSG,
					receiverIt->second.getNickName().c_str(), COL, extractMessage(message).c_str());
		}
		if (receiverIt == users.end())
			return send_message_to_server(userIt->first, 3, RICK, ERR_NOSUCHNICK, COL, NOSUCHUSER);
	} else {
		std::string channelName = extractArgument(1, message, -1);
		loopTroughtTheUsersInChan(channelName, userFd, 0, message, userIt->second);
	}
}

void User::leaveChannel(std::map<int, User>& users, User& user, std::string channelName, int mode) {
	std::map<std::string, bool>::iterator channel = channels.find(channelName);

	if (channel == channels.end())
		return send_message_to_server(user.getUserFd(), 5, RICK, ERR_NOSUCHCHANNEL,
									  user.getNickName().c_str(), channelName.c_str(), COL,
									  CANTLEAVE_C);
	if (channelName.compare("#General") == 0 && mode != 1)
		return send_message_to_server(user.getUserFd(), 4, RICK, PRIVMSG, channelName.c_str(), COL,
									  CNTL);
	channels.erase(channel);
	if (mode == 0) {
		send_message_to_server(user.getUserFd(), 2, user.getNickName(), P, channelName.c_str());
		for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
			 usersIt++)
			if (usersIt->second.isInChannel(channelName) == true)
				send_message_to_server(usersIt->first, 4, user.getNickName(), P,
									   channelName.c_str(), COL, LEFT);
	}
	if (mode == 1) {
		send_message_to_server(user.getUserFd(), 5, RICK, K, channelName.c_str(),
							   user.getNickName().c_str(), COL, KCKD);
		for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
			 usersIt++)
			if (usersIt->second.isInChannel(channelName) == true)
				send_message_to_server(usersIt->second.getUserFd(), 5, RICK, K, channelName.c_str(),
									   user.getNickName().c_str(), COL, NKCK);
	}
}

void Server::setNick(User& user, std::string newNickName, std::string msg) {
	if (isNickNameAvailable(newNickName)) {
		std::map<int, User>::iterator userIt = users.begin();
		for (; userIt != users.end(); userIt++) {
			if (userIt->first != user.getUserFd())
				send_message_to_server(userIt->first, 2, user.getNickName().c_str(), N,
									   newNickName.c_str());
		}
		send_message_to_server(user.getUserFd(), 2, user.getNickName().c_str(), N,
							   newNickName.c_str());
		user.setNickName(newNickName);

	} else
		send_message_to_server(user.getUserFd(), 5, RICK, ERR_NICKNAMEINUSE,
							   user.getNickName().c_str(), newNickName.c_str(), COL, ALREADYTAKEN);
}

void Server::setUserName(User& user, std::string newUserName, std::string msg) {
	if (isUserNameAvailable(newUserName)) {
		std::map<int, User>::iterator userIt = users.begin();
		for (; userIt != users.end(); userIt++) {
			if (userIt->first == user.getUserFd()) {
				std::string yourNameChangedTo = CHANGE + newUserName + ".";
				send_message_to_server(userIt->first, 4, RICK, PRIVMSG,
									   userIt->second.getNickName().c_str(), COL,
									   yourNameChangedTo.c_str());
			}
		}
		user.setUserName(newUserName);

	} else
		send_message_to_server(user.getUserFd(), 5, RICK, ERR_NICKNAMEINUSE,
							   user.getNickName().c_str(), newUserName.c_str(), COL, ALREADYTAKEN);
}

void Server::listChannels(std::string userName) {
	std::map<int, User>::iterator userIt = users.begin();

	for (; userIt != users.end(); userIt++)
		if (userIt->second.getNickName().compare(userName) == 0) break;
	if (userIt == users.end())
		return send_message_to_server(userIt->first, 3, RICK, ERR_NOSUCHNICK, COL, NOSUCHUSER);
	send_message_to_server(userIt->first, 5, RICK, RPL_STARTLIST, userName.c_str(), CH, COL, NM);
	for (std::map<std::string, Channel>::iterator channelIt = channels.begin();
		 channelIt != channels.end(); channelIt++)
		send_message_to_server(userIt->first, 3, RICK, RPL_LIST, userName.c_str(),
							   channelIt->second.getChannelName().c_str());
	send_message_to_server(userIt->first, 4, RICK, RPL_LISTEND, userName.c_str(), COL, ENDOFC);
}

void Server::mode(std::string message, int userFd) {
	if (Parser::getWordCount(message) > 4 || Parser::getWordCount(message) < 2) return;
	std::string channelName = extractArgument(1, message, -1);
	std::string mode = "";
	std::map<int, User>::iterator userIt = users.find(userFd);
	std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);
	bool add = false;

	if (channelIt == channels.end())
		return send_message_to_server(userFd, 3, RICK, ERR_NOSUCHCHANNEL, COL, NOSUCHCHAN);
	if (Parser::getWordCount(message) == 2) {
		const std::map<std::string, bool> modes = channelIt->second.getChannelModes();
		for (std::map<std::string, bool>::const_iterator modeIt = modes.begin();
			 modeIt != modes.end(); modeIt++)
			if (modeIt->second == true) mode += modeIt->first;
		send_message_to_server(userFd, 4, RICK, M, channelName.c_str(), COL, mode.c_str());
	} else if ((Parser::getWordCount(message) == 3 || Parser::getWordCount(message) == 4) &&
			   userIt->second.isOperatorInChannel(channelName)) {
		mode = extractArgument(2, message, -1);
		if (mode[0] == 'b' && mode.size() == 1) return;
		if (isModeValid(mode) == false)
			return send_message_to_server(userIt->first, 3, RICK, ERR_NOSUCHSERVICE, COL, NOMODE);
		if (mode[0] == '+')
			addMode(channelIt->second, userIt->second, mode, message);
		else if (mode[0] == '-')
			removeMode(channelIt->second, userIt->second, mode, message);
	}
}

void Server::channelTopic(std::string message, std::string channelName, int userFd) {
	std::map<int, User>::iterator userIt = users.find(userFd);
	std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);

	if (channelIt == channels.end())
		return send_message_to_server(userFd, 3, RICK, ERR_NOSUCHCHANNEL, COL, NOSUCHCHAN);
	if (Parser::getWordCount(message) == 2)
		send_message_to_server(userFd, 5, RICK, RPL_TOPIC, userIt->second.getNickName().c_str(),
							   channelIt->second.getChannelName().c_str(), COL,
							   channelIt->second.getChannelTopic().c_str());
	if (Parser::getWordCount(message) > 2 && userIt->second.isOperatorInChannel(channelName) &&
		!channelIt->second.checkMode("t")) {
		std::string command = extractArgument(0, message, -1);
		int newTopicStartPos = command.length() + 1 + channelName.length() + 1 + 1;
		if (command.compare(T) != 0) newTopicStartPos = newTopicStartPos - 1;
		channelIt->second.setChannelTopic(message.substr(newTopicStartPos));

		for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end();
			 usersIt++)
			if (usersIt->second.isInChannel(channelName) == true)
				send_message_to_server(usersIt->first, 4, userIt->second.getNickName().c_str(), T,
									   channelIt->second.getChannelName().c_str(), COL,
									   channelIt->second.getChannelTopic().c_str());
	}
}

void User::ping(std::string message, int userFd) {
	if (message.length() < 6) return;
	send_message_to_server(userFd, 3, RICK, "PONG", COL, message.substr(5).c_str());
}

void Server::who(int userFd, std::string message) {
	if (Parser::getWordCount(message) > 2)
		return send_message_to_server(userFd, 1, RICK, ERR_TOOMANYTARGETS);
	std::map<int, User>::iterator user = users.find(userFd);
	std::string userNames = "";
	std::map<int, User>::iterator userIt = users.begin();

	for (; userIt != users.end(); userIt++)
		if (userIt->second.getUserFd() != userFd)
			send_message_to_server(userFd, 5, user->second.getNickName().c_str(), RPL_WHOREPLY,
								   RICK, userIt->second.getNickName().c_str(), COL,
								   userIt->second.getNickName().c_str());
	send_message_to_server(userFd, 3, user->second.getNickName(), RPL_ENDOFWHO, COL, ENDOFW);
}

void Server::whois(int userFd, std::string message) {
	std::map<int, User>::iterator user = users.find(userFd);
	send_message_to_server(userFd, 4, RICK, PRIVMSG, user->second.getNickName().c_str(), COL,
						   NOSTEELIN);
}

void Server::motd(int userFd, std::string channelName) {
	std::map<int, User>::iterator userIt = users.find(userFd);
	std::ifstream file("conf/motd.txt");
	std::string line;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			if (channelName.empty() == false)
				send_message_to_server(userFd, 4, RICK, PRIVMSG, channelName.c_str(), COL,
									   line.c_str());
			else
				send_message_to_server(userFd, 3, RICK, PRIVMSG, COL, line.c_str());
		}
		file.close();
	} else {
		if (channelName.empty() == false)
			send_message_to_server(userFd, 4, RICK, PRIVMSG, channelName.c_str(), COL, FOFF);
		else
			send_message_to_server(userFd, 3, RICK, PRIVMSG, COL, FOFF);
	}
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/