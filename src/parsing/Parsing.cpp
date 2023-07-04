#include "../../includes/Server.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/

const char *Server::WrongArgCountException::what() const throw() { return WRONGINPUT; }
const char *Server::WrongPortException::what() const throw() { return WRONGPORT; }
const char *Server::WrongPassException::what() const throw() { return WRONGPASS; }
const char *Server::LongPassException::what() const throw() { return LONGPASS; }

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
	if (passwordCheck(inputPass) == false) throw(WrongPassException());
}

static bool validCharacters(char c) { return (c < 48 || c > 57) && (c < 65 || c > 122); }

bool Server::passwordCheck(std::string psswrd) {
	if (psswrd.length() > 8) throw(LongPassException());
	std::string::iterator iter = std::find_if(psswrd.begin(), psswrd.end(), validCharacters);
	if (iter != psswrd.end()) return false;
	setPassword(psswrd);
	std::cout << getPassword() << std::endl;

	return true;
}

/*___________________________________________ SETTERS ___________________________________________*/

void Server::setPort(int portNum) { port = portNum; }
void Server::setPassword(std::string psswrd) { password = psswrd; }

/*___________________________________________ GETTERS ___________________________________________*/

bool Server::isRunning() { return serverState; }
int Server::getPort() { return port; }
std::string Server::getPassword() { return password; }
