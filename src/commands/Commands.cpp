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
	commandParser(stringLength, message);
	return 1;
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

void Server::listChannels() {std::cout << "test" << std::endl; }

void Server::modeUser() {}

void Server::modeOper() {}

void Server::topicUser() {}

void Server::topicOper() {}

void Server::commandParser(int stringLength, std::string message) {
	int caseId;
	std::string command = getCommand(message);
	std::string commands[12] = {"MESSAGE",	 "JOIN",	  "LEAVE",		"KICK",
								"INVITE",	 "QUIT",	  "NICK",		"LIST",
								"MODE_USER", "MODE_OPER", "TOPIC_USER", "TOPIC_OPER"};
	void (Server::*functions[12])() = {

		functions[0] = &Server::message,	  functions[1] = &Server::joinChannel,
		functions[2] = &Server::leaveChannel, functions[3] = &Server::kick,
		functions[4] = &Server::invite,		  functions[5] = &Server::quitServer,
		functions[6] = &Server::nick,		  functions[7] = &Server::listChannels,
		functions[8] = &Server::modeUser,	  functions[9] = &Server::modeOper,
		functions[10] = &Server::topicUser,	  functions[11] = &Server::topicOper};
		
	for (int i = 0; i < 12; i++) {
		if (command.compare(commands[i]) == 0) {
			(this->*functions[i])();
			break;
		}
	}
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