#ifndef USER_HPP
#define USER_HPP

#include "Defines.hpp"

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

	//COMMAND TO EXECUTE
	void message();
	void joinChannel();
	void leaveChannel();
	void kick();
	void invite();
	void quitServer();
	void nick();
	void listChannels();
	void modeUser();
	void modeOper();
	void topicUser();
	void topicOper();
	void emptyFunction();

   private:
	std::string userName;
	std::string nickName;
	bool connected;
	int userFd;

};

#endif