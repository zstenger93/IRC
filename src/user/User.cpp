#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

User::User() : userName(""), nickName(""), connected(false) {}
User::User(int inputFd) : userName(""), nickName(""), connected(false), userFd(inputFd) {}
User::~User() {}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/
/*___________________________________________ SETTERS ___________________________________________*/

void User::connectUser(bool isConnected) { connected = isConnected; }
void User::setUserName(std::string uName) { userName = uName; }
void User::setNickName(std::string nName) { nickName = nName; }

/*___________________________________________ GETTERS ___________________________________________*/

int User::getUserFd() { return userFd; }
bool User::isConnected() { return connected; }
std::string User::getUserName() { return userName; }
std::string User::getNickName() { return nickName; }