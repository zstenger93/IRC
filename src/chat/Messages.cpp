#include "../../includes/Server.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void send_message_to_server(int fd, int count, std::string prefix, ...) {
	std::string message = ":" + prefix + " ";
	int i = 0;
	va_list arguments;
	va_start(arguments, prefix);

	while (i < count) {
		message = message + va_arg(arguments, const char*);
		i++;
		if (message.back() == ':') continue;
		if (i != count) message = message + " ";
	}
	va_end(arguments);
	message += "\r\n";
	std::cout << SENT << message << std::endl;
	send(fd, message.c_str(), message.length(), 0);
}

std::vector<std::string> splitString(const std::string& message) {
	std::vector<std::string> tokens;
	std::istringstream iss(message);
	std::string word;

	while (iss.good()) {
		iss >> word;
		if (!word.empty()) tokens.push_back(word);
	}
	return tokens;
}

std::string extractArgument(int specificArg, const std::string& message, int aCount) {
	std::vector<std::string> tokens = splitString(message);

	if (tokens.size() - 1 > aCount && aCount != -1) return std::string();
	if (specificArg >= 0 && specificArg < static_cast<int>(tokens.size()))
		return tokens[specificArg];
	return std::string();
}

std::string extractMessage(std::string message) {
	std::string parsedMessage = message.substr(message.find_first_of(':') + 1);
	return parsedMessage;
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/