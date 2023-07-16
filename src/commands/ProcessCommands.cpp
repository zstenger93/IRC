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

	std::map<int, User>::iterator it = users.find(userPoll[pollId].fd);
	if (it->second.isConnected() == false) {
		authenticate(message, it);
	}
	if (it->second.isConnected() == false) {
		return 1;
	}
	if (buffer_len == USERDISCONECTED) {
		removeUser(pollId);
		onlineUserCount--;
		return USERDISCONECTED;
	} else if (buffer_len == -1)
		throw CustomException(F_FAILED_MESSAGE);
	while (message.find("\r\n") == NOT_FOUND && buffer_len != 0) {
		memset(buffer, 0, 512);
		buffer_len = recv(userPoll[pollId].fd, buffer, 512, 0);
		stringLength += buffer_len;
		message += buffer;
	}
	commandParser(it, message, it->first, pollId);
	return 1;
}

void Server::commandParser(std::map<int, User>::iterator& user, std::string message, int fd,
						   int pollId) {
	int caseId = 0;
	std::string command = getCommand(message);
	std::string commands[19] = {"NOTICE", "PRIVMSG", "JOIN", "PART",  "KICK", "INVITE", "QUIT",
								"NICK",	  "LIST",	 "MODE", "TOPIC", "CAP",  "PASS",	"ADMIN",
								"WHO",	  "PING",	 "MOTD", "WHOIS", "BOT"};
	for (int i = 0; i < 19; i++) {
		if (command.compare(commands[i]) == 0) {
			caseId = i;
			break;
		}
	}
	std::cout << "Command recived: " << command << " Full message: " << message
			  << "Option choosen: " << caseId << std::endl;
	switch (caseId) {
		case 0:
			break;
		case 1:
			sendMessage(message, users, fd, pollId, userPoll, onlineUserCount);
			break;
		case 2:
			if (Parser::getWordCount(message) == 2)
				handleJoin(message, user->second, extractArgument(1, message, 2));
			else
				handleJoin(message, user->second, extractArgument(1, message, 3));
			break;
		case 3:
			user->second.leaveChannel(users, user->second, extractArgument(1, message, -1), 0);
			break;
		case 4:
			user->second.kickUser(users, extractArgument(2, message, 3),
								  extractArgument(1, message, 3), fd);
			break;
		case 5:
			user->second.inviteUser(users, extractArgument(2, message, 3),
									extractArgument(1, message, 3), fd);
			break;
		case 6:
			removeUser(pollId);	 // quitServer();
			break;
		case 7:
			setNick(user, extractArgument(1, message, 2));
			break;
		case 8:
			listChannels(user->second.getNickName());
			break;
		case 9:
			mode(message, fd);
			break;
		case 10:
			channelTopic(message, extractArgument(1, message, -1), fd);
			break;
		case 11:
			// CAP
			break;
		case 12:
			// to automaticly join to general after providing the right /PASS
			handleJoin(message, user->second, "#General");
			break;
		case 13:
			if (Parser::getWordCount(message) == 3) shutdown(message);
			break;
		case 14:
			who(fd, message);
			break;
		case 15:
			user->second.ping(message, fd);
			break;
		case 16:
			motd(fd, extractArgument(2, message, 3));
			break;
		case 17:
			whois(fd, message);
			break;
		case 18:
			bot.runAi(fd, message, user->second, users, pollId, userPoll, onlineUserCount);
			break;
		default:
			send_message_to_server(fd, 1, RICK, COMMAND_NOT_FOUND);
			break;
	}
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/