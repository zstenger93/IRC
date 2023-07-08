#ifndef USER_HPP
#define USER_HPP

#include "Defines.hpp"

class Server;

class User {
   public:
	User();
	User(int inputFd);
	~User();
	void setUserName(std::string uName);
	void setNickName(std::string nName);
	void connectUser(bool isConnected);
	bool isConnected();
	std::string getUserName();
	std::string getNickName();
	int getUserFd();

   private:
	std::string userName;
	std::string nickName;
	bool connected;
	int userFd;

};

#endif