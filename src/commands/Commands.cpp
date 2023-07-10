#include "../../includes/Commands.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void User::sendMessage() {}

void User::joinChannel(std::string name) {
	std::map<std::string, bool>::iterator it = channels.find(name);
	// std::cout << it->second << std::endl;
	if (it == channels.end()) {
		channels.insert(std::make_pair(name, true));
		std::map<std::string, bool>::iterator it = channels.find(name);
		// std::cout << it->second << std::endl;
		return;
	}
	// return you are already on the channel
}

void User::leaveChannel() {}

void User::kickUser() {}

void User::inviteUser() {}

void User::quitServer() {}

void User::setNick(std::map<int, User>::iterator it, std::string newNickname) {
	// needs channel name after username
	if (newNickname.length() != 0) {
		nickName = "\0037" + newNickname + "\0030";
		send_message_to_server(it->first, 2, nickName.c_str(), NICKCHANGED);
	} else {
		send_message_to_server(it->first, 2, nickName.c_str(), NICKEMPTYSTR);
	}
}

void User::listChannels() { std::cout << "test" << std::endl; }

void User::modeUser() {}

void User::modeOper() {}

void User::topicUser() {}

void User::topicOper() {}

void User::emptyFunction() {}

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