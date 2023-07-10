#include "../../includes/Channel.hpp"
#include "../../includes/Commands.hpp"
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

void Server::commandParser(std::map<int, User>::iterator user, std::string message, int fd, int pollId) {
	int caseId = 0;
	std::string command = getCommand(message);
	std::string commands[15] = {"PRIVMSG",	  "JOIN",		"PART", "KICK",	  "INVITE",
								"QUIT",		  "NICK",		"LIST",	 "MODE_USER", "MODE_OPER",
								"TOPIC_USER", "TOPIC_OPER", "CAP",	 "PASS", "ADMIN"};
	for (int i = 0; i < 15; i++) {
		if (command.compare(commands[i]) == 0) {
			caseId = i;
			break;
		}
	}
	switch (caseId) {
		case 0:
			user->second.sendMessage();
			break;
		case 1:
			handleJoin(user->second, extractArgument(1, message, 2));
			break;
		case 2:
			user->second.leaveChannel(extractArgument(1, message, 2));
			break;
		case 3:
			user->second.kickUser(users, extractArgument(1, message, 3), extractArgument(2, message, 3));
			break;
		case 4:
			user->second.inviteUser(users, extractArgument(1, message, 3), extractArgument(2, message, 3));
			break;
		case 5:
			user->second.quitServer();
			users.erase(users.find(user->second.getUserFd()));
			removeUser(pollId);
			break;
		case 6:
			user->second.setNick(user, extractArgument(1, message, 2));
			break;
		case 7:
			user->second.listChannels();
			break;
		case 8:
			user->second.modeUser();
			break;
		case 9:
			user->second.modeOper();
			break;
		case 10:
			user->second.topicUser();
			break;
		case 11:
			user->second.topicOper();
			break;
		case 12:;
			// just for silence the error, handled in authentication for /CAP
			user->second.quitServer();
			break;
		case 13:;
			//to automaticly join to general after providing the right /PASS
			handleJoin(user->second, "#General");
			break;
		case 14:
			shutdown();
			break;
		default:
			send_message_to_server(fd, 1, COMMAND_NOT_FOUND);
			break;
	}
}

/*___________________________________________ SETTERS ___________________________________________*/
/*___________________________________________ GETTERS ___________________________________________*/