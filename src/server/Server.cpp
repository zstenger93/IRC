#include "../../includes/Server.hpp"

#include "../../includes/Client.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

Server::Server(int argc, char **argv) : reset(true) {
	Server::setServerPassword();
	Server::setAdminDetails();	// kinda extra, not needed will decide later
	Server::setConnectionLimits();
	Server::inputParser(argc, argv);
}

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
	if (listen(serverSocket, allowed_connections) == -1) {
		return std::cerr << F_TO_LISTEN << std::endl, close(serverSocket), -1;
	}
	userPoll[0].fd = serverSocket;
	userPoll[0].events = POLLIN;
	userPoll[0].revents = 0;
	onlineUserCount = 1;
	return setServerSocket(serverSocket), setRunning(true), 0;
}

void Server::acceptConnection() {
	struct sockaddr_in clientAdress;
	socklen_t clientAdressLen = sizeof(clientAdress);
	int clientSocket =
		accept(getServerSocket(), (struct sockaddr *)&clientAdress, &clientAdressLen);
	
	userPoll[onlineUserCount].fd = clientSocket;
	userPoll[onlineUserCount].events = POLLIN;
	userPoll[onlineUserCount].revents = 0;
	onlineUserCount++;
}

// User 1 User 2 User 3 // 3
// User 1 User 3 // 2

void Server::removeUser(int pollId) {}

int Server::processInput(int pollId) {
	char buffer[40];
	memset(buffer, 0, 40);
	int cmd = recv(userPoll[pollId].fd, buffer, 40, 0);
	if (cmd == 0) {
		removeUser(pollId);
		onlineUserCount--;
		return (USERDISCONECTED);
	}
	std::cout << cmd << std::endl;
	std::cout << buffer << std::endl;
	return (0);
}

void Server::run() {
	// std::cout << "Inside run" << std::endl;
	while (Server::isRunning()) {
		if (poll(userPoll, onlineUserCount, 5000) == -1) throw CustomException("except");
		for (int pollId = 0; pollId < onlineUserCount; pollId++) {
			if (userPoll[pollId].revents & POLLIN) {
				if (userPoll[pollId].fd == serverSocketFd) {
					acceptConnection();
					std::cout << onlineUserCount << std::endl;
				} else {
					if (processInput(pollId) == USERDISCONECTED) pollId--;
				}
			}
		}
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

void Server::setRunning(bool state) { serverState = state; }
void Server::setServerSocket(int socket) { serverSocketFd = socket; }

/*___________________________________________ GETTERS ___________________________________________*/

bool Server::shouldReset() { return reset; }
bool Server::isRunning() { return serverState; }
int Server::getServerSocket() { return serverSocketFd; }
