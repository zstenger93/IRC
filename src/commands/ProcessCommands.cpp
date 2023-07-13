#include "../../includes/Channel.hpp"
#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"
#include "../../includes/Parser.hpp"

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

void Server::commandParser(std::map<int, User>::iterator user, std::string message, int fd,
						   int pollId) {
	int caseId = 0;
	std::string command = getCommand(message);
	std::string commands[17] = {"PRIVMSG", "JOIN", "PART", "KICK",	"INVITE", "QUIT",
								"NICK",	   "LIST", "MODE", "TOPIC", "CAP",	  "PASS",
								"ADMIN",   "WHO", "PING",	"MOTD", "WHOIS"};
	for (int i = 0; i < 17; i++) {
		if (command.compare(commands[i]) == 0) {
			caseId = i;
			break;
		}
	}
	switch (caseId) {
		case 0:
			sendMessage(message, users, fd);
			break;
		case 1:
			if (Parser::getWordCount(message) == 2)
				handleJoin(message, user->second, extractArgument(1, message, 2));
			else
				handleJoin(message, user->second, extractArgument(1, message, 3));
			break;
		case 2:
			user->second.leaveChannel(users, user->second, extractArgument(1, message, 2));
			break;
		case 3:
			user->second.kickUser(users, extractArgument(1, message, 3),
								  extractArgument(2, message, 3));
			break;
		case 4:
			user->second.inviteUser(users, extractArgument(1, message, 3),
									extractArgument(2, message, 3));
			break;
		case 5:
			removeUser(pollId);	 // quitServer();
			break;
		case 6:
			user->second.setNick(user, extractArgument(1, message, 2));
			break;
		case 7:
			listChannels(user->second.getUserName());
			break;
		case 8:
			mode(message, fd);
			break;
		case 9:
			channelTopic(message, extractArgument(1, message, 2), fd);
			break;
		case 10:
			// CAP
			break;
		case 11:
			// to automaticly join to general after providing the right /PASS
			handleJoin(message, user->second, "#General");
			break;
		case 12:
			if (Parser::getWordCount(message) == 3)
				shutdown(message);
			break;
		case 13:;
			who(fd, message);
			break;
		case 14:
			user->second.ping();
			break;
		case 15:
			motd(fd);
			break;
		case 16:
			whois(fd, message);
			break;
		default:
			send_message_to_server(fd, 1, RICK, COMMAND_NOT_FOUND);
			break;
	}
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/