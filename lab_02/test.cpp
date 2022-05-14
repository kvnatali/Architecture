#include <gtest/gtest.h>
#include <thread>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/StreamCopier.h>

#include "database/database.h"
#include "database/users.h"
#include "config/config.h"

bool isIinited = false;

void setDatabase() {
    if (isIinited) return;

    Config::get().host() = "127.0.0.1";
    Config::get().port() = "6033";
    Config::get().database() = "sql_test";
    Config::get().login() = "test";
    Config::get().password() = "pzjqUkMnc7vfNHET";
    
    isIinited = true;
}

TEST(check_user_login, user_table_set) {
    database::Users user;
    user.login() = "SSokolov";
    EXPECT_EQ("SSokolov", user.get_login()) << "логины должны совпадать";
}

TEST(create_new_user, user_creation_set) {

    setDatabase();

    database::Users user;
    user.first_name() = "Olga";
    user.last_name()  = "Orlova";
    user.login()      = "OOrlova";
    user.age()        = "60";

    try
    {
        user.save_to_mysql();
        ASSERT_TRUE(true);
    }
    catch (...)
    {
        ASSERT_TRUE(false);
    }
}

TEST(search_by_login, basic_test_set) {

    setDatabase();

    try
    {
        std::vector<database::Users> result;
        result = database::Users::read_by_login("nivanova");
        EXPECT_EQ("nivanova", result[0].get_login()) << "логины должны совпадать";
    }
    catch (...)
    {
        ASSERT_TRUE(false);
    } 

}


int main ([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}