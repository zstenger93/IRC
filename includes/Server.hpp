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
	bool passwordCheck(std::string psswrd);
	void inputParser(int argc, char **argv);

	// USER
	void addUser(int userFd);
	std::map<int, User> users;
	void removeUser(int pollId);
	void sendUserRemoved(User &user);
	bool getPass(std::string &msg);
	void authenticate(std::string message, std::map<int, User>::iterator it);
	bool userExists(std::string userName);
	User &getUser(std::string userNickName);

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

	// PROCESS COMMANDS
	int processCommands(int pollId);
	void commandParser(User &user, std::string msg, int fd, int pollId);

	// COMMAND HANDLING
	void listChannels(std::string userName);
	void who(int userFd, std::string message);
	void mode(std::string message, int userFd);
	void whois(int userFd, std::string message);
	void motd(int userFd, std::string channelName);
	void setNick(User &user, std::string newNickname, std::string msg);
	void sendFiles(std::map<int, User> users, std::string message, int userFd);
	void channelTopic(std::string message, std::string channelName, int userFd);
	void sendMessage(std::string message, std::map<int, User> &users, int userFd, int pollId,
					 pollfd uPoll[CONNECTIONS], int uCount);

	// COMMAND UTILS
	bool isModeValid(std::string mode);
	void addModeO(User &user, std::string msg);
	std::string getCommand(std::string message);
	bool checkIfCanBeExecuted(std::string channelName, int senderFd);
	void loopTroughtTheUsersInChan(std::string chanName, int senderFd, int mode,
								   std::string message, User &user);
	void addMode(Channel &channel, User &user, std::string mode, std::string msg);
	void removeMode(Channel &channel, User &user, std::string mode, std::string msg);

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