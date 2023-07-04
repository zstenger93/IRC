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
	void setPort(int portNum);
	bool passwordCheck(std::string psswrd);
	void setPassword(std::string psswrd);
	int getPort();
	std::string getPassword();

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

   protected:
   private:
	bool serverState;
	std::string password;
	int port;
};

#endif