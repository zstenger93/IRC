#include "../../includes/Channel.hpp"
#include "../../includes/Commands.hpp"
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
			std::cout << "OK" << std::endl;
			send_message_to_server(it->first, 5, "001", it->second.getNickName().c_str(), ":",
								   it->second.getUserName().c_str(), CONNECTED);
			it->second.motd(it->second);
			return;
		} else {
			send_message_to_server(it->first, 1, NOCONNECTION);
			throw(CustomException(WRONGPASS));
		}
	} else if (command.compare("CAP") == 0) {
		send_message_to_server(it->first, 2, "462 : ", LOGIN_REQUIRED);
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