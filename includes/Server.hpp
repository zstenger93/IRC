#ifndef SERVER_HPP
#define SERVER_HPP

#include "Bot.hpp"
#include "Defines.hpp"

class User;
class Client;
class Channel;
class Parser;

class Server {
   public:
	Server();
	Server(int argc, char **argv);
	~Server();
	// INPUT PARSING
	int getPort();
	bool isRunning();
	std::string getHostMask();
	void setPort(int portNum);
	std::string getPassword();
	void removeUser(int pollId);
	bool passwordCheck(std::string psswrd);
	void inputParser(int argc, char **argv);

	// USER
	void addUser(int userFd);
	std::map<int, User> users;
	bool getPass(std::string &msg);
	void authenticate(std::string message, std::map<int, User>::iterator it);

	// CHANNEL
	std::map<std::string, Channel> channels;
	void createChannel(User &user, std::string name);
	void handleJoin(std::string message, User &user, std::string name);
	int isJoinedWithActiveMode(Channel &channel, User &user, std::string message);

	// MAIN LOOPS
	void run();
	int setup();
	bool shouldReset();
	int getServerSocket();
	void acceptConnection();
	void setRunning(bool state);
	void setServerSocket(int socket);
	const Client &getClient(int clientNumb) const;

	// COMMAND HANDLING
	int processCommands(int pollId);
	void listChannels(std::string userName);
	void who(int userFd, std::string message);
	void mode(std::string message, int userFd);
	void whois(int userFd, std::string message);
	std::string getCommand(std::string message);
	void motd(int userFd, std::string channelName);
	void sendFiles(std::map<int, User> users, std::string message, int userFd);
	void channelTopic(std::string message, std::string channelName, int userFd);
	void loopTroughtTheUsersInChan(std::string chanName, int senderFd, int mode,
								   std::string message, User &user);
	bool checkIfCanBeExecuted(std::string channelName, int senderFd);
	void sendMessage(std::string message, std::map<int, User> &users, int userFd, int pollId,
					 pollfd uPoll[CONNECTIONS], int uCount);
	void commandParser(std::map<int, User>::iterator &it, std::string message, int fd, int pollId);

	// CONNECTION LIMITS
	int getMaxlimit();
	int getAllowedLimit();
	void setConnectionLimits();
	void setMaxLimit(int maxLimit);
	void setAllowedLimit(int allowedLimit);

	// ADMIN
	void setAdminDetails();
	std::string getAdmin();
	std::string getAdminPass();
	void shutdown(std::string message);
	void setAdmin(std::string adminName);
	void setAdminPass(std::string adminPass);
	void setNick(std::map<int, User>::iterator &it, std::string newNickname);

	// PASS
	void setServerPassword();
	void setPassword(std::string serverPassword);
	std::string extractWord(const std::string &line);
	std::string base64Decode(const std::string &encodedData);

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
	int onlineUserCount;
	std::string hostmask;
	pollfd userPoll[CONNECTIONS];
	// Server info
	int port;
	int max_connections;
	std::string password;
	int allowed_connections;
	std::string operator_name;
	std::string operator_password;

	bool reset;
	Marvin bot;
};

#endif