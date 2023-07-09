#include "../../includes/Channel.hpp"

#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

Channel::Channel(std::string name) : channelName(name), mode(NORMAL) {}
Channel::~Channel() {}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/
/*___________________________________________ SETTERS ___________________________________________*/

void Channel::setChannelMode(ChannelMode _mode) { mode = _mode; }

/*___________________________________________ GETTERS ___________________________________________*/

ChannelMode Channel::getChannelMode() { return mode; }
std::string Channel::getChannelName() { return channelName; }