#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Defines.hpp"

class Server {
   public:
	Server(int argc, char **argv);
	~Server();
	// INPUT PARSING
	void inputParser(int argc, char **argv);
	bool isRunning();
	int getPort();
	void setPort(int portNum);
	std::string getPassword();
	void setPassword(std::string psswrd);
	bool passwordCheck(std::string psswrd);
	void removeUser(int pollId);

	void run();
	bool shouldReset();
	int setup();
	int getServerSocket();
	void setRunning(bool state);
	void setServerSocket(int socket);
	const Client &getClient(int clientNumb) const;

	void setupPoll();  // do we need this?
	void acceptConnection();
	int processInput(int pollId);

	// CONNECTION LIMITS
	void setConnectionLimits();
	void setMaxLimit(int maxLimit);
	void setAllowedLimit(int allowedLimit);
	int getMaxlimit();
	int getAllowedLimit();

	// ADMIN
	void createAdmin();	 // needs to be written
	void setAdminDetails();
	std::string getAdmin();
	std::string getAdminPass();
	void setAdmin(std::string adminName);
	void setAdminPass(std::string adminPass);

	std::string extractWord(const std::string &line);

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
	pollfd userPoll[CONNECTIONS];
	int onlineUserCount;
	// Server info
	std::string password;
	int port;

	bool reset;	 // first loop. always true

	// Connection limits
	int max_connections;
	int allowed_connections;

	// Server admin info
	std::string operator_name;
	std::string operator_password;
};

#endif