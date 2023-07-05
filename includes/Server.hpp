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
	bool passwordCheck(std::string psswrd);
	void setPassword(std::string psswrd);
	void setRunning(bool state);
	int getPort();
	int getServerSocket();
	void setServerSocket(int socket);
	std::string getPassword();

	void createAdmin();
	void setAdmin(std::string adminName);
	void setAdminPass(std::string adminPass);
	std::string getAdmin();
	std::string getAdminPass();


	class WrongArgCountException : public std::exception {
	   public:
		const char *what() const throw();
	};

	class WrongPortException : public std::exception {
	   public:
		const char *what() const throw();
	};

	class LongPassException : public std::exception {
	   public:
		const char *what() const throw();
	};

	class WrongPassException : public std::exception {
	   public:
		const char *what() const throw();
	};

   private:
	bool serverState;
	std::string password;
	int port;
	int serverSocketFd;
	std::string operator_name;
	std::string operator_password;
};

#endif