#ifndef USER_HPP
#define USER_HPP

#include "Server.hpp"

class User {
   public:
	User();
	~User();
	void setUserName(std::string uName);
	void setNickName(std::string nName);
	bool isConnected();
	std::string getUserName();
	std::string getNickName();

   private:
	std::string userName;
	std::string nickName;
	bool connected;
	int userFd;
};

#endif