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

	std::map<int, User>::iterator it = users.find(pollId);
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
	commandParser(stringLength, message, it->first);
	return 1;
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

void Server::authenticate(std::string message, std::map<int, User>::iterator it) {
	std::string command = getCommand(message);
	bool pass = getPass(message);
	if (command.compare("PASS") == 0) {
		if (pass == true) {
			it->second.connectUser(true);
			std::cout << "OK" << std::endl;
			send_message_to_server(it->first, 1, CONNECTED);
			return;
		}
	} else if (command.compare("CAP") == 0) {
		send_message_to_server(it->first, 1, LOGIN_REQUIRED);
	} else {
		send_message_to_server(it->first, 1, NOCONNECTION);
		throw(CustomException(WRONGPASS));
	}
	std::cout << "KO" << std::endl;
}

// void Server::CommandExecutionChecker(int stringLength, std::string message, std::string command)
// { 	std::cout << command << " " << message.length() << " " << stringLength << std::endl;
// }

void Server::message() {}

void Server::joinChannel() {}

void Server::leaveChannel() {}

void Server::kick() {}

void Server::invite() {}

void Server::quitServer() {}

void Server::nick() {}

void Server::listChannels() { std::cout << "test" << std::endl; }

void Server::modeUser() {}

void Server::modeOper() {}

void Server::topicUser() {}

void Server::topicOper() {}

void Server::emptyFunction() {}

void Server::commandParser(int stringLength, std::string message, int fd) {
	int i = 0;
	std::string command = getCommand(message);
	std::string commands[14] = {"MESSAGE",	  "JOIN",		"LEAVE", "KICK",	  "INVITE",
								"QUIT",		  "NICK",		"LIST",	 "MODE_USER", "MODE_OPER",
								"TOPIC_USER", "TOPIC_OPER", "CAP", "PASS"};
	void (Server::*functions[14])() = {

		functions[0] = &Server::message,	  functions[1] = &Server::joinChannel,
		functions[2] = &Server::leaveChannel, functions[3] = &Server::kick,
		functions[4] = &Server::invite,		  functions[5] = &Server::quitServer,
		functions[6] = &Server::nick,		  functions[7] = &Server::listChannels,
		functions[8] = &Server::modeUser,	  functions[9] = &Server::modeOper,
		functions[10] = &Server::topicUser,	  functions[11] = &Server::topicOper,
		functions[12] = &Server::emptyFunction, functions[13] = &Server::emptyFunction};

	for (i = 0; i < 14; i++) {
		if (command.compare(commands[i]) == 0) {
			(this->*functions[i])();
			break;
		}
	}
	if (i == 14) send_message_to_server(fd, 1, COMMAND_NOT_FOUND);
	// CommandExecutionChecker(stringLength, message, command);
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

// TODO
// CHECK FOR THE COMMAND IN THE ARRAY OF THE COMMANDS
// SEND IN THE SPECIFIC STRUCT from the struct list depending from the command, wirh a void
// pointer execute the command that is rquired
//