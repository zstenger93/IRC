#include "../../includes/Bot.hpp"

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
	setHelpLine(extractFromConfig("helpline"));
	setGrade(extractFromConfig("grade"));
	setAiModelExcuse();
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
	std::string aiCommand = extractArgument(0, message, -1);
	for (std::string::iterator it = aiCommand.begin(); it != aiCommand.end(); ++it) {
		*it = std::tolower(static_cast<unsigned char>(*it));
	}
	std::string aiCommands[6] = {"what is the meaning of life?", "what's the time?", "help",
								 "how should I grade this project?", "tell me a joke"};

	for (int i = 0; i < 6; i++)
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
		case 5:
			break;
		default:
			aiModelExcuse(userFd, userNick);
			break;
	}
}

void Marvin::currentTime(int userFd, std::string userNick) {
	std::time_t currentTime = std::time(NULL);
	std::cout << std::asctime(std::localtime(&currentTime)) << std::endl;
}

void Marvin::setAiModelExcuse() {
	int i = 0;
	std::string line;
	std::ifstream file("conf/asanai.txt");
	while (std::getline(file, line)) {
		asAnAi[i++] = line;
	}
}

void Marvin::answerTmol(int userFd, std::string userNick) {}
void Marvin::answerHelp(int userFd, std::string userNick) {}
void Marvin::answerGrade(int userFd, std::string userNick) {}
void Marvin::generateJoke(int userFd, std::string userNick) {}
void Marvin::aiModelExcuse(int userFd, std::string userNick) {
	for (int i = 0; i < 6; i++)
		send_message_to_server(userFd, 5, getBotName().c_str(), PRIVMSG, userNick.c_str(), COL,
							   asAnAi[i].c_str());
}

/*___________________________________________ SETTERS
 * ___________________________________________*/

void Marvin::setBotName(std::string setTo) { botName = setTo; }
void Marvin::setBotWelcomeLine(std::string setTo) { botWelcomeLine = setTo; }
void Marvin::setBotThink(std::string setTo) { botThink = setTo; }
void Marvin::setBotTmol(std::string setTo) { tmol = setTo; }
void Marvin::setHelpLine(std::string setTo) { helpLine = setTo; }
void Marvin::setFail(std::string setTo) { grade = setTo; }
void Marvin::setGrade(std::string setTo) { fail = setTo; }

/*___________________________________________ GETTERS
 * ___________________________________________*/

std::string Marvin::getBotName() { return botName; }
std::string Marvin::getBotWelcomeLine() { return botWelcomeLine; }
std::string Marvin::getBotThink() { return botThink; }
std::string Marvin::getBotTmol() { return tmol; }
std::string Marvin::getHelpLine() { return helpLine; }
std::string Marvin::getFail() { return fail; }
std::string Marvin::getGrade() { return grade; }