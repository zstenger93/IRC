#include "../../includes/Server.hpp"

#include <string>
#include <variant>

#include "../../includes/Channel.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

Server::Server() {}

Server::Server(int argc, char **argv) : reset(true), hostmask("127.0.0.1") {
	Server::setServerPassword();
	Server::setAdminDetails();
	Server::setConnectionLimits();
	Server::inputParser(argc, argv);
	bot.constructBot();
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

void Server::run() {
	while (Server::isRunning()) {
		if (poll(userPoll, onlineUserCount, 5000) == -1) throw CustomException("except");
		for (int pollId = 0; pollId < onlineUserCount; pollId++) {
			try {
				if (userPoll[pollId].revents & POLLIN) {
					if (userPoll[pollId].fd == serverSocketFd) {
						acceptConnection();
					} else {
						if (processCommands(pollId) == USERDISCONECTED) pollId--;
					}
				}
			} catch (const std::exception &error) {
				std::cout << RED << error.what() << END << std::endl;
			}
		}
	}
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
	addUser(clientSocket);
}

void Server::addUser(int userFd) {
	static int i = 1;
	users.insert(std::make_pair(userFd, User(userFd, "user" + std::to_string(i++))));
}

void Server::sendUserRemoved(User &user) {
	for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++) {
		for (std::map<std::string, Channel>::iterator channelsIt = channels.begin();
			 channelsIt != channels.end(); channelsIt++) {
			if (usersIt->second.isInChannel(channelsIt->second.getChannelName()) &&
				user.isInChannel(channelsIt->second.getChannelName())) {
				if (usersIt->second.getNickName() != user.getNickName())
					send_message_to_server(usersIt->first, 4, user.getNickName().c_str(), P,
										   channelsIt->second.getChannelName().c_str(), COL, LEFT);
			}
		}
	}
}

void Server::removeUser(int pollId) {
	std::map<int, User>::iterator userIt = users.find(userPoll[pollId].fd);
	if (userIt != users.end()) users.erase(userIt);
	close(userPoll[pollId].fd);
	while (pollId < onlineUserCount) {
		userPoll[pollId].events = userPoll[pollId + 1].events;
		userPoll[pollId].revents = userPoll[pollId + 1].revents;
		userPoll[pollId].fd = userPoll[pollId + 1].fd;
		userPoll[pollId] = userPoll[pollId + 1];
		pollId++;
	}
	onlineUserCount--;
	userPoll[pollId].events = 0;
	userPoll[pollId].revents = 0;
	userPoll[pollId].fd = 0;
}

bool Server::userExists(std::string userNickName) {
	std::map<int, User>::iterator usersIt = users.begin();

	for (; usersIt != users.end(); usersIt++) {
		if (usersIt->second.getNickName().compare(userNickName) == 0) return true;
	}
	return false;
}

bool Server::isNickNameAvailable(std::string nickName) {
	std::map<int, User>::iterator usersIt = users.begin();
	if (nickName.find("Marvin") != std::string::npos) return false;
	for (; usersIt != users.end(); usersIt++) {
		if (usersIt->second.getNickName().compare(nickName) == 0) return false;
	}
	return true;
}

bool Server::isUserNameAvailable(std::string userName) {
	std::map<int, User>::iterator usersIt = users.begin();

	for (; usersIt != users.end(); usersIt++) {
		if (usersIt->second.getUserName().compare(userName) == 0) return false;
	}
	return true;
}

/*___________________________________________ SETTERS ___________________________________________*/

void Server::setRunning(bool state) { serverState = state; }
void Server::setServerSocket(int socket) { serverSocketFd = socket; }

/*___________________________________________ GETTERS ___________________________________________*/

bool Server::shouldReset() { return reset; }
bool Server::isRunning() { return serverState; }
std::string Server::getHostMask() { return hostmask; }
int Server::getServerSocket() { return serverSocketFd; }

User &Server::getUser(std::string userNickName) {
	std::map<int, User>::iterator usersIt = users.begin();

	for (; usersIt != users.end(); usersIt++) {
		if (usersIt->second.getNickName().compare(userNickName) == 0) return usersIt->second;
	}
	throw CustomException("getUser() -> NoSuchUser");

	return usersIt->second;
}
