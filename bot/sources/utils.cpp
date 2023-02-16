#include "../includes/Bot.hpp"
#include <sstream>

std::vector<std::vector<std::string> >	Bot::parsing_msg(std::string msg)
{
	std::vector<std::vector<std::string> > 	big_v;
	std::istringstream						sstream(msg);
	std::string								s;

	while (std::getline(sstream, s, '\n'))
	{
		std::istringstream			sstream2(s);
		std::string					s2;
		std::vector<std::string>	tmp;

		while (std::getline(sstream2, s2, ' '))
		{
			if (s2.find("\n") != std::string::npos)
				s2.replace(s2.find("\n"), 1, "");
			if (s2.find("\r") != std::string::npos)
				s2.replace(s2.find("\r"), 1, "");
			tmp.push_back(s2);
		}
		big_v.push_back(tmp);
	}
	return (big_v);
}

void	Bot::pop_back_str(std::string &str)
{
	std::string::iterator it = str.begin() + str.size();
	str.erase(it, str.end());
	return ;
}