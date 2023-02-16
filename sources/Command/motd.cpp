#include "../../includes/Server.hpp"

int	Server::cmd_motd(std::vector<std::string> params, User &user)
{
	(void)params;
	this->send_client("375 " + user.getNick() + " :- " + "CGG Message of the day - ", user);
	this->send_client("372 " + user.getNick() + " :- " + "Welcome to the CGG server", user);
	this->send_client("372 " + user.getNick() + "                     ,ood8888booo,", user);
	this->send_client("372 " + user.getNick() + "                  ,od8           8bo,", user);
	this->send_client("372 " + user.getNick() + "               ,od                   bo,", user);
	this->send_client("372 " + user.getNick() + "             ,d8                       8b,", user);
	this->send_client("372 " + user.getNick() + "            ,o                           o,    ,a8b", user);
	this->send_client("372 " + user.getNick() + "           ,8                             8,,od8  8", user);
	this->send_client("372 " + user.getNick() + "           8'                             d8'     8b", user);
	this->send_client("372 " + user.getNick() + "           8                           d8'ba     aP'", user);
	this->send_client("372 " + user.getNick() + "           Y,                       o8'         aP'", user);
	this->send_client("372 " + user.getNick() + "            Y8,                      YaaaP'    ba", user);
	this->send_client("372 " + user.getNick() + "             Y8o                   Y8'         88", user);
	this->send_client("372 " + user.getNick() + "              `Y8               ,8\"           `P", user);
	this->send_client("372 " + user.getNick() + "                Y8o        ,d8P'              ba", user);
	this->send_client("372 " + user.getNick() + "           ooood8888888P\"\"\"'                  P'", user);
	this->send_client("372 " + user.getNick() + "        ,od                                  8", user);
	this->send_client("372 " + user.getNick() + "     ,dP     o88o                           o'", user);
	this->send_client("372 " + user.getNick() + "    ,dP          8                          8", user);
	this->send_client("372 " + user.getNick() + "   ,d'   oo       8                       ,8", user);
	this->send_client("372 " + user.getNick() + "   $    d\"$8      8           Y    Y  o   8", user);
	this->send_client("372 " + user.getNick() + "  d    d  d8    od  \"\"boooooooob   d\"\" 8   8", user);
	this->send_client("372 " + user.getNick() + "  $    8  d   ood' ,   8        b  8   '8  b", user);
	this->send_client("372 " + user.getNick() + "  $   $  8   b    Y  d8        `b  d    '8  b", user);
	this->send_client("372 " + user.getNick() + "   $  $ 8   b    Y  d8          8 ,P     '8  b", user);
	this->send_client("372 " + user.getNick() + "   `$$  Yb  b     8b 8b         8 8,      '8  o,", user);
	this->send_client("372 " + user.getNick() + "        `Y  b      8o  $$o      d  b        b   $o", user);
	this->send_client("372 " + user.getNick() + "         8   '$     8$,,$\"      $   $o      '$o$$", user);
	this->send_client("372 " + user.getNick() + "          $o$$P\"                 $$o$", user);
	this->send_client("376 " + user.getNick() + " :End of MOTD command.", user);
	return (0);
}
