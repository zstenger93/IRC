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
	std::map<std::string, bool>::iterator it = channels.find(name);
	if (it == channels.end()) {
		send_message_to_server(user.getUserFd(), 4, user.getNickName(), JOIN, name.c_str(), COL,
							   name.c_str());
		channels.insert(std::make_pair(name, true));
		return;
	} else {
		send_message_to_server(user.getUserFd(), 2, RICK, ERR_USERONCHANNEL, name.c_str());
	}
}

void Channel::addMode(std::string mode, bool value) { modes.insert(std::make_pair(mode, value)); }

bool Channel::checkMode(std::string mode) {
	return (modes.find(mode) == modes.end() ? false : modes.find(mode)->second);
}

bool Channel::isPasswordCorrect(std::string inputPassword) {
	if (channelPassword.compare(inputPassword) == 0) return true;
	return false;
}

/*___________________________________________ SETTERS ___________________________________________*/

void Channel::setChannelTopic(std::string newChannelTopic) { channelTopic = newChannelTopic; }
void Channel::changeUserCount(int count) { userCount = count; }

/*___________________________________________ GETTERS ___________________________________________*/

int Channel::getUserCount() { return userCount; }
std::string Channel::getChannelName() { return channelName; }
const std::string Channel::getChannelTopic() { return channelTopic; }
const std::map<std::string, bool>& Channel::getChannelModes() { return modes; }
int Channel::getUserLimit() { return userLimit; }