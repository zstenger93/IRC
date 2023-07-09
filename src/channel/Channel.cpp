#include "../../includes/Channel.hpp"

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

Channel::Channel(std::string name) : channelName(name), mode(NORMAL) {}
Channel::~Channel() {}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void Server::createChannel(std::string name) {
	channels.insert(std::make_pair(name, Channel(name)));
}

/*___________________________________________ SETTERS ___________________________________________*/

void Channel::setChannelMode(ChannelMode _mode) { mode = _mode; }

/*___________________________________________ GETTERS ___________________________________________*/

ChannelMode Channel::getChannelMode() { return mode; }
std::string Channel::getChannelName() { return channelName; }