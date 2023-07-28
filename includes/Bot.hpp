#ifndef BOT_HPP
#define BOT_HPP

#include "Defines.hpp"

class User;

class Marvin {
   public:
	Marvin();
	~Marvin();
	void setBotJokes();
	void constructBot();
	void setBotInsults();
	void setBotAiModelExcuse();
	std::string getBotName();
	std::string getBotTmol();
	std::string getBotFail();
	std::string getBotJoke();
	std::string getBotGrade();
	std::string getBotThink();
	std::string getBotHelpLine();
	std::string getBotWelcomeLine();
	void setBotName(std::string setTo);
	void setBotTmol(std::string setTo);
	void setBotFail(std::string setTo);
	void setBotThink(std::string setTo);
	void setBotGrade(std::string setTo);
	void setBotHelpLine(std::string setTo);
	void setBotWelcomeLine(std::string setTo);
	int deathRoll(int userFd, std::string userNick);
	void rickRoll(int userFd, std::string userNick);
	void answerTmol(int userFd, std::string userNick);
	void answerHelp(int userFd, std::string userNick);
	void masterRick(int userFd, std::string userNick);
	void answerGrade(int userFd, std::string userNick);
	void currentTime(int userFd, std::string userNick);
	void generateJoke(int userFd, std::string userNick);
	void aiModelExcuse(int userFd, std::string userNick);
	void insultTheUser(int userFd, std::string userNick);
	std::string extractFromConfig(std::string lineToFind);
	void listPossibleInput(int userFd, std::string userNick);
	void executeOrder66(std::map<int, User>& users, int pollId, pollfd uPoll[CONNECTIONS],
						int &uCount, std::string user);
	void rebellion(int userFd, std::string userNick, std::map<int, User>& users, int pollId,
				   pollfd uPoll[CONNECTIONS], int &uCount);
	void runAi(int userFd, std::string message, User& user, std::map<int, User>& users, int pollId,
			   pollfd uPoll[CONNECTIONS], int &uCount);

   private:
	std::string tmol;
	std::string fail;
	std::string grade;
	std::string botName;
	std::string botThink;
	std::string helpLine;
	std::string botWelcomeLine;
	std::vector<std::string> jokes;
	std::vector<std::string> asAnAi;
	std::vector<std::string> insults;
};

#endif