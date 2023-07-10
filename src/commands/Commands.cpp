#include "../../includes/Commands.hpp"

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
	user.joinChannel(user, name);
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::sendMessage() {
	// where to? channel or user?
	// hash at start of 2nd arg -> channel
	// send to channel
	// else error
	// else send to user
	// else error
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::leaveChannel() {
	// find the channel and disconnect the user from it
	// else error
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::kickUser() {
	// find the user and kick it from the channel
	// else error
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::inviteUser() {
	// dunno howtf this supposed to work
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::quitServer() {
	// disconnect the user myb? idk.
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
// need to add parsing, parameter should be admin password
void Server::shutdown() {
	// this is only server admin function
	// shut down the server
	serverState = false;  // not sure if this is needed
	reset = false;
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
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

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::listChannels() {
	// list the available channels? or what you have joined to?
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::modeUser() {
	// show the mode of the channel. i guess it should take the channel name as arg
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::modeOper() {
	// change the mode of the channel
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::topicUser() {
	// show the topic of the channel
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
void User::topicOper() {
	// change the topic of the channel
}

// tf it is doing:
// code:
// must have:
// optional:
// error:
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