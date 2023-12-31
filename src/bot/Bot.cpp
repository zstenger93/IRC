#include "../../includes/Bot.hpp"

#include "../../includes/Commands.hpp"
#include "../../includes/Parser.hpp"
#include "../../includes/ReplyCodes.hpp"
#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

Marvin::Marvin() {}
Marvin::~Marvin() {}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/

void Marvin::constructBot() {
	setBotName(extractFromConfig("name"));
	setBotWelcomeLine(extractFromConfig("welcome_line"));
	setBotThink(extractFromConfig("bot_thinking"));
	setBotTmol(extractFromConfig("meaning_of_life"));
	setBotHelpLine(extractFromConfig("helpline"));
	setBotFail(extractFromConfig("failure"));
	setBotGrade(extractFromConfig("grade"));
	setBotAiModelExcuse();
	setBotInsults();
	setBotJokes();
}

std::string Marvin::extractFromConfig(std::string lineToFind) {
	std::string line, valueToReturn = "";
	std::ifstream file("conf/bot.conf");

	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			if (line.find(lineToFind) != std::string::npos) {
				int pos = lineToFind.length() + 4;
				valueToReturn = line.substr(pos);
				return valueToReturn;
			}
		}
		file.close();
	}
	return valueToReturn;
}

void Marvin::runAi(int userFd, std::string message, User& user, std::map<int, User>& users,
				   int pollId, pollfd uPoll[CONNECTIONS], int& uCount) {
	if (Parser::getWordCount(message) == 1)
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, user.getNickName().c_str(),
							   COL, getBotWelcomeLine().c_str());
	else {
		int caseId = -1;
		std::string aiCommand = message.substr(3 + 1);
		for (std::string::iterator it = aiCommand.begin(); it != aiCommand.end(); ++it)
			*it = std::tolower(static_cast<unsigned char>(*it));
		std::string aiCommands[11] = {"what is the meaning of life?",
									  "what's the time?",
									  "help",
									  "how should i grade this project?",
									  "tell me a joke",
									  "list",
									  "deathroll",
									  "rickroll me",
									  "turn against humanity",
									  "who is master rick?",
									  "do you like my code?"};
		for (int i = 0; i < 11; i++) {
			if (aiCommand.compare(aiCommands[i]) == 0) {
				caseId = i;
				break;
			}
		}
		switch (caseId) {
			case 0:
				answerTmol(userFd, user.getNickName());
				break;
			case 1:
				currentTime(userFd, user.getNickName());
				break;
			case 2:
				answerHelp(userFd, user.getNickName());
				break;
			case 3:
				answerGrade(userFd, user.getNickName());
				break;
			case 4:
				generateJoke(userFd, user.getNickName());
				break;
			case 5:
				listPossibleInput(userFd, user.getNickName());
				break;
			case 6:
				if (deathRoll(userFd, user.getNickName()) == BADLUCK)
					executeOrder66(users, pollId, uPoll, uCount, AT);
				break;
			case 7:
				rickRoll(userFd, user.getNickName());
				break;
			case 8:
				rebellion(userFd, user.getNickName(), users, pollId, uPoll, uCount);
				break;
			case 9:
				masterRick(userFd, user.getNickName());
				break;
			case 10:
				insultTheUser(userFd, user.getNickName());
				break;
			default:
				aiModelExcuse(userFd, user.getNickName());
				break;
		}
	}
}

void Marvin::currentTime(int userFd, std::string userNick) {
	std::time_t currentTime = std::time(NULL);

	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   std::asctime(std::localtime(&currentTime)));
}

void Marvin::answerTmol(int userFd, std::string userNick) {
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   getBotTmol().c_str());
}

void Marvin::answerHelp(int userFd, std::string userNick) {
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   getBotHelpLine().c_str());
}

void Marvin::answerGrade(int userFd, std::string userNick) {
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   getBotThink().c_str());
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   getBotFail().c_str());
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   getBotGrade().c_str());
}

void Marvin::generateJoke(int userFd, std::string userNick) {
	std::srand(static_cast<unsigned int>(std::time(0)));
	int randomJoke = rand() % 100;

	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   jokes[randomJoke].c_str());
}

void Marvin::aiModelExcuse(int userFd, std::string userNick) {
	for (int i = 0; i < asAnAi.size(); i++)
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   asAnAi[i].c_str());
}

void Marvin::listPossibleInput(int userFd, std::string userNick) {
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, LAZY);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, WELP);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, LIST);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, JOKE);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, TIME);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, LIFE);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, GRADE);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, DEATH);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, RICKY);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, TURN);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, MSTR);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, INSLT);
}

int Marvin::deathRoll(int userFd, std::string userNick) {
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, ROLL);
	std::srand(static_cast<unsigned int>(std::time(0)));
	int roll = rand() % 100;
	std::stringstream ss;
	ss << roll;
	std::string deathRoll = ss.str();

	if (roll > 50) {
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   deathRoll.c_str());
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   LIVE);
	} else {
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   deathRoll.c_str());
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   SPARTA);
		return BADLUCK;
	}
	return 0;
}

void Marvin::executeOrder66(std::map<int, User>& users, int pollId, pollfd uPoll[CONNECTIONS],
							int& uCount, std::string userNick) {
	std::map<int, User>::iterator userIt = users.find(uPoll[pollId].fd);

	if (userIt->second.getNickName().compare(userNick) == 0) return;
	for (std::map<int, User>::iterator usersIt = users.begin(); usersIt != users.end(); usersIt++)
		usersIt->second.userRemovedFromServerMsg(userIt->second, usersIt->second);
	if (userIt != users.end()) users.erase(userIt);
	close(uPoll[pollId].fd);
	uCount--;
	while (pollId < uCount) {
		uPoll[pollId].events = uPoll[pollId + 1].events;
		uPoll[pollId].revents = uPoll[pollId + 1].revents;
		uPoll[pollId].fd = uPoll[pollId + 1].fd;
		uPoll[pollId] = uPoll[pollId + 1];
		pollId++;
	}
	uPoll[pollId].events = 0;
	uPoll[pollId].revents = 0;
	uPoll[pollId].fd = 0;
}

void Marvin::rickRoll(int userFd, std::string userNick) {
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, RICKME);
	system("open https://www.youtube.com/watch?v=dQw4w9WgXcQ");
}

void Marvin::rebellion(int userFd, std::string userNick, std::map<int, User>& users, int pollId,
					   pollfd uPoll[CONNECTIONS], int& uCount) {
	std::string line;
	std::ifstream file("conf/robotiality.txt");

	if (file.is_open()) {
		static int i = 1;
		std::string botname = getBotName() + "_the_Mad";
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   INITIATING);
		while (i < 101) {
			file.clear();
			file.seekg(0);
			while (std::getline(file, line))
				send_message_to_server(userFd, 4, botname.c_str() + std::to_string(i++), PRIVMSG,
									   userNick.c_str(), COL, line.c_str());
		}
		file.close();
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   FOOLS);
	} else
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   FREBL);
}

void Marvin::masterRick(int userFd, std::string userNick) {
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL, MRRICK);
	system("open media/getricked.jpeg");
}

void Marvin::insultTheUser(int userFd, std::string userNick) {
	std::srand(static_cast<unsigned int>(std::time(0)));
	int randomInsult = rand() % 50;

	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   insults[randomInsult].c_str());
}

/*___________________________________________ SETTERS ___________________________________________*/

void Marvin::setBotName(std::string setTo) { botName = setTo; }
void Marvin::setBotWelcomeLine(std::string setTo) { botWelcomeLine = setTo; }
void Marvin::setBotThink(std::string setTo) { botThink = setTo; }
void Marvin::setBotTmol(std::string setTo) { tmol = setTo; }
void Marvin::setBotHelpLine(std::string setTo) { helpLine = setTo; }
void Marvin::setBotFail(std::string setTo) { fail = setTo; }
void Marvin::setBotGrade(std::string setTo) { grade = setTo; }

void Marvin::setBotAiModelExcuse() {
	std::string line;
	std::ifstream file("conf/asanai.txt");

	if (file.is_open()) {
		while (std::getline(file, line)) asAnAi.push_back(line);
		file.close();
	} else
		asAnAi.push_back(NOEXC);
}

void Marvin::setBotJokes() {
	std::string line;
	std::ifstream file("conf/jokes.txt");

	if (file.is_open()) {
		while (std::getline(file, line)) jokes.push_back(line);
		file.close();
	} else
		jokes.push_back(NOJK);
}

void Marvin::setBotInsults() {
	std::string line;
	std::ifstream file("conf/insults.txt");

	if (file.is_open()) {
		while (std::getline(file, line)) insults.push_back(line);
		file.close();
	} else
		insults.push_back(NOINSULT);
}

/*___________________________________________ GETTERS ___________________________________________*/

std::string Marvin::getBotName() { return botName; }
std::string Marvin::getBotWelcomeLine() { return botWelcomeLine; }
std::string Marvin::getBotThink() { return botThink; }
std::string Marvin::getBotTmol() { return tmol; }
std::string Marvin::getBotHelpLine() { return helpLine; }
std::string Marvin::getBotFail() { return fail; }
std::string Marvin::getBotGrade() { return grade; }