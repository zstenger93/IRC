#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/poll.h>

#include "Defines.hpp"

class Client {
   public:
	Client();
	~Client();
	Client(int socket, sockaddr_in adress);
	int getClientSocket() const;
	void setClientSocket(int values);
	struct sockaddr_in getSockAddr();
	void setSocAddr(struct sockaddr_in values);
	pollfd getClientPoll();

   protected:
   private:
	std::string nick;
	std::string name;
	int clientSocket;
	struct sockaddr_in clientAdress;
	pollfd pollFd;
};

#endif