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

	std::map<std::string, bool> channels;

	int getUserFd();
	bool isConnected();
	std::string getUserName();
	std::string getNickName();
	void connectUser(bool isConnected);
	void setUserName(std::string uName);
	void setNickName(std::string nName);
	bool isInChannel(std::string channelName);
	void ping(std::string message, int userFd);
	bool isOperatorInChannel(std::string channelName);
	void joinChannel(User& user, std::string name, int op);
	void leaveChannel(std::map<int, User>& users, User& user, std::string channelName, int mode);
	void kickUser(std::map<int, User>& users, std::string kickUserName, std::string channelName,
				  int senderFd);
	void inviteUser(std::map<int, User>& users, std::string addUserName, std::string channelName,
					int senderFd);

   private:
	std::string userName;
	std::string nickName;
	bool connected;
	int userFd;
};

#endif