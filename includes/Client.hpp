#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Defines.hpp"

class Client {
   public:
	int getClientSocket();
	void setClientSocket(int values);
	struct sockaddr_in getSockAddr();
	void setSocAddr(struct sockaddr_in values);

   protected:
   private:
	int clientSocket;
	struct sockaddr_in clientAdress;
};

#endif