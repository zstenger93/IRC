#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

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

   private:
	std::string channelName;
	enum ChannelMode mode;
};

#endif