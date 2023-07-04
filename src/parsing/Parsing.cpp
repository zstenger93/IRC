#include "../../includes/Server.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/

const char *Server::WrongArgCountException::what() const throw() { return WRONGINPUT; }
const char *Server::WrongPortException::what() const throw() { return WRONGPORT; }

/*__________________________________________ FUNCTIONS __________________________________________*/

void Server::inputParser(int argc, char **argv) {
	if (argc != 3) throw(WrongArgCountException());
	std::string inputPort = argv[1], inputPass = argv[2];
	int port = std::atoi(inputPort.c_str());
	if (port > 6659 && port < 6670)
		setPort(port);
	else
		throw(WrongPortException());
	std::cout << getPort() << std::endl;
	setPassword(inputPass);
	std::cout << getPassword() << std::endl;
}

/*___________________________________________ SETTERS ___________________________________________*/

void Server::setPort(int portNum) { port = portNum; }
void Server::setPassword(std::string psswrd) { password = psswrd; }
void Server::setRunning(bool state) { serverState = state; }

/*___________________________________________ GETTERS ___________________________________________*/

bool Server::isRunning() { return serverState; }
int Server::getPort() { return port; }
std::string Server::getPassword() { return password; }
