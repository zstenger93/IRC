#include "../../includes/User.hpp"

/*__________________________________ CONSTRUCTORS / DESTRUCTOR __________________________________*/

User::User() : connected(true) {}
User::~User() {}

/*_____________________________________ OPERATOR OVERLOADS ______________________________________*/
/*_______________________________________ NESTED CLASSES ________________________________________*/
/*__________________________________________ FUNCTIONS __________________________________________*/
/*___________________________________________ SETTERS ___________________________________________*/

void User::setUserName(std::string uName) { userName = uName; }
void User::setNickName(std::string nName) { nickName = nName; }

/*___________________________________________ GETTERS ___________________________________________*/

bool User::isConnected() { return connected; }
std::string User::getUserName() { return userName; }
std::string User::getNickName() { return nickName; }