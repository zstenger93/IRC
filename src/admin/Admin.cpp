#include "../../includes/Server.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

std::string Server::extractWord(const std::string &line) {
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

std::string Server::base64Decode(const std::string &encodedData) {
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

void Server::setAdmin(std::string adminName) { operator_name = adminName; }
void Server::setAdminPass(std::string adminPass) { operator_password = adminPass; }

/*___________________________________________ GETTERS ___________________________________________*/

std::string Server::getAdmin() { return operator_name; }
std::string Server::getAdminPass() { return operator_password; }
