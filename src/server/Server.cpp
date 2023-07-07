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
		return CustomException(F_SET_SOCKET), -1;
	}
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &address, sizeof(address)) == -1) {
		return throw CustomException(F_SOCKET_OPT), -1;
	}
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(getPort());
	if (bind(serverSocket, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
		return throw CustomException(F_SOCKET_BIND), close(serverSocket), -1;
	}
	if (listen(serverSocket, allowed_connections) == -1) {
		return throw CustomException(F_TO_LISTEN), close(serverSocket), -1;
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

void Server::removeUser(int pollId) {
	close(userPoll[pollId].fd);
	while (pollId < onlineUserCount) {
		userPoll[pollId].events = userPoll[pollId + 1].events;
		userPoll[pollId].revents = userPoll[pollId + 1].revents;
		userPoll[pollId].fd = userPoll[pollId + 1].fd;
		userPoll[pollId] = userPoll[pollId + 1];
		pollId++;
	}
	userPoll[pollId].events = 0;
	userPoll[pollId].revents = 0;
	userPoll[pollId].fd = 0;
}

void Server::CommandExecutionChecker(int stringLength, std::string message, std::string command)
{
}

void Server::commandParser(int stringLength, std::string message) {
	int pos = message.find_first_of(" \t\n");
	std::string command;
	if (pos != NOT_FOUND)
		command = message.substr(0, pos);
	else
		command = message;
	std::cout <<command<<" "<< message.length() <<" "<<stringLength<<std::endl;
}

int Server::processInput(int pollId) {
	std::string message = "";
	char buffer[512];
	int buffer_len;
	int stringLength = 0;

	memset(buffer, 0, 512);
	buffer_len = recv(userPoll[pollId].fd, buffer, 512, 0);
	message = buffer;
	stringLength += buffer_len;
	if (buffer_len == 0) {
		removeUser(pollId);
		onlineUserCount--;
		return (USERDISCONECTED);
	} else if (buffer_len == -1)
		throw CustomException(F_FAILED_MESSAGE);
	while (message.find("\r\n") == NOT_FOUND && buffer_len != 0) {
		memset(buffer, 0, 512);
		buffer_len = recv(userPoll[pollId].fd, buffer, 512, 0);
		stringLength += buffer_len;
		message += buffer;
	}
	commandParser(stringLength, message);
	return (0);
}

void Server::run() {
	while (Server::isRunning()) {
		if (poll(userPoll, onlineUserCount, 5000) == -1) throw CustomException("except");
		for (int pollId = 0; pollId < onlineUserCount; pollId++) {
			try {
				if (userPoll[pollId].revents & POLLIN) {
					if (userPoll[pollId].fd == serverSocketFd) {
						acceptConnection();
					} else {
						if (processInput(pollId) == USERDISCONECTED) pollId--;
					}
				}
			} catch (const std::exception &error) {
				std::cout << RED << error.what() << std::endl;
			}
		}
	}
}

/*___________________________________________ SETTERS ___________________________________________*/

void Server::setRunning(bool state) { serverState = state; }
void Server::setServerSocket(int socket) { serverSocketFd = socket; }

/*___________________________________________ GETTERS ___________________________________________*/

bool Server::shouldReset() { return reset; }
bool Server::isRunning() { return serverState; }
int Server::getServerSocket() { return serverSocketFd; }
