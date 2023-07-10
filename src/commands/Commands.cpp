#include "../../includes/Commands.hpp"

#include <fstream>
#include <utility>

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

int Server::processCommands(int pollId) {
	std::string message = "";
	char buffer[512];
	int buffer_len, stringLength = 0;

	memset(buffer, 0, 512);
	buffer_len = recv(userPoll[pollId].fd, buffer, 512, USERDISCONECTED);
	message = buffer;
	stringLength += buffer_len;

	std::map<int, User>::iterator it = users.find(userPoll[pollId].fd);

	if (it->second.isConnected() == false) {
		authenticate(message, it);
	}
	if (buffer_len == USERDISCONECTED) {
		removeUser(pollId);
		onlineUserCount--;
		return USERDISCONECTED;
	} else if (buffer_len == -1)
		throw CustomException(F_FAILED_MESSAGE);
	while (message.find("\r\n") == NOT_FOUND && buffer_len != 0) {
		memset(buffer, 0, 512);
		buffer_len = recv(userPoll[pollId].fd, buffer, 512, 0);
		stringLength += buffer_len;
		message += buffer;
	}
	commandParser(it, message, it->first);
	return 1;
}

void Server::authenticate(std::string message, std::map<int, User>::iterator it) {
	std::string command = getCommand(message);
	bool pass = getPass(message);
	if (command.compare("PASS") == 0) {
		if (pass == true) {
			it->second.connectUser(true);
			std::cout << "OK" << std::endl;
			send_message_to_server(it->first, 2, it->second.getUserName().c_str(), CONNECTED);
			it->second.motd(it->second);
			return;
		} else {
			send_message_to_server(it->first, 1, NOCONNECTION);
			throw(CustomException(WRONGPASS));
		}
	} else if (command.compare("CAP") == 0) {
		send_message_to_server(it->first, 1, LOGIN_REQUIRED);
	}
	std::cout << "KO" << std::endl;
}

bool Server::getPass(std::string& msg) {
	std::size_t firstSpacePos = msg.find(' ');

	if (firstSpacePos != std::string::npos) {
		std::size_t secondSpacePos = msg.find(' ', firstSpacePos + 1);
		if (secondSpacePos == std::string::npos) {
			std::size_t length = secondSpacePos - firstSpacePos - 1;
			std::string secondWord = msg.substr(firstSpacePos + 1, length);
			std::size_t lastCharPos = secondWord.find_last_not_of("\r\n");
			std::string recievedPass = secondWord.substr(0, lastCharPos + 1);

			std::cout << "secw: \"" << recievedPass << "\"" << std::endl;
			if (recievedPass.compare(password) == 0) return true;
		}
	}
	return false;
}

void Server::commandParser(std::map<int, User>::iterator user, std::string message, int fd) {
	int caseId = 0;
	std::string command = getCommand(message);
	std::string commands[14] = {"MESSAGE",	  "JOIN",		"LEAVE", "KICK",	  "INVITE",
								"QUIT",		  "NICK",		"LIST",	 "MODE_USER", "MODE_OPER",
								"TOPIC_USER", "TOPIC_OPER", "CAP",	 "PASS"};
	for (int i = 0; i < 14; i++) {
		if (command.compare(commands[i]) == 0) {
			caseId = i;
			break;
		}
	}
	switch (caseId) {
		case 0:
			user->second.sendMessage();
			break;
		case 1:
			handleJoin(user->second, extractArgument(1, message, 2));
			break;
		case 2:
			user->second.leaveChannel();
			break;
		case 3:
			user->second.kickUser();
			break;
		case 4:
			user->second.inviteUser();
			break;
		case 5:
			// don't have to handle quit, it's automatic
			break;
		case 6:
			user->second.setNick(user, extractArgument(1, message, 2));
			break;
		case 7:
			user->second.listChannels();
			break;
		case 8:
			user->second.modeUser();
			break;
		case 9:
			user->second.modeOper();
			break;
		case 10:
			user->second.topicUser();
			break;
		case 11:
			user->second.topicOper();
			break;
		case 12:;
			// just for silence the error, handled in authentication
			break;
		case 13:;
			handleJoin(user->second, "#General");
			break;
		default:
			send_message_to_server(fd, 1, COMMAND_NOT_FOUND);
			break;
	}
}

void Server::handleJoin(User& user, std::string name) {
	if (name.length() == 0) std::cout << "no channels" << std::endl;
	std::map<std::string, Channel>::iterator it = channels.find(name);
	if (it == channels.end()) {
		createChannel(name);
		send_message_to_server(user.getUserFd(), 3, user.getNickName().c_str(), name.c_str(),
							   CREATEDCHANNEL);
		user.joinChannel(name);
		send_message_to_server(user.getUserFd(), 3, user.getNickName().c_str(), name.c_str(),
							   JOINEDCHANNEL);
		return;
	}
	user.joinChannel(name);
}

void User::sendMessage() {}

void User::joinChannel(std::string name) {
	std::map<std::string, bool>::iterator it = channels.find(name);
	// std::cout << it->second << std::endl;
	if (it == channels.end()) {
		channels.insert(std::make_pair(name, true));
		std::map<std::string, bool>::iterator it = channels.find(name);
		// std::cout << it->second << std::endl;
		return;
	}
	// return you are already on the channel
}

void User::leaveChannel() {}

void User::kickUser() {}

void User::inviteUser() {}

void User::quitServer() {}

void User::setNick(std::map<int, User>::iterator it, std::string newNickname) {
	// needs channel name after username
	if (newNickname.length() != 0) {
		nickName = "\0037" + newNickname + "\0030";
		send_message_to_server(it->first, 2, nickName.c_str(), NICKCHANGED);
	} else {
		send_message_to_server(it->first, 2, nickName.c_str(), NICKEMPTYSTR);
	}
}

void User::listChannels() { std::cout << "test" << std::endl; }

void User::modeUser() {}

void User::modeOper() {}

void User::topicUser() {}

void User::topicOper() {}

void User::emptyFunction() {}

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

/*___________________________________________ SETTERS ___________________________________________*/

/*___________________________________________ GETTERS ___________________________________________*/

std::string Server::getCommand(std::string message) {
	int pos = message.find_first_of(" \t\n");
	std::string command;
	if (pos != NOT_FOUND)
		command = message.substr(0, pos);
	else
		command = message;
	return command;
}
