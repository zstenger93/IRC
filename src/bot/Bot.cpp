#include "../../includes/Bot.hpp"

#include "../../includes/Commands.hpp"

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

void Marvin::runAi(std::string message) {
	int caseId;
	std::string command = extractArgument(0, message, -1);
	std::string commands[6] = {"what is the meaning of life?", "what's the time?", "help",
							   "how should I grade this project?", "tell me a joke"};

	for (int i = 0; i < 6; i++)
		if (command.compare(commands[i]) == 0) {
			caseId = i;
			break;
		}

	switch (caseId) {
		case 0:
			break;
		case 1:
			currentTime();
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			asAnAiLanguageModel();
			break;
	}
}

void Marvin::currentTime() {
	std::time_t currentTime = std::time(NULL);
	std::cout << std::asctime(std::localtime(&currentTime)) << std::endl;
}

void Marvin::asAnAiLanguageModel() {}

/*___________________________________________ SETTERS ___________________________________________*/

void Marvin::setBotName(std::string setTo) { botName = setTo; }
void Marvin::setBotWelcomeLine(std::string setTo) { botWelcomeLine = setTo; }
void Marvin::setBotThink(std::string setTo) { botThink = setTo; }
void Marvin::setBotTmol(std::string setTo) { tmol = setTo; }
void Marvin::setHelpLine(std::string setTo) { helpLine = setTo; }
void Marvin::setFail(std::string setTo) { grade = setTo; }
void Marvin::setGrade(std::string setTo) { fail = setTo; }

/*___________________________________________ GETTERS ___________________________________________*/

std::string Marvin::getBotName() { return botName; }
std::string Marvin::getBotWelcomeLine() { return botWelcomeLine; }
std::string Marvin::getBotThink() { return botThink; }
std::string Marvin::getBotTmol() { return tmol; }
std::string Marvin::getHelpLine() { return helpLine; }
std::string Marvin::getFail() { return fail; }
std::string Marvin::getGrade() { return grade; }