#ifndef USERS_H
#define USERS_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"

namespace database
{
    class Users{
        private:
            long _id;
            std::string _login;
            std::string _first_name;
            std::string _last_name;
            std::string _age;
            

        public:

            static Users fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_login() const;
            const std::string &get_first_name() const;
            const std::string &get_last_name() const;
            const std::string &get_age() const;
            

            long&        id();
            std::string &login();
            std::string &first_name();
            std::string &last_name();
            std::string &age();

            static void init();
            static Users read_by_id(long id);
            static std::vector<Users> read_by_login(std::string login);
            static std::vector<Users> read_all();
            static std::vector<Users> search(std::string first_name,std::string last_name);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif