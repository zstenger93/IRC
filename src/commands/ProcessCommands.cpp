#include "../../includes/Channel.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Parser.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/
/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

int Server::processCommands(int pollId) {
	std::string message = "";
	char buffer[1024];
	int buffer_len;
	memset(buffer, 0, 1024);
	buffer_len = recv(userPoll[pollId].fd, buffer, 1024, MSG_DONTWAIT);
	message = buffer;
	std::map<int, User>::iterator userIt = users.find(userPoll[pollId].fd);

	if (userIt->second.isConnected() == false) authenticate(message, userIt);
	if (userIt->second.isConnected() == false) return 1;
	if (buffer_len == USERDISCONECTED)
		return removeUser(pollId), USERDISCONECTED;
	else if (buffer_len == -1)
		throw CustomException(F_FAILED_MESSAGE);
	getCommand(userIt->second, message, pollId, buffer_len, buffer);
	return 1;
}

void Server::commandParser(User& user, std::string msg, int fd, int pollId) {
	int caseId = 0, pos = msg.find("DCC");
	std::string command = getCommand(msg);
	std::string commands[20] = {"NOTICE", "PRIVMSG", "JOIN", "PART",  "KICK", "INVITE", "QUIT",
								"NICK",	  "LIST",	 "MODE", "TOPIC", "CAP",  "PASS",	"ADMIN",
								"WHO",	  "PING",	 "MOTD", "WHOIS", "BOT",  "USER"};
	for (int i = 0; i < 20; i++) {
		if (command.compare(commands[i]) == 0) {
			caseId = i;
			break;
		}
	}
	std::cout << RECIEVED << command << FULLMSG << msg << EXEC << caseId << std::endl;
	switch (caseId) {
		case NOTHING:
			break;
		case MESSAGE:
			if (pos == std::string::npos)
				sendMessage(msg, users, fd, pollId, userPoll, onlineUserCount);
			else
				sendFiles(users, msg, fd);
			break;
		case JOIN_CHANNEL:
			if (Parser::getWordCount(msg) == 2)
				handleJoin(msg, user, extractArgument(1, msg, 2));
			else
				handleJoin(msg, user, extractArgument(1, msg, 3));
			break;
		case LEAVE_CHANNEL:
			user.leaveChannel(users, user, extractArgument(1, msg, -1), 0);
			break;
		case KICK:
			user.kickUser(users, extractArgument(2, msg, 3), extractArgument(1, msg, 3), fd);
			break;
		case INVITE:
			user.inviteUser(users, extractArgument(2, msg, 3), extractArgument(1, msg, 3), fd);
			break;
		case QUIT:
			sendUserRemoved(user);
			removeUser(pollId);
			break;
		case SET_NICK:
			setNick(user, extractArgument(1, msg, 2), msg);
			break;
		case LIST_CHANNELS:
			listChannels(user.getNickName());
			break;
		case CHANNEL_MODE:
			mode(msg, fd);
			break;
		case CHANNEL_TOPIC:
			channelTopic(msg, extractArgument(1, msg, -1), fd);
			break;
		case CAP_MSG_FROM_CLIENT:
			break;
		case AUTO_JOIN_GENERAL:
			handleJoin(msg, user, "#General");
			break;
		case ADMIN_SHUTDOWN_SERVER:
			if (Parser::getWordCount(msg) == 3)
				shutdown(msg);
			else
				send_message_to_server(user.getUserFd(), 4, RICK, ERR_NEEDMOREPARAMS,
									   user.getNickName().c_str(),
									   extractArgument(0, msg, -1).c_str(), COL);
			break;
		case LIST_EVERYONE_ON_SERVER:
			who(fd, msg);
			break;
		case PING_LATENCY_CHECK:
			user.ping(msg, fd);
			break;
		case MESSAGE_OF_THE_DAY:
			motd(fd, extractArgument(2, msg, 3));
			break;
		case WHO_IS_USER:
			whois(fd, msg);
			break;
		case BOT:
			bot.runAi(fd, msg, user, users, pollId, userPoll, onlineUserCount);
			break;
		case SET_USERNAME:
			setUserName(user, extractArgument(1, msg, 2), msg);
			break;
		default:
			send_message_to_server(fd, 1, RICK, COMMAND_NOT_FOUND);
			break;
	}
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/