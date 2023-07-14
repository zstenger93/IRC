#ifndef BOT_HPP
#define BOT_HPP

#include "Defines.hpp"

class Server;

class Marvin {
   public:
	Marvin();
	~Marvin();
	void constructBot();
	void setBotName(std::string setTo);
	void setBotWelcomeLine(std::string setTo);
	void setBotThink(std::string setTo);
	void setBotTmol(std::string setTo);
	void setHelpLine(std::string setTo);
	void setGrade(std::string setTo);
	std::string getBotName();
	std::string getBotWelcomeLine();
	std::string getBotThink();
	std::string getBotTmol();
	std::string getHelpLine();
	std::string getGrade();
	std::string extractFromConfig(std::string lineToFind);

   private:
	std::string botName;
	std::string botWelcomeLine;
	std::string botThink;
	std::string tmol;
	std::string helpLine;
	std::string grade;
};

#endif