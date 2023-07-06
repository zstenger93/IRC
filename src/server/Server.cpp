#include "../../includes/Server.hpp"

#include <exception>

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

//-----------------------------READ ADMIN DETAILS FROM CONFIG FILE ------------------------------

static std::string extractWord(const std::string &line) {
	std::string::size_type pos = line.find('=');
	if (pos != std::string::npos) {
		std::string word = line.substr(pos + 1);
		std::string::size_type firstNonSpace = word.find_first_not_of(" \t"),
							   lastNonSpace = word.find_last_not_of(" \t");
		if (firstNonSpace != std::string::npos && lastNonSpace != std::string::npos) {
			return word.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
		}
	}
	return "";	// maybe throw an error as no username found or whatever
}

static std::string base64Decode(const std::string &encodedData) {
	const std::string base64Chars = BASE64;
	std::string decodedData;
	std::string::size_type encodedLength = encodedData.length(), i = 0;
	unsigned char buffer[3], temp[4];
	while (i < encodedLength) {
		for (int j = 0; j < 4; ++j) {
			temp[j] = static_cast<unsigned char>(base64Chars.find(encodedData[i++]));
		}
		buffer[0] = (temp[0] << 2) | (temp[1] >> 4);
		decodedData += buffer[0];
		if (temp[2] < 64) {
			buffer[1] = (temp[1] << 4) | (temp[2] >> 2);
			decodedData += buffer[1];
		}
		if (temp[3] < 64) {
			buffer[2] = ((temp[2] & 0x03) << 6) | temp[3];
			decodedData += buffer[2];
		}
	}
	return decodedData;
}

void Server::setAdminDetails() {
	std::string line, name;
	std::ifstream file("conf/irc.conf");
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		if (line.find("oper_username") != std::string::npos) {
			setAdmin(extractWord(line));
		}
		if (line.find("oper_password") != std::string::npos) {
			setAdminPass(base64Decode(extractWord(line)));
		}
	}
	std::cout << "operator: " << operator_name << std::endl;
	std::cout << "password: " << operator_password << std::endl;
}

/*___________________________________________ SETTERS ___________________________________________*/

// SERVER
void Server::setServerSocket(int socket) { serverSocketFd = socket; }
void Server::setRunning(bool state) { serverState = state; }
// ADMIN
void Server::setAdmin(std::string adminName) { operator_name = adminName; }
void Server::setAdminPass(std::string adminPass) { operator_password = adminPass; }

/*___________________________________________ GETTERS ___________________________________________*/

// SERVER
int Server::getServerSocket() { return serverSocketFd; }
bool Server::shouldReset() { return reset; }
// ADMIN
std::string Server::getAdmin() { return operator_name; }
std::string Server::getAdminPass() { return operator_password; }
