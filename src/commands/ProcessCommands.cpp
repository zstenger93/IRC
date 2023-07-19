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
	char buffer[512];
	int buffer_len, stringLength = 0;
	memset(buffer, 0, 512);
	buffer_len = recv(userPoll[pollId].fd, buffer, 512, USERDISCONECTED);
	message = buffer;
	stringLength += buffer_len;

	std::map<int, User>::iterator userIt = users.find(userPoll[pollId].fd);
	if (userIt->second.isConnected() == false) {
		authenticate(message, userIt);
	}
	if (userIt->second.isConnected() == false) {
		return 1;
	}
	if (buffer_len == USERDISCONECTED) {
		removeUser(pollId);
		onlineUserCount--;
		return USERDISCONECTED;
	} else if (buffer_len == -1)
		throw CustomException(F_FAILED_MESSAGE);
	while (message.find("\n") == std::string::npos) {
		memset(buffer, 0, 512);
		buffer_len = recv(userPoll[pollId].fd, buffer, 512, MSG_DONTWAIT);
		stringLength += buffer_len;
		message += buffer;
		if (message.find("\n") != std::string::npos) {
			message = message.substr(0, message.length() - 1);
			message = message + "\r\n";
		}
	}
	commandParser(userIt->second, message, userIt->first, pollId);
	return 1;
}

void Server::commandParser(User& user, std::string msg, int fd, int pollId) {
	int caseId = 0, pos = msg.find("DCC");
	std::string command = getCommand(msg);
	std::string commands[19] = {"NOTICE", "PRIVMSG", "JOIN", "PART",  "KICK", "INVITE", "QUIT",
								"NICK",	  "LIST",	 "MODE", "TOPIC", "CAP",  "PASS",	"ADMIN",
								"WHO",	  "PING",	 "MOTD", "WHOIS", "BOT"};
	for (int i = 0; i < 19; i++) {
		if (command.compare(commands[i]) == 0) {
			caseId = i;
			break;
		}
	}
	std::cout << RECIEVED << command << FULLMSG << msg << EXEC << caseId << std::endl;
	switch (caseId) {
		case 0:
			break;
		case 1:
			if (pos == std::string::npos)
				sendMessage(msg, users, fd, pollId, userPoll, onlineUserCount);
			else
				sendFiles(users, msg, fd);
			break;
		case 2:
			if (Parser::getWordCount(msg) == 2)
				handleJoin(msg, user, extractArgument(1, msg, 2));
			else
				handleJoin(msg, user, extractArgument(1, msg, 3));
			break;
		case 3:
			user.leaveChannel(users, user, extractArgument(1, msg, -1), 0);
			break;
		case 4:
			user.kickUser(users, extractArgument(2, msg, 3), extractArgument(1, msg, 3), fd);
			break;
		case 5:
			user.inviteUser(users, extractArgument(2, msg, 3), extractArgument(1, msg, 3), fd);
			break;
		case 6:
			sendUserRemoved(user);
			removeUser(pollId);
			break;
		case 7:
			setNick(user, extractArgument(1, msg, 2));
			break;
		case 8:
			listChannels(user.getNickName());
			break;
		case 9:
			mode(msg, fd);
			break;
		case 10:
			channelTopic(msg, extractArgument(1, msg, -1), fd);
			break;
		case 11:
			// CAP
			break;
		case 12:
			// to automaticly join to general after providing the right /PASS
			handleJoin(msg, user, "#General");
			break;
		case 13:
			if (Parser::getWordCount(msg) == 3) shutdown(msg);
			break;
		case 14:
			who(fd, msg);
			break;
		case 15:
			user.ping(msg, fd);
			break;
		case 16:
			motd(fd, extractArgument(2, msg, 3));
			break;
		case 17:
			whois(fd, msg);
			break;
		case 18:
			bot.runAi(fd, msg, user, users, pollId, userPoll, onlineUserCount);
			break;
		default:
			send_message_to_server(fd, 1, RICK, COMMAND_NOT_FOUND);
			break;
	}
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/