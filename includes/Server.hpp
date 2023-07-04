#ifndef SERVER_HPP
#define SERVER_HPP

#include "Defines.hpp"

class Server {
   public:
	// Server();
	// ~Server();
	void inputParser(int argc, char **Argv);
	bool isRunning();
	void run();
	int setup();
	void setPort(int portNum);
	void setPassword(std::string psswrd);
	void setRunning(bool state);
	int getPort();
	int getServerSocket();
	void setServerSocket(int socket);
	std::string getPassword();

	class WrongArgCountException : public std::exception {
	   public:
		const char *what() const throw();
	};

	class WrongPortException : public std::exception {
	   public:
		const char *what() const throw();
	};

   protected:
   private:
	bool serverState;
	std::string password;
	int port;
	int serverSocketFd;
};

#endif