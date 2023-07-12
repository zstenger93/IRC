#include "../../includes/Channel.hpp"

#include "../../includes/Commands.hpp"
#include "../../includes/ReplyCodes.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

Channel::Channel(std::string name) : channelName(name), userCount(0), userLimit(5) {
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
	std::map<std::string, bool>::iterator channelIt = channels.find(name);
	if (channelIt == channels.end()) {
		send_message_to_server(user.getUserFd(), 4, RICK, "PRIVMSG", user.getNickName().c_str(),
							   ":", JOINEDCHANNEL);
		channels.insert(std::make_pair(name, true));
		// send_message_to_server(user.getUserFd(), 3, user.getNickName().c_str(), name.c_str(),
		// 					   JOINEDCHANNEL);
		return;
	}
	std::cout << "HAVE THE USER ALREADY\n";
	send_message_to_server(user.getUserFd(), 3, user.getNickName(), JOIN, name.c_str());
	// send_message_to_server(user.getUserFd(), 5, ERR_USERONCHANNEL, user.getNickName().c_str(),
	// 					   name.c_str(), COL, ALREADYJOINED);
}

void Channel::addMode(std::string mode, bool value) { modes.insert(std::make_pair(mode, value)); }

bool Channel::checkMode(std::string mode) {
	return (modes.find(mode) == modes.end() ? false : modes.find(mode)->second);
}

/*___________________________________________ SETTERS ___________________________________________*/

void Channel::setChannelTopic(std::string newChannelTopic) { channelTopic = newChannelTopic; }
void Channel::changeUserCount(int count) { userCount = count; }

/*___________________________________________ GETTERS ___________________________________________*/

int Channel::getUserCount() { return userCount; }
std::string Channel::getChannelName() { return channelName; }
const std::string Channel::getChannelTopic() { return channelTopic; }
const std::map<std::string, bool>& Channel::getChannelModes() { return modes; }