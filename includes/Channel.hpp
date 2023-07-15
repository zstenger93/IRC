#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Defines.hpp"

class Server;

class Channel {
   public:
	Channel(std::string name);
	~Channel();

	std::string getChannelName();
	void addMode(std::string mode, bool value);
	bool checkMode(std::string mode);
	const std::map<std::string, bool> &getChannelModes();
	const std::string getChannelTopic();
	void setChannelTopic(std::string newChannelTopic);
	void changeUserCount(int count);
	int getUserCount();
	int getUserLimit();
	bool isPasswordCorrect(std::string inputPassword);

   private:
	std::string channelName;
	std::string channelTopic;
	std::map<std::string, bool> modes;
	std::string channelPassword;
	int userCount;
	int userLimit;
};

#endif