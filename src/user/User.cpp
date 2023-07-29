#include "../../includes/User.hpp"
#include "../../includes/Commands.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

User::User() : userName(""), nickName(""), connected(false) {}
User::User(int inputFd, std::string name)
	: userName(name), nickName(name), connected(false), userFd(inputFd) {}
User::~User() {}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/
/*___________________________________________ SETTERS ___________________________________________*/

void User::connectUser(bool isConnected) { connected = isConnected; }
void User::setUserName(std::string uName) { userName = uName; }
void User::setNickName(std::string nName) { nickName = nName; }

void User::setOperatorPrivilage(std::string channelName, bool setTo) {
	std::map<std::string, bool>::iterator channelIt = channels.find(channelName);
	channelIt->second = setTo;
}

void User::userRemovedFromServerMsg(User &user, User &usersIt) {
	for (std::map<std::string, bool>::iterator channelsIt = channels.begin();
		 channelsIt != channels.end(); channelsIt++) {
		if (user.isInChannel(channelsIt->first) && usersIt.isInChannel(channelsIt->first))
			if (usersIt.getNickName().compare(user.getNickName()) != 0)
				send_message_to_server(usersIt.getUserFd(), 4, user.getNickName().c_str(), P,
									   channelsIt->first.c_str(), COL, RMVDBYMARV);
	}
}

bool User::isInvitedToChannel(std::string channelName) {
	for (unsigned long i = 0; i < invitedChannels.size(); i++)
		if (invitedChannels[i].compare(channelName) == 0) return true;
	return false;
}

/*___________________________________________ GETTERS ___________________________________________*/

int User::getUserFd() { return userFd; }
bool User::isConnected() { return connected; }
std::string User::getUserName() { return userName; }
std::string User::getNickName() { return nickName; }

bool User::isOperatorInChannel(std::string channelName) {
	std::map<std::string, bool>::iterator modeIt = channels.find(channelName);
	if (modeIt != channels.end()) return modeIt->second;
	return false;
}
