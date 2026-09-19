#pragma once
#include <string>

using namespace std;

class CliManager
{
    private:
        int argc;
        char **argv;


    public:
        CliManager();
        CliManager(int cargc, char** cargv):argc(cargc), argv(cargv){}
        void run();
        bool isLoggedIn();
        void Login(string username, string password);
        bool UserExist(string username);
        string GetUsername();
        void Logout();
        bool StrongPassword(string password);
        bool isCommandAllowed(string command);
        void help();




};