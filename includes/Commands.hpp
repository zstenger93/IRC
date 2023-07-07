#ifndef COMMANDS_HPP
#define COMMANDS_HPP

enum CommandList {
	JOIN,
	LEAVE,
	QUIT,
	NICK,
	LIST,
	TOPIC_USER,
	TOPIC_OPER,
	MODE_USER,
	MODE_OPER,
	KICK,
	INVITE,
	SHUTDOWN
};

class Commands {
   public:
   private:
};


struct t_params
{
	bool is_admin;
	bool is_in_chanal;
	bool is_oppperator;
	bool is_required;
	bool do_need_params;
	bool do_need_flags;
	bool do_need_nick;
};

#endif