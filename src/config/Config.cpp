#include "../../includes/Server.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void Server::setConnectionLimits() {
	std::string line, limitType;
	std::ifstream file("conf/irc.conf");
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		if (line.find("max_connections") != std::string::npos) {
			setMaxLimit(std::atoi(extractWord(line).c_str()));
		}
		if (line.find("allowed_connections") != std::string::npos) {
			setAllowedLimit(std::atoi(extractWord(line).c_str()));
		}
	}
	file.close();
}

void Server::setServerPassword() {
	std::string line, limitType;
	std::ifstream file("conf/irc.conf");
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		if (line.find("server_password") != std::string::npos) {
			setPassword(base64Decode(extractWord(line)));
			return;
		}
	}
	file.close();
}

/*___________________________________________ SETTERS ___________________________________________*/

void Server::setPassword(std::string serverPassword) { password = serverPassword; }
void Server::setMaxLimit(int maxLimit) { max_connections = maxLimit; }
void Server::setAllowedLimit(int allowedLimit) { allowed_connections = allowedLimit; }

/*___________________________________________ GETTERS ___________________________________________*/

std::string Server::getPassword() { return password; }