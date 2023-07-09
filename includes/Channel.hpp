#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Defines.hpp"

class Server;

enum ChannelMode {
	TOPIC,
	NORMAL,
	INVITE,
	PASSWORD,
	USERLIMIT,
	OPERATORPRIV,
};

class Channel {
   public:
	Channel(std::string name);
	~Channel();

	void setChannelMode(ChannelMode _mode);
	ChannelMode getChannelMode();
	std::string getChannelName();

   private:
	std::string channelName;
	enum ChannelMode mode;
};

#endif