#pragma once
#include "CliManager.h"
#include <string>
#include <iostream>

using namespace std;

class Manager
{
    private:
        CliManager cliobj;

    public:
        Manager();
        void Register(string username, string password);
        bool IsActive(string username);
        void Active(string username);
        bool IsInActive(string username);
        void InActive(string username);
        void StudentList(string activation = "EMPTY");

};


































