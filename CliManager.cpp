#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "CliManager.h"
#include "Manager.h"
#include "Food.h"
#include <fstream>
#include <cstdio>

using namespace std;



void CliManager::run()
{
    Food foodobj;
    Manager managerobj;

    if(this->argc == 1)
    {
        this->help();
        return;
    }
    
    string command = this->argv[1];

    if(command == "LOGIN")
    {
        Login(this->argv[2], this->argv[3]);
        return;
    }

    if(command == "LOGOUT")
    {
        Logout();
        return;
    }

    if(!isLoggedIn())
    {
        cout<<"YOU NEED TO LOGIN FIRST";
        return;
    }

    if(isCommandAllowed(command))
    {
     if(command == "MENU")
        {
            foodobj.View(foodobj.StringToDate(this->argv[2]), foodobj.StringToDate(this->argv[3]));
        }
    else if(command == "RESERVE")
        {
            foodobj.Reserve(foodobj.StringToDate(this->argv[2]), this->argv[3]);
        }
    else if(command == "REGISTER")
        {
            managerobj.Register(this->argv[2], this->argv[3]);
        }
    else if(command == "ACTIVE")
        {
            managerobj.Active(this->argv[2]);
        }
    else if(command == "INACTIVE")
        {
            managerobj.InActive(this->argv[2]);
        }
    else if(command == "LIST")
        {
            if(this->argc == 2)
            {
                managerobj.StudentList();
            }
            else
            {
                managerobj.StudentList(this->argv[2]);
            }
        }
    else if(command == "ADDFOOD")
        {
            foodobj.AddFood(this->argv[2], stoi(this->argv[3]), foodobj.StringToDate(this->argv[4]));
        }
    else if(command == "REMOVEFOOD")
        {
            foodobj.RemoveFood(this->argv[2], foodobj.StringToDate(this->argv[3]));
        }
    else if(command == "REPORT")
        {
            foodobj.ReserveReport(foodobj.StringToDate(this->argv[2]),foodobj.StringToDate(this->argv[3]));
        }

    }
    else
    {
        cout<<"ACCESS DENIED";
        return;
    }

    

   
}



bool CliManager::isLoggedIn()
{
    ifstream sessionFile("session.txt");
    string line;

    if(sessionFile && getline(sessionFile, line) && !line.empty())
        return true;
    else
        return false;
}


bool CliManager::isCommandAllowed(string command)
{
    ifstream sessionFile("session.txt");
    string line;
    
    getline(sessionFile, line);

    string temp; 
    stringstream stream(line);
    vector<string> parts;
    while(stream >> temp)
    {
        parts.push_back(temp);
    }

    if(parts[3] == "User")
    {
        if(command == "LOGIN" || command == "LOGOUT" || command == "MENU" ||
            command == "RESERVE")
        {
            return true;
        }
        else
            return false;
    }
    else
    {
        if(command == "REGISTER" || command == "ACTIVE" || command == "INACTIVE" ||
            command == "LIST" || command == "ADDFOOD" || command == "REMOVEFOOD" ||
            command == "REPORT")
        {
            return true;
        }
        else
            return false;
    }
}


void CliManager::help()
{
    cout<<"====== This is Command Line Inteface (CLI) Application ======"<<endl;
    cout<<"Please use parameters when you run app."<<endl; 
    cout<<"Like: ./app command1 command2 ...";
}


void CliManager::Login(string username, string password)
{
    string line, fileusername, filepassword, filestatus, filerole;

    if(this->isLoggedIn())
    {
        cout<<"YOU NEED TO LOGOUT FIRST";
        return;
    }
    else
    {
        ifstream inmanager("Managers.txt");
        if(!inmanager)
        {
            inmanager.close();
            return;
        }
        while(getline(inmanager, line))
        {
            stringstream ss(line);
            ss >> fileusername >> filepassword;
            if(fileusername == username && filepassword == password)
            {
                cout<<username<<" LOGGEDIN SUCCESSFULLY";

                ofstream sessionFile("session.txt");
                sessionFile<<fileusername<<" "<<filepassword<<" 1 Manager";

                sessionFile.close();
                inmanager.close();
                return;
            }
        }

        inmanager.close();
        line.clear();

        ifstream in("Users.txt");
        if(!in)
        {
            in.close();
            return;
        }


        while(getline(in, line))
        {
            stringstream ss(line);

            ss >> fileusername >> filepassword >> filestatus >> filerole;
        
            if(fileusername == username && filepassword == password && filestatus == "0" && filerole == "User")
            {
                cout<<"ACCOUNT IS DEACTIVE";
                in.close();
                return;
            }
            
            if(fileusername == username && filepassword == password && filerole == "User")
            {
                cout<<username<<" LOGGEDIN SUCCESSFULLY";

                ofstream sessionFile("session.txt");
                sessionFile<<fileusername<<" "<<filepassword<<" "<<filestatus<<" "<<filerole;

                sessionFile.close();
                in.close();
                return;
            }
        }

        cout<<"WRONG PASSWORD OR USERNAME DOESN'T EXIST";
        in.close();

    }
}


void CliManager::Logout()
{
    if(isLoggedIn())
    {
        remove("session.txt");
        cout<<"LOGGEDOUT SUCCESSFULLY";
    }
    else
    {
        cout<<"YOU NEED TO LOGIN FIRST";
        return;
    }
}


bool CliManager::StrongPassword(string password)
{
    if(password.length() < 8)
        return false;

    bool upper = false, lower = false, digit = false, specialchar = false;

    for(int i = 0; i < password.length(); i++)
    {
        if(isupper(password[i]))
            upper = true;
        if(islower(password[i]))
            lower = true;
        if(isdigit(password[i]))
            digit = true;
    }
    if(password.find('!') != string::npos || password.find('@') != string::npos || password.find('#') != string::npos || password.find('$') != string::npos ||
           password.find('%') != string::npos || password.find('^') != string::npos || password.find('&') != string::npos || password.find('*') != string::npos ||
           password.find('-') != string::npos || password.find('_') != string::npos || password.find('=') != string::npos || password.find('+') != string::npos)
        {
            specialchar = true;
        }
    
    return upper && lower && digit && specialchar;
}


bool CliManager::UserExist(string username)
{
    ifstream in("Users.txt");
    if(!in)
    {return false;}

    string line, fileusername;
    
    while(getline(in, line))
    {
        stringstream ss(line);

        ss >>fileusername;
        
        if(fileusername == username)
        {
            return true;
        }
    }

    return false;
}


string CliManager::GetUsername()
{
    string line, username;
    ifstream in("session.txt");
    getline(in, line);

    stringstream ss(line);
    ss >> username;

    in.close();

    return username;
}

CliManager::CliManager(){}









