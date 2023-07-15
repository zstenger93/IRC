#ifndef BOT_HPP
#define BOT_HPP

#include "Defines.hpp"

class Marvin {
   public:
	Marvin();
	~Marvin();
	void constructBot();
	void setBotName(std::string setTo);
	void setBotWelcomeLine(std::string setTo);
	void setBotThink(std::string setTo);
	void setBotTmol(std::string setTo);
	void setBotHelpLine(std::string setTo);
	void setBotFail(std::string setTo);
	void setBotGrade(std::string setTo);
	void setBotAiModelExcuse();
	void setBotJokes();
	std::string getBotName();
	std::string getBotWelcomeLine();
	std::string getBotThink();
	std::string getBotTmol();
	std::string getBotHelpLine();
	std::string getBotFail();
	std::string getBotGrade();
	std::string getBotJoke();
	std::string extractFromConfig(std::string lineToFind);
	void runAi(int userFd, std::string userNick, std::string message);
	void currentTime(int userFd, std::string userNick);
	void answerTmol(int userFd, std::string userNick);
	void answerHelp(int userFd, std::string userNick);
	void answerGrade(int userFd, std::string userNick);
	void generateJoke(int userFd, std::string userNick);
	void aiModelExcuse(int userFd, std::string userNick);
	void listPossibleInput(int userFd, std::string userNick);

   private:
	std::string botName;
	std::string botWelcomeLine;
	std::string botThink;
	std::string tmol;
	std::string helpLine;
	std::string fail;
	std::string grade;
	std::vector<std::string> asAnAi;
	std::vector<std::string> jokes;
};

#endif