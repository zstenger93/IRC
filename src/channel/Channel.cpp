#include "../../includes/Channel.hpp"

#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

Channel::Channel(std::string name) : channelName(name) {
	std::string modesArray[5] = {"i", "t", "k", "o", "l"};

	for (int i = 0; i < 5; i++) modes.insert(std::make_pair(modesArray[i], false));
}
Channel::~Channel() {}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void Server::createChannel(User& user, std::string name) {
	channels.insert(std::make_pair(name, Channel(name)));
}

void User::joinChannel(User& user, std::string name) {
	std::map<std::string, bool>::iterator it = channels.find(name);
	if (it == channels.end()) {
		channels.insert(std::make_pair(name, true));
		send_message_to_server(user.getUserFd(), 5, name.c_str(), ":", user.getNickName().c_str(),
							   name.c_str(), JOINEDCHANNEL);
		return;
	}
	send_message_to_server(user.getUserFd(), 3, user.getNickName().c_str(), name.c_str(),
						   ALREADYJOINED);
}

void Channel::addMode(std::string mode, bool value) { modes.insert(std::make_pair(mode, value)); }

bool Channel::checkMode(std::string mode) {
	return (modes.find(mode) == modes.end() ? false : modes.find(mode)->second);
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/

std::string Channel::getChannelName() { return channelName; }

const std::map<std::string, bool>& Channel::getChannelModes() { return modes; }