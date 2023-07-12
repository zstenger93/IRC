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
	void setUserName(std::string uName);
	void setNickName(std::string nName);
	void connectUser(bool isConnected);
	bool isConnected();
	std::string getUserName();
	std::string getNickName();
	int getUserFd();

	// CHANNELS
	std::map<std::string, bool> channels;

	// COMMAND TO EXECUTE
	void joinChannel(User& user, std::string name);
	void leaveChannel(std::map<int, User> & users, User& user, std::string channelName);
	void kickUser(std::map<int, User> & users, std::string kickUserName, std::string channelName);
	void inviteUser(std::map<int, User> & users, std::string addUserName, std::string channelName);
	void setNick(std::map<int, User>::iterator it, std::string newNickname);
	void who();
	void oper();
	void ping();

	// HELPER FUNCTIONS
	bool	isInChannel(std::string channelName);
	bool	isOperatorInChannel(std::string channelName);

   private:
	std::string userName;
	std::string nickName;
	bool connected;
	int userFd;
	bool isInvited;
	std::string activeChannels[2];	// save the channels user is joined to?
};

#endif