#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/poll.h>

#include <exception>

#include "Client.hpp"
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
	bool passwordCheck(std::string psswrd);
	void setPassword(std::string psswrd);
	void setRunning(bool state);
	int getPort();
	int getServerSocket();
	void setServerSocket(int socket);
	std::string getPassword();

	void setAdminDetails();
	void createAdmin();
	void setAdmin(std::string adminName);
	void setAdminPass(std::string adminPass);
	std::string getAdmin();
	std::string getAdminPass();
	const Client &getClient(int clientNumb) const;
	void acceptConnection();
	void setupPoll();

	// class WrongArgCountException : public std::exception {
	//    public:
	// 	const char *what() const throw();
	// };

	// class WrongPortException : public std::exception {
	//    public:
	// 	const char *what() const throw();
	// };

	// class LongPassException : public std::exception {
	//    public:
	// 	const char *what() const throw();
	// };

	// class WrongPassException : public std::exception {
	//    public:
	// 	const char *what() const throw();
	// };

	class CustomException : public std::exception {
	   private:
		std::string message;

	   public:
		CustomException(const std::string &word);
		const char *what() const throw();
		virtual ~CustomException() throw();
	};

   private:
	// sockets
	bool serverState;
	int serverSocketFd;
	pollfd userPoll[MAX_CONNECTIONS];
	int onlineUserCount;
	// setup user info
	std::string operator_name;
	std::string operator_password;
	// Setup info
	std::string password;
	int port;
};

#endif