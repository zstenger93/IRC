#ifndef USER_HPP
#define USER_HPP

#include "Defines.hpp"

class Channel;
class Server;

class User {
   public:
	User();
	User(int inputFd, std::string name);
	~User();
	int getUserFd();
	bool isConnected();
	std::string getUserName();
	std::string getNickName();
	void connectUser(bool isConnected);
	void setUserName(std::string uName);
	void setNickName(std::string nName);

	// CHANNELS
	std::map<std::string, bool> channels;

	// COMMAND TO EXECUTE
	void ping(std::string message, int userFd);
	void joinChannel(User& user, std::string name, int op);
	void leaveChannel(std::map<int, User>& users, User& user, std::string channelName, int mode);
	void kickUser(std::map<int, User>& users, std::string kickUserName, std::string channelName,
				  int senderFd);
	void inviteUser(std::map<int, User>& users, std::string addUserName, std::string channelName,
					int senderFd);

	// HELPER FUNCTIONS
	bool isInChannel(std::string channelName);
	bool isOperatorInChannel(std::string channelName);

   private:
	std::string userName;
	std::string nickName;
	bool connected;
	int userFd;
	std::string activeChannels[2];	// save the channels user is joined to?
};

#endif