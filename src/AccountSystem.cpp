#include <iostream>
#include <fstream>
#include <utility>
#include <sstream>
#include "AccountSystem.h"
#define red(text) "\033[31m" text "\033[0m"
#define green(text) "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text) "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text) "\033[36m" text "\033[0m"
#define white(text) "\033[37m" text "\033[0m"

User *AccountSystem::search(std::string name)
{
    for (size_t i = 0; i < user_list.size(); i++)
    {
        if (name == user_list[i].getUsername())
        {
            return &user_list[i];
        }
    }
    return nullptr;
}
void AccountSystem::init(std::string USER_DATA_PATH)
{

    AccountSystem::USER_DATA_PATH = USER_DATA_PATH;

    try
    {

        std::ifstream file(USER_DATA_PATH); // 開啟檔案
        if (!file)
        {
            throw std::runtime_error("Error: File does not exist - " + USER_DATA_PATH);
        }

        // TODO: Loading user data from USER_DATA_PATH and call adding_user (from AccountSystem) function to insert data
        // Hints: stringstream
        std::string read_line;
        while (getline(file, read_line))
        {
            std::string username, password;
            std::stringstream ss(read_line);

            // do something
            getline(ss, username, ',');
            getline(ss, password, ',');

            AccountSystem::adding_user(username, password);
        }
        file.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
}

void AccountSystem::sign_up()
{
    std::string user_name, user_password, user_password_2;
    std::cout << yellow("Welcome, please enter your name: ");
    getline(std::cin, user_name);
    // check if user already exist(next time)
    //  if(search(user_name))
    while (true)
    {

        std::cout << yellow("Welcome, please enter your password: ");
        getline(std::cin, user_password);
        std::cout << yellow("Welcome, please enter your password again: ");
        getline(std::cin, user_password_2);
        // check if same
        if (user_password != user_password_2)
        {
            std::cout << red("Two passwords are not the same, please try again\n");
        }
        else
        {
            // successful register new user
            break;
        }
    }
    // add user to user list
    adding_user(user_name, user_password);
}
std::pair<bool, std::string> AccountSystem::login()
{
    std::pair<bool, std::string> result = std::make_pair(false, "");
    std::string user_name, user_password;
    bool login_success = false;
    while (!login_success)
    {
        std::cout << yellow("User name (Enter -1 to sign up): ");
        getline(std::cin, user_name);
        if (user_name == "-1")
        {
            sign_up();
            return std::make_pair(false, "");
        }
        // search usr
        User *login_usr;
        login_usr = search(user_name);
        while (login_usr == nullptr)
        {
            std::cout << red("User not found! Please enter User name again!\n");
            std::cout << yellow("User name (Enter -1 to sign up): ");
            getline(std::cin, user_name);
            // if enter -1 then sign up
            if (user_name == "-1")
            {
                sign_up();
                return std::make_pair(false, "");
            }
            // search again
            login_usr = search(user_name);
        }
        // if user exist, enter password
        std::cout << "Welcome, " << login_usr->getUsername() << "\n";
        std::cout << "Please enter your password: ";
        getline(std::cin, user_password);
        // try pass word, max 3 times
        int n = 3;
        while (n > 0)
        {

            // if password correct
            if (login_usr->getPassword() == user_password)
            {
                login_success = true;
                break;
            }
            if (n == 1)
            {
                std::cout << red("Too many unsuccessful sign-in attemps...\n ");
            }
            else
            {
                std::cout << red("Password incorrect... Please try again: ");
                getline(std::cin, user_password);
            }
            n--;
        }
    }

    // if login success
    std::cout << green("Login Success!! welcome abord!!\n");
    // update login_user
    AccountSystem::login_user = user_name;
    result = std::make_pair(true, user_name);
    return result;
}
void AccountSystem::adding_user(std::string username, std::string password)
{
    User new_user(username, password);
    user_list.push_back(new_user);
    AccountSystem::userdataUpdate();
    return;
}
std::string AccountSystem::getuserLogin()
{
    return login_user;
}
void AccountSystem::userdataUpdate()
{

    try
    {
        std::ofstream info_out(AccountSystem::USER_DATA_PATH);
        if (!info_out)
        {
            throw std::runtime_error("Error: File does not exist - " + USER_DATA_PATH);
        }

        // 把整個 vector 跑過一次，透過 info_out 寫入
        // Hints: 你可能會需要 class User 的 getter 才能取得某些資訊
        for (auto &usr : user_list)
        {
            std::string username = usr.getUsername();
            std::string password = usr.getPassword();

            info_out << username << "," << password << "\n";
        }
        info_out.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
}
