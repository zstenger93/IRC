#include "../../includes/Client.hpp"

#include <sys/poll.h>

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
Client::Client() {}
Client::~Client() { std::cout << "Client has been yeeted" << std::endl; }
Client::Client(int socket, sockaddr_in adress) : clientSocket(socket), clientAdress(adress) {
	pollFd.fd = socket;
	pollFd.events = POLLIN;
	pollFd.revents = 0;
	std::cout << "Client has been constructed" << std::endl;
}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/
/*___________________________________________ SETTERS ___________________________________________*/

void Client::setClientSocket(int values) { clientSocket = values; }
void Client::setSocAddr(struct sockaddr_in values) { clientAdress = values; }

/*___________________________________________ GETTERS ___________________________________________*/

struct sockaddr_in Client::getSockAddr() { return clientAdress; }
int Client::getClientSocket() const { return clientSocket; }
pollfd Client::getClientPoll() { return pollFd; }