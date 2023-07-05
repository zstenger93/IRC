#include "../../includes/Server.hpp"

#include "../../includes/Client.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
// Server::Server() : serverState(true) {}
// Server::~Server(){}
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/
int Server::setup() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	int adress = 1;
	struct sockaddr_in serverInfo;
	if (serverSocket == -1) {
		std::cerr << F_SET_SOCKET << std::endl;
		return -1;
	}
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &adress, sizeof(adress)) == -1) {
		std::cerr << F_SOCKET_OPT << std::endl;
		close(serverSocket);
		return -1;
	}
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(getPort());
	if (bind(serverSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
		std::cerr << F_SOCKET_BIND << std::endl;
		close(serverSocket);
		return -1;
	}
	if (listen(serverSocket, MAX_CONNECTIONS) == -1) {
		std::cerr << F_TO_LISTEN << std::endl;
		close(serverSocket);
		return -1;
	}
	setServerSocket(serverSocket);
	setRunning(true);
	return (0);
}



void Server::run() {
	struct sockaddr_in clientAdress;
	socklen_t clientAdressLen = sizeof(clientAdress);
	int clientSocket =
		accept(getServerSocket(), (struct sockaddr *)&clientAdress, &clientAdressLen);
	if (clientSocket == -1) {
		std::cerr << "Failed to accept the connection " << std::endl;
	}
	close(clientSocket);
}
/*___________________________________________ SETTERS ___________________________________________*/
void Server::setServerSocket(int socket) { serverSocketFd = socket; }
/*___________________________________________ GETTERS ___________________________________________*/
int Server::getServerSocket() { return (serverSocketFd); }