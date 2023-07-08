#include "../../includes/Defines.hpp"

void send_message_to_server(int fd, int count, ...) {
	std::string message = ":RickRoll.de ";
	int i = 0;
	va_list arguments;
	va_start(arguments, count);
	while (i < count) {
		message = message + va_arg(arguments, const char *);
		i++;
		if (i != count) message = message + " ";
	}
	va_end(arguments);
	message += "\r\n";
    send(fd, message.c_str(), message.length(), 0);
}