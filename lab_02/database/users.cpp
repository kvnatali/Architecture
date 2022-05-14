#include "users.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>
#include <fstream>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Users::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            //*
            Statement drop_stmt(session);
            drop_stmt << "DROP TABLE IF EXISTS users", now;
            //*/

            // (re)create table
                        for (auto &hint : database::Database::get_all_hints())
            {
            Statement create_stmt(session);
            create_stmt <<"create table if not exists users ("
                        <<"id INT AUTO_INCREMENT,"
                        <<"login varchar(255) not null,"
                        <<"first_name varchar(255),"
                        <<"last_name varchar(255),"
                        <<"age varchar(255),"
                        <<"CONSTRAINT pk_users PRIMARY KEY (id),"
                        <<"CONSTRAINT c_login UNIQUE (login)"
                        <<");"
                        << hint,
                now;
            }
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void Users::preload(const std::string &file)
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            std::string json;
            std::ifstream is(file);
            std::istream_iterator<char> eos;
            std::istream_iterator<char> iit(is);
            while (iit != eos)
                json.push_back(*(iit++));
            is.close();

            Poco::JSON::Parser parser;
            Poco::Dynamic::Var result = parser.parse(json);
            Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();

            size_t i{0};
            for (i = 0; i < arr->size(); ++i)
            {
                Poco::JSON::Object::Ptr object = arr->getObject(i);
                std::string login = object->getValue<std::string>("login");
                std::string first_name = object->getValue<std::string>("first_name");
                std::string last_name = object->getValue<std::string>("last_name");
                std::string age = object->getValue<std::string>("age");
    
                Poco::Data::Statement insert(session);
                insert << "INSERT INTO users (login, first_name, last_name, age) VALUES(?, ?, ?, ?)",
                    Poco::Data::Keywords::use(login),
                    Poco::Data::Keywords::use(first_name),
                    Poco::Data::Keywords::use(last_name),
                    Poco::Data::Keywords::use(age);

                insert.execute();
            }

            std::cout << "Inserted " << i << " records" << std::endl;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Users::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("login", _login);
        root->set("first_name", _first_name);
        root->set("last_name", _last_name);
        root->set("age", _age);

        return root;
    }

    Users Users::fromJSON(const std::string &str)
    {
        Users user;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user.login() = object->getValue<std::string>("login");
        user.first_name() = object->getValue<std::string>("first_name");
        user.last_name() = object->getValue<std::string>("last_name");
        user.age() = object->getValue<std::string>("age");
        

        return user;
    }

    Users Users::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Users u;
            select << "SELECT login, first_name, last_name, age FROM users where id=?",
                into(u._login),
                into(u._first_name),
                into(u._last_name),
                into(u._age),
                
                use(id),
                range(0, 1); //  iterate over result set one row at a time
  
            if (!select.done())
                if (!select.execute())
                    throw new std::logic_error("Item not found");

            return u;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<Users>  Users::read_by_login(std::string login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::vector<Users> result;
            Users u;
            std::string sharding_hint = database::Database::sharding_hint(login);
            login = "%" + login + "%";
            std::string select_str = "SELECT login, first_name, last_name, age FROM users where login like ? ";
            select_str += sharding_hint;
            std::cout << select_str << std::endl;

            select << select_str,
                into(u._login),
                into(u._first_name),
                into(u._last_name),
                into(u._age),
                
                use(login),
                range(0, 1); //  iterate over result set one row at a time
  
            while (!select.done())
            {
                if(select.execute())
                result.push_back(u);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    std::vector<Users> Users::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Users> result;
            Users u;
            std::cout << "SELECT login,first_name,last_name,age FROM users" << std::endl;
            select << "SELECT login,first_name,last_name,age FROM users",
                into(u._login),
                into(u._first_name),
                into(u._last_name),
                into(u._age),
                
                range(0, 1); //  iterate over result set one row at a time
            while (!select.done())
            {
                if(select.execute())
                result.push_back(u);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::MySQLException &e)
        {
            std::cout << "MySQLException: " << e.what() << std::endl;
            throw;
        }
    }

    std::vector<Users> Users::search(std::string first_name, std::string last_name)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Users> result;
            Users u;
            first_name+="%";
            last_name+="%";
            for (size_t shard_num = 0; shard_num < database::Database::get_max_shard(); shard_num++)
            {
                std::string select_str = "SELECT login, first_name, last_name, age FROM users where first_name LIKE ? and last_name LIKE ?";
                std::string sharding_hint = "-- sharding:";
                sharding_hint += std::to_string(shard_num);
                select_str += sharding_hint;
                select << select_str,
                    into(u._login),
                    into(u._first_name),
                    into(u._last_name),
                    into(u._age),
                    
                    use(first_name),
                    use(last_name),
                    range(0, 1); //  iterate over result set one row at a time

                std::cout << select_str << std::endl;
                while (!select.done())
                {
                    if(select.execute())  result.push_back(u);
                }
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

   
    void Users::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);
            std::string sharding_hint = database::Database::sharding_hint(_login);

            std::string select_str = "INSERT INTO users (login,first_name,last_name,age) VALUES(?, ?, ?, ?)";
            select_str += sharding_hint;
            std::cout << select_str << std::endl;
            insert << select_str,
                use(_login),
                use(_first_name),
                use(_last_name),
                use(_age),

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Users::get_id() const
    {
        return _id;
    }

    const std::string &Users::get_login() const
    {
        return _login;
    }

    const std::string &Users::get_first_name() const
    {
        return _first_name;
    }

    const std::string &Users::get_last_name() const
    {
        return _last_name;
    }

    const std::string &Users::get_age() const
    {
        return _age;
    }



    long &Users::id()
    {
        return _id;
    }

    std::string &Users::login()
    {
        return _login;
    }

    std::string &Users::first_name()
    {
        return _first_name;
    }

    std::string &Users::last_name()
    {
        return _last_name;
    }

    std::string &Users::age()
    {
        return _age;
    }

}