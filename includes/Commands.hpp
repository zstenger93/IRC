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

// class Commands {
//    public:
//    private:
// };

// struct CommandStruct {
// 	bool has_parameters;
// 	bool is_chan_op;
// 	bool is_in_chanal;
// 	bool have_flags;
// };	// SHOULD ADD MORE PARAMETERS

#define DEFINE_COMMAND_STRUCT(name, has_parameters, is_chan_op, is_in_channel, have_flags)
struct name {
	bool has_parameters;
	bool is_chan_op;
	bool is_in_channel;
	bool have_flags;
}

DEFINE_COMMAND_STRUCT(TopicStruct, true, false, true, false);
DEFINE_COMMAND_STRUCT(NickStruct, true, false, true, true);
DEFINE_COMMAND_STRUCT(Invite, true, false, true, true);

#endif