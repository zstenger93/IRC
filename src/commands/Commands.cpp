#include "../../includes/Commands.hpp"

#include <cstring>
#include <string>
#include <utility>

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void Server::handleJoin(User& user, std::string name) {
	if (name.length() == 0) std::cout << "Cannot pass empty string as argument" << std::endl;
	std::map<std::string, Channel>::iterator it = channels.find(name);
	if (it == channels.end()) {
		createChannel(name);
		send_message_to_server(user.getUserFd(), 3, user.getNickName().c_str(), name.c_str(),
							   CREATEDCHANNEL);
		user.joinChannel(user, name);
		send_message_to_server(user.getUserFd(), 3, user.getNickName().c_str(), name.c_str(),
							   JOINEDCHANNEL);
		return;
	}
	// if channel is invite only
	user.joinChannel(user, name);
}

void User::sendMessage() {
	// where to? channel or user?
	// hash at start of 2nd arg -> channel
	// send to channel
	// else error
	// else send to user
	// else error
}

void User::leaveChannel(std::string channelName) {
	std::map<std::string, bool>::iterator channel = channels.find(channelName);
	if (channel == channels.end()) {
		// RETURN NO SUCH CHANNEL ERROR
	}
	channels.erase(channel);
	// find the channel and disconnect the user from it
	// else error
}

bool User::isInChannel(std::string channelName) {
	return (channels.find(channelName) == channels.end());
}

void User::kickUser(std::map<int, User>& users, std::string kickUserName,
					std::string channelName) {	// users
	std::map<std::string, bool>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		// RETURN NO SUCH CHANNEL ERROR
	}
	if (channelIt->second == false) {
		// THE USER IS NOT OPERATOR ERROR
	}

	std::map<int, User>::iterator userIt;
	for (userIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->second.getUserName().compare(kickUserName) == 0) break;
	}
	if (userIt == users.end()) {
		// NO SUCH USER ERROR
	}

	if (userIt->second.isInChannel(channelName) == false) {
		// KICKUSER IS NOT IN THE CHANNEL
	}

	userIt->second.leaveChannel(channelName);
	// SEND TO CHANNEL USER KICKED KICKEDUSER FROM THE CHANNEL
}

void User::inviteUser(std::map<int, User>& users, std::string addUserName,
					  std::string channelName) {  // users
	std::map<std::string, bool>::iterator channelIt = channels.find(channelName);
	if (channelIt == channels.end()) {
		// RETURN NO SUCH CHANNEL ERROR
	}
	if (channelIt->second == false) {
		// THE USER IS NOT OPERATOR ERROR
	}

	std::map<int, User>::iterator userIt;
	for (userIt = users.begin(); userIt != users.end(); userIt++) {
		if (userIt->second.getUserName().compare(addUserName) == 0) break;
	}
	if (userIt == users.end()) {
		// NO SUCH USER ERROR
	}

	if (userIt->second.isInChannel(channelName) == true) {
		// USER ALREADY IN THE CHANNEL
	}

	userIt->second.channels.insert(std::make_pair(channelName, false));
	// USER HAS BEEN INVITED AND ADDED TO THE CHANNEL
}
void User::quitServer() {
	
	// disconnect the user myb? idk.

}

// need to add parsing, parameter should be admin password
void Server::shutdown() {
	// this is only server admin function
	// shut down the server
	serverState = false;  // not sure if this is needed
	reset = false;
}

void User::setNick(std::map<int, User>::iterator it, std::string newNickname) {
	// needs channel name after username
	if (newNickname.length() != 0) {
		// check if someone is already using this nick ??
		nickName = "\0037" + newNickname + "\0030";
		send_message_to_server(it->first, 2, nickName.c_str(), NICKCHANGED);
		// do we send msg to other users that one changed it's nickname?
	} else {
		send_message_to_server(it->first, 2, nickName.c_str(), NICKEMPTYSTR);
	}
}

void User::listChannels() {
	// list the available channels? or what you have joined to?
}

void User::modeUser() {
	// show the mode of the channel. i guess it should take the channel name as arg
}

void User::modeOper() {
	// change the mode of the channel
}

void User::topicUser() {
	// show the topic of the channel
}

void User::topicOper() {
	// change the topic of the channel
}

void User::motd(User& user) {
	// std::ifstream file("conf/motd.txt");
	// std::string line;
	// send_message_to_server(user.getUserFd(), 1, MOTD);
	// while (std::getline(file, line)) {
	// 	std::cout << line << std::endl;
	// 	send_message_to_server(user.getUserFd(), 1, line.c_str());
	// }
	// file.close();
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/