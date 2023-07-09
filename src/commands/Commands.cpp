#include <netdb.h>

#include "../../includes/Server.hpp"

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

void Server::commandParser(std::map<int, User>::iterator it, std::string message, int fd) {
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
		case 6:
			it->second.setNick(it, "JOHN");
			break;
	}
	// if (i == 14) send_message_to_server(fd, 1, COMMAND_NOT_FOUND);
}

void User::message() {}

void User::joinChannel() {}

void User::leaveChannel() {}

void User::kick() {}

void User::invite() {}

void User::quitServer() {}

void User::setNick(std::map<int, User>::iterator it, std::string newNickname) {
	nickName = newNickname;
	send_message_to_server(it->first, 2, nickName.c_str(), NICKCHANGED);
}

void User::listChannels() { std::cout << "test" << std::endl; }

void User::modeUser() {}

void User::modeOper() {}

void User::topicUser() {}

void User::topicOper() {}

void User::emptyFunction() {}

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
