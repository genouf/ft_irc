#pragma once
#include "User.hpp"
#include <map>
#include <string>

class User;

class Channel
{
	private:
		std::string		_name;
		std::string		_topic;
		std::map<int, User*>	_users;

	public:
		Channel();
		Channel(std::string name);
		virtual ~Channel();

		/*	GETTER	*/
		std::string				const 	&getName() const;
		std::string				const 	&getTopic() const;
		std::map<int, User*>			&getUsers();

		/*	SETTER	*/
		void				setName(std::string const &name);
		void				setTopic(std::string topic);

		/*	FUNCTIONS	*/

		/*	ADD OR REMOVE	*/
		void				addUser(User *user);
		void				removeUser(User user);
		void				removeUser(std::string nick);
		/*	INFO	*/
		bool				isUserInChannel(User user);
		int					getNbUsers();
		std::string 		getNbUsers_string();
};
