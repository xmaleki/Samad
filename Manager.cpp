#include "Manager.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

void Manager::Register(string username, string password)
{
    if(cliobj.UserExist(username))
    {
        cout<<"USERNAME "<<username<<" ALREADY EXISTS";
    }
    else if(cliobj.StrongPassword(password))
    {
        ofstream out("Users.txt" , ios::app);
        out<<username<<" "<<password<<" "<<"0"<<" User"<<endl;
        out.close();
        cout<<username<<" REGISTERED SUCCESSFULLY";
    }
    else
    {
        cout<<"PASSWORD IS NOT STRONG ENOUGH";
    }
}


void Manager::Active(string username)
{
    if(!cliobj.UserExist(username))
    {
        cout<<"USER NOT FOUND";
        return;
    }

    if(IsActive(username))
    {
        cout<<"USER WAS ALREADY ACTIVE";
        return;
    }

    string line, filepassword, fileusername, activation, role;
    ifstream inFile("Users.txt");
    ofstream outFile("tempUsers.txt");

    while(getline(inFile, line))
    {
        if(line.empty())
        {
            outFile<<line<<"\n";
            continue;
        }

        stringstream ss(line);

        ss >> fileusername;

        if(fileusername == username)
        {
            ss >> filepassword >> activation >> role;
            outFile<<fileusername<<" "<<filepassword<<" "<<"1"<<" "<<role<<endl;
        }
        else
        {
            outFile<<line<<endl;
        }
    }

    inFile.close();
    outFile.close();

    remove("Users.txt");
    rename("tempUsers.txt", "Users.txt");
}



bool Manager::IsActive(string username)
{
    ifstream in("Users.txt");

    if(!in)
        return false;

    string line, active, fileusername;

    while(getline(in, line))
    {
        stringstream ss(line);

        ss >> fileusername;

        if(fileusername == username)
        {
            for(int i = 0; i < 1; i++)
            {
                ss >> active;
                active.clear();
            }

            ss >> active; 
        
            if(stoi(active) == 1)
                return true;
            else
                return false;
        }
    }
    in.close();
}


void Manager::InActive(string username)
{
    if(!cliobj.UserExist(username))
    {
        cout<<"USER NOT FOUND";
        return;
    }

    if(IsInActive(username))
    {
        cout<<"USER WAS ALREADY INACTIVE";
        return;
    }

    string line, filepassword, fileusername, activation, role;
    ifstream inFile("Users.txt");
    ofstream outFile("tempUsers.txt");

    while(getline(inFile, line))
    {
        if(line.empty())
        {
            outFile<<line<<"\n";
            continue;
        }

        stringstream ss(line);

        ss >> fileusername;

        if(fileusername == username)
        {
            ss >> filepassword >> activation >> role;
            outFile<<fileusername<<" "<<filepassword<<" "<<"0"<<" "<<role<<endl;
        }
        else
        {
            outFile<<line<<endl;
        }
    }

    inFile.close();
    outFile.close();

    remove("Users.txt");
    rename("tempUsers.txt", "Users.txt");
}



bool Manager::IsInActive(string username)
{
    ifstream in("Users.txt");
    string line, active, fileusername;

    while(getline(in, line))
    {
        stringstream ss(line);

        ss >> fileusername;

        if(fileusername == username)
        {
            for(int i = 0; i < 1; i++)
            {
                ss >> active;
                active.clear();
            }

            ss >> active; 
        
            if(stoi(active) == 0)
                return true;
            else
                return false;
        }
    }
    in.close();
}





void Manager::StudentList(string activation)
{
    vector<string> usernames;
    ifstream inFile("Users.txt");
    string line, username, password, fileactivation, role;

    if(activation == "ACTIVE")
    {
        while(getline(inFile, line))
        {
            stringstream ss(line);
            ss >> username >> password >> fileactivation >> role;
        
            if(stoi(fileactivation) == 1 && role == "User")
            {
                usernames.push_back(username);
            }
        }
    }
    else if(activation == "DEACTIVE")
    {
        while(getline(inFile, line))
        {
            stringstream ss(line);
            ss >> username >> password >> fileactivation >> role;
        
            if(stoi(fileactivation) == 0 && role == "User")
            {
                usernames.push_back(username);
            }
        }
    }
    else if(activation == "EMPTY")
    {
        while(getline(inFile, line))
        {
            stringstream ss(line);
            ss >> username >> password >> fileactivation >> role;
            if(role == "User")
            {
                usernames.push_back(username);    
            }
        }
    }

    sort(usernames.begin(), usernames.end());

    for(const string &name : usernames)
    {
        cout<<name<<endl;
    }

    inFile.close();
}



Manager::Manager(){}



