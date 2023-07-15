#include "../../includes/Bot.hpp"

#include <unistd.h>

#include <cstdlib>

#include "../../includes/Commands.hpp"
#include "../../includes/ReplyCodes.hpp"

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
	setBotJokes();
}

std::string Marvin::extractFromConfig(std::string lineToFind) {
	std::string line, valueToReturn = "";
	std::ifstream file("conf/bot.conf");
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		if (line.find(lineToFind) != std::string::npos) {
			int pos = lineToFind.length() + 4;
			valueToReturn = line.substr(pos);
			return valueToReturn;
		}
	}
	return valueToReturn;
}

void Marvin::runAi(int userFd, std::string userNick, std::string message) {
	int caseId;
	std::string aiCommand = message.substr(3 + 1);
	for (std::string::iterator it = aiCommand.begin(); it != aiCommand.end(); ++it) {
		*it = std::tolower(static_cast<unsigned char>(*it));
	}
	std::string aiCommands[6] = {"what is the meaning of life?\r\n", "what's the time?\r\n",
								 "help\r\n", "how should i grade this project?\r\n",
								 "tell me a joke\r\n"};
	for (int i = 0; i < 5; i++)
		if (aiCommand.compare(aiCommands[i]) == 0) {
			caseId = i;
			break;
		}
	switch (caseId) {
		case 0:
			answerTmol(userFd, userNick);
			break;
		case 1:
			currentTime(userFd, userNick);
			break;
		case 2:
			answerHelp(userFd, userNick);
			break;
		case 3:
			answerGrade(userFd, userNick);
			break;
		case 4:
			generateJoke(userFd, userNick);
			break;
		default:
			aiModelExcuse(userFd, userNick);
			break;
	}
}

void Marvin::currentTime(int userFd, std::string userNick) {
	std::time_t currentTime = std::time(NULL);
	send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
						   std::asctime(std::localtime(&currentTime)));
}

void Marvin::setBotAiModelExcuse() {
	int i = 0;
	std::string line;
	std::ifstream file("conf/asanai.txt");
	while (std::getline(file, line)) {
		asAnAi.push_back(line);
	}
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
	for (int i = 0; i < 6; i++)
		send_message_to_server(userFd, 4, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   asAnAi[i].c_str());
}

void Marvin::setBotJokes() {
	int i = 0;
	std::string line;
	std::ifstream file("conf/jokes.txt");
	while (std::getline(file, line)) {
		jokes.push_back(line);
	}
}

/*___________________________________________ SETTERS ___________________________________________*/

void Marvin::setBotName(std::string setTo) { botName = setTo; }
void Marvin::setBotWelcomeLine(std::string setTo) { botWelcomeLine = setTo; }
void Marvin::setBotThink(std::string setTo) { botThink = setTo; }
void Marvin::setBotTmol(std::string setTo) { tmol = setTo; }
void Marvin::setBotHelpLine(std::string setTo) { helpLine = setTo; }
void Marvin::setBotFail(std::string setTo) { fail = setTo; }
void Marvin::setBotGrade(std::string setTo) { grade = setTo; }

/*___________________________________________ GETTERS ___________________________________________*/

std::string Marvin::getBotName() { return botName; }
std::string Marvin::getBotWelcomeLine() { return botWelcomeLine; }
std::string Marvin::getBotThink() { return botThink; }
std::string Marvin::getBotTmol() { return tmol; }
std::string Marvin::getBotHelpLine() { return helpLine; }
std::string Marvin::getBotFail() { return fail; }
std::string Marvin::getBotGrade() { return grade; }