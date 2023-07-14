#include "../../includes/Bot.hpp"

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
	std::cout << "Marvin has been constructed" << std::endl;
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