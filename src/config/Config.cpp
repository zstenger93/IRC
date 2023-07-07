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
	std::cout << "max connections: " << max_connections << std::endl;
	std::cout << "allowed connections: " << allowed_connections << std::endl;
}

/*___________________________________________ SETTERS ___________________________________________*/

void Server::setMaxLimit(int maxLimit) { max_connections = maxLimit; }
void Server::setAllowedLimit(int allowedLimit) { allowed_connections = allowedLimit; }

/*___________________________________________ GETTERS ___________________________________________*/