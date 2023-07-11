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

   private:
	std::string channelName;
	std::map<std::string, bool> modes;
};

#endif