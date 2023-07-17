#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Defines.hpp"

class Server;

class Channel {
   public:
	Channel(std::string name);
	~Channel();

	int getUserCount();
	int getUserLimit();
	std::string getChannelName();
	void changeUserCount(int count);
	bool checkMode(std::string mode);
	const std::string getChannelTopic();
	void setChannelUserLimit(int setTo);
	void setChannelPassword(std::string setTo);
	void addMode(std::string mode, bool value);
	void setChannelTopic(std::string newChannelTopic);
	bool isPasswordCorrect(std::string inputPassword);
	const std::map<std::string, bool> &getChannelModes();

   private:
	std::string channelName;
	std::string channelTopic;
	std::map<std::string, bool> modes;
	std::string channelPassword;
	int userCount;
	int userLimit;
};

#endif