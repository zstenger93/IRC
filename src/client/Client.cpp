#include "../../includes/Client.hpp"

#include <netinet/in.h>

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/
/*___________________________________________ SETTERS ___________________________________________*/
void Client::setClientSocket(int values) { clientSocket = values; }
void Client::setSocAddr(struct sockaddr_in values) { clientAdress = values; }
/*___________________________________________ GETTERS ___________________________________________*/
struct sockaddr_in Client::getSockAddr() { return clientAdress; }
int Client::getClientSocket() { return clientSocket; }