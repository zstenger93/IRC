#include "../../includes/Channel.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/ReplyCodes.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void Server::authenticate(std::string message, std::map<int, User>::iterator it) {
	std::string command = getCommand(message);
	bool pass = getPass(message);
	if (command.compare("PASS") == 0) {
		if (pass == true) {
			it->second.connectUser(true);
			send_message_to_server(it->first, 5, RICK, RPL_WELCOME,
								   it->second.getNickName().c_str(), COL,
								   it->second.getUserName().c_str(), CONNECTED);
			return;
		} else {
			send_message_to_server(it->first, 4, RICK, ERR_PASSWDMISMATCH, SERVER, COL,
								   NOCONNECTION);
			throw(CustomException(WRONGPASS));
		}
	} else if (command.compare("CAP") == 0) {
		send_message_to_server(it->first, 3, RICK, ERR_ALREADYREGISTRED, COL, LOGIN_REQUIRED);
	}
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
			if (recievedPass.compare(password) == 0) return true;
		}
	}
	return false;
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/

std::string Server::getCommand(std::string message) {
	int pos = message.find_first_of(" \r\t\n");
	std::string command;
	if (pos != NOT_FOUND)
		command = message.substr(0, pos);
	else
		command = message;
	return command;
}
