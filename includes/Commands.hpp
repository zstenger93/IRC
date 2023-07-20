#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Defines.hpp"

void send_message_to_server(int fd, int count, std::string prefix, ...);
std::string extractArgument(int specificArg, const std::string& message, int aCount);
std::string extractMessage(std::string message);

#endif