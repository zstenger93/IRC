#ifndef REPLYCODES_HPP
# define REPLYCODES_HPP

// Reply message
//0 - 5
# define    RPL_WELCOME				"001"
//300
# define    RPL_ENDOFWHO			"315"
# define    RPL_STARTLIST           "321"
# define    RPL_LIST				"322"
# define    RPL_LISTEND				"323"
# define    RPL_TOPIC				"332"
# define    RPL_INVITING			"341"
# define    RPL_WHOREPLY			"352"
# define    RPL_NAMREPLY			"353"
# define    RPL_ENDOFNAMES			"366"
# define    RPL_NOUSERS				"395"
// 400
# define    ERR_NOSUCHNICK			"401"
# define    ERR_NOSUCHCHANNEL		"403"
# define    ERR_TOOMANYTARGETS		"407"
# define    ERR_NOSUCHSERVICE		"408"
# define    ERR_NICKNAMEINUSE		"433"
# define    ERR_USERNOTINCHANNEL	"441"
# define    ERR_USERONCHANNEL		"443"
# define    ERR_NEEDMOREPARAMS		"461"
# define    ERR_ALREADYREGISTRED	"462"
# define    ERR_PASSWDMISMATCH		"464"
# define    ERR_CHANNELISFULL		"471"
# define    ERR_INVITEONLYCHAN		"473"
# define    ERR_BADCHANNELKEY		"475"
// Server Defaults
# define	SRV_QUIT				"You RickRolled out of the server."
# define	SRV_KICK				"THIS IS SPARTAAA. You have been kicked."
#define     COL                     ":"
#define     JOIN_SERVER             " JOIN "
#define     PRIVMSG                 "PRIVMSG"
#define     JOIN                    "JOIN"
#define     COMMAND                 "command"
#define     SERVER                  "server"

#endif