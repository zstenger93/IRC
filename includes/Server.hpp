#ifndef SERVER_HPP
#define SERVER_HPP

#include "Defines.hpp"

class User;
class Commands;
class Client;
class Channel;
class Parser;

class Server {
   public:
	Server();
	Server(int argc, char **argv);
	~Server();
	// INPUT PARSING
	void inputParser(int argc, char **argv);
	bool isRunning();
	int getPort();
	std::string getHostMask();
	void setPort(int portNum);
	std::string getPassword();
	bool passwordCheck(std::string psswrd);
	void removeUser(int pollId);

	// USER
	std::map<int, User> users;
	void addUser(int userFd);
	void authenticate(std::string message, std::map<int, User>::iterator it);
	bool getPass(std::string &msg);

	// CHANNEL
	std::map<std::string, Channel> channels;
	void createChannel(User &user, std::string name);
	void handleJoin(std::string message, User &user, std::string name);
	bool isJoinedWithActiveMode(Channel &channel, User &user, std::string message);

	// MAIN LOOPS
	void run();
	bool shouldReset();
	int setup();
	int getServerSocket();
	void setRunning(bool state);
	void setServerSocket(int socket);
	const Client &getClient(int clientNumb) const;

	void setupPoll();  // do we need this?
	void acceptConnection();

	// COMMAND HANDLING
	int processCommands(int pollId);
	void commandParser(std::map<int, User>::iterator it, std::string message, int fd, int pollId);
	std::string getCommand(std::string message);
	void listChannels(std::string userName);
	void mode(std::string message, int userFd);
	void channelTopic(std::string message, std::string channelName, int userFd);
	void sendMessage(std::string message, std::map<int, User> &users, int userFd);
	void loopTroughtTheUsersInChan(std::string chanName, int senderFd, int mode,
								   std::string message, User &user);
	void executeCommmandsToChannel(std::string channelName, User &user, int mode,
								   std::string message);
	bool checkIfCanBeExecuted(std::string channelName, int senderFd);
	void motd(int userFd, std::string channelName);
	void whois(int userFd, std::string message);
	void who(int userFd, std::string message);

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
	void shutdown(std::string message);

	// PASS
	std::string extractWord(const std::string &line);
	std::string base64Decode(const std::string &encodedData);
	void setServerPassword();
	void setPassword(std::string serverPassword);

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
	std::string hostmask;
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