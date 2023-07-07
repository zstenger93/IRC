#include "../../includes/Server.hpp"

#include "../../includes/Client.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

Server::Server(int argc, char **argv) : reset(true) { Server::inputParser(argc, argv); }
Server::~Server() {}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

int Server::setup() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0), address = 1;
	struct sockaddr_in serverInfo;
	if (serverSocket == -1) {
		return std::cerr << F_SET_SOCKET << std::endl, -1;
	}
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &address, sizeof(address)) == -1) {
		return std::cerr << F_SOCKET_OPT << std::endl, close(serverSocket), -1;
	}
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(getPort());
	if (bind(serverSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
		return std::cerr << F_SOCKET_BIND << std::endl, close(serverSocket), -1;
	}
	if (listen(serverSocket, MAX_CONNECTIONS) == -1) {
		return std::cerr << F_TO_LISTEN << std::endl, close(serverSocket), -1;
	}
	onlineUserCount = 1;
	userPoll[0].fd = serverSocket;
	userPoll[0].events = 0;
	userPoll[0].revents = POLLIN;
	return setServerSocket(serverSocket), setRunning(true), 0;
}

void Server::acceptConnection() {
	struct sockaddr_in clientAdress;
	socklen_t clientAdressLen = sizeof(clientAdress);
	int clientSocket =
		accept(getServerSocket(), (struct sockaddr *)&clientAdress, &clientAdressLen);
	onlineUserCount++;
}

void Server::run() {
	while (Server::isRunning()) {
		// if (poll(userPoll, onlineUserCount, 5000) == -1)
		// 	;
		// for (int i = 0; i < onlineUserCount; i++) {
		// 	try {
		// 		if (whatever) addUser();
		// 		acceptConnection();
		// 		else do_command
		// 	} catch (const std::exception &error) {
		// 		;
		// 	}
		// }
	}
}

/*___________________________________________ SETTERS ___________________________________________*/

// SERVER
void Server::setRunning(bool state) { serverState = state; }
void Server::setServerSocket(int socket) { serverSocketFd = socket; }

/*___________________________________________ GETTERS ___________________________________________*/

// SERVER
bool Server::shouldReset() { return reset; }
bool Server::isRunning() { return serverState; }
int Server::getServerSocket() { return serverSocketFd; }
