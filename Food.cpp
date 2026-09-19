#include "Food.h"
#include <string>
#include "Date.h"
#include "CliManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace std;


Food::Food()
{
    ifstream file("foods.txt");
    if (!file) 
        return;

    string line;
    while (getline(file, line)) 
    {
        if (line.empty()) continue; 

        stringstream ss(line);
        string dateStr;
        ss >> dateStr; 

       
        if (!dateStr.empty() && dateStr.back() == ':')
        {
            dateStr.pop_back();
        }

        try {
            Date currentDate(dateStr);
            string restOfLine;
            getline(ss, restOfLine);
            
            for (char& c : restOfLine)
            {
                if (c == ',') 
                    c = ' ';
            }

            stringstream foodStream(restOfLine);
            string token;
            
            while (foodStream >> token)
            {
                size_t colon1Pos = token.find(':');
                size_t colon2Pos = token.find(':', colon1Pos + 1);
                if ((colon1Pos != string::npos) && (colon2Pos != string::npos))
                {
                    string food = token.substr(0, colon1Pos);
                    int remaining = stoi(token.substr(colon1Pos + 1, colon2Pos));
                    int reserved = stoi(token.substr(colon2Pos + 1));
                    
                    if (remaining > 0)
                    {
                        FoodMap[currentDate][food].remaining += remaining;
                        FoodMap[currentDate][food].reserved += reserved; 
                    }
                }
            }
        } 
        catch (const exception& e) {
            continue;
        }
    }
    file.close();
}



void Food::View(Date startdate, Date enddate)
{
    if(!cliobj.isLoggedIn())
    {
        cout<<"YOU NEED TO LOGIN FIRST";
        return;
    }
    
    if(!startdate.isOlder(enddate))
    {
        cout<<"STARTDATE MUST BE BEFORE ENDDATE";
        return;
    }

    for (const auto& datePair : FoodMap) 
    {
        Date currentDate = datePair.first;

        if (!(currentDate < startdate) && !(enddate < currentDate)) 
        {
            bool hasFoodToDisplay = false;
            for (const auto& foodPair : datePair.second)
            {
                if (foodPair.second.remaining > 0)
                {
                    hasFoodToDisplay = true;
                    break;
                }
            }

            if (hasFoodToDisplay) 
            {
                cout << currentDate.toString() << ": ";
                
                bool isFirst = true;
                for (const auto& foodPair : datePair.second)
                {
                    if (foodPair.second.remaining > 0)
                    {
                        if (!isFirst)
                        {
                            cout << ", ";
                        }
                        cout << foodPair.first << ":" << foodPair.second.remaining;
                        isFirst = false;
                    }
                }
                cout << "\n";
            }
        }
    }


}




void Food::saveToFoodFile()
{
    ofstream file("foods.txt", ios::trunc);
    if (!file) return;

    for (const auto& datePair : FoodMap) 
    {
        bool hasFood = false;
        for (const auto& foodPair : datePair.second)
        {
            if (foodPair.second.remaining > 0)
            {
                hasFood = true;
                break;
            }
        }

        if(hasFood)
        {
            file << datePair.first.toString() << ": ";
            
            bool isFirst = true;
            for (const auto& foodPair : datePair.second)
            {
                if (foodPair.second.remaining > 0)
                {
                    if (!isFirst)
                    {
                        file << ",";
                    }
                    file << foodPair.first << ":" << foodPair.second.remaining << ":" << foodPair.second.reserved;
                    isFirst = false;
                }
            }
            file << "\n";
        }
    }
    file.close();

}



bool Food::ReserveCheck(Date date, string food)
{
    auto it = FoodMap.find(date);

    if(it == FoodMap.end())
    {
        return false;
    }

    auto& innerFoodMap = it->second;

    auto foodIterator = innerFoodMap.find(food);

    if(foodIterator == innerFoodMap.end())
        return false;

    return true;
}




bool Food::ReservationUsernameCheck()
{
    string line1, fusername;
    
    ifstream inreserv("reservations.txt");

    if(!inreserv.is_open())
        return false;

    while(getline(inreserv, line1))
    {
        stringstream ss(line1);
        ss >> fusername;

        if(cliobj.GetUsername() == fusername)
            return true;
    }

    return false;
}




void Food::Reserve(Date date, string food)
{
     if(!cliobj.isLoggedIn())
    {
        cout<<"YOU NEED TO LOGIN FIRST";
        return;
    }

    if(!ReserveCheck(date, food))
    {
        cout<<"SELECTED FOOD WAS NOT SERVED";
        return;
    }

    if(ReservationInDay(date))
    {
        cout<<"RESERVATION ALREADY EXISTS FOR THIS DATE";
        return;
    }

    FoodMap[date][food].remaining--;
    FoodMap[date][food].reserved++;
    saveToFoodFile();

    if(!ReservationUsernameCheck())
    {
        ofstream out("reservations.txt" , ios::app);
        out<<cliobj.GetUsername()<<" "<<food<<":"<<date.toString()<<endl;
        out.close();
    }
    else
    {
        addFoodDateToReservation(food, date);
    }
    
    cout<<"SUCCESSFULLY RESERVED";
}




void Food::addFoodDateToReservation(string food, Date date)
{
    ifstream inFile("reservations.txt");
    ofstream outFile("tempreservations.txt");

    string lineres;

    while(getline(inFile, lineres))
    {
        if(lineres.empty())
        {
            outFile<<lineres<<"\n";
            continue;
        }

        stringstream ss(lineres);
        string currentUsername;
        ss >> currentUsername;

        if(currentUsername == cliobj.GetUsername())
        {
            outFile<<lineres<<" "<<food<<":"<<date.toString()<<endl;
        }
        else
        {
            outFile<<lineres<<endl;
        }
    }

    inFile.close();
    outFile.close();

    remove("reservations.txt");
    rename("tempreservations.txt", "reservations.txt");
}




bool Food::ReservationInDay(Date date)
{
    ifstream inFile("reservations.txt");
    
    if(!inFile.is_open())
        return false;

    string line, fusername, ffooddate, filefood, filedate;

    while(getline(inFile, line))
    {
        if(line.empty())
            continue;

        stringstream ss(line);

        ss >> fusername;

        while(ss >> ffooddate)
        {
            int pos = ffooddate.find(':');

            if (pos == string::npos)
                continue;

            filefood = ffooddate.substr(0, pos);
            filedate = ffooddate.substr(pos + 1);

            if(fusername == cliobj.GetUsername() && filedate == date.toString())
            {
                inFile.close();
                return true;
            }
        }                
    }

    inFile.close();
    return false;
}



Date Food::StringToDate(string date)
{
    Date dateobj;
    string number;
    int num[3], i = 0;
    

    for(auto ch : date)
    {
        number += ch;

        if(ch == '-')
        {
            num[i] = stoi(number);
            i++;
            number.clear();
        }
    }

    num[i] = stoi(number);

    dateobj.setyear(num[0]);
    dateobj.setmonth(num[1]);
    dateobj.setday(num[2]);

    return dateobj;
}



void Food::AddFood(string food, int amount, Date date)
{
    if(amount <= 0)
    {
        cout<<"AMOUNT SHOULD BE BIGGER THAN 0";
        return;
    }

    FoodMap[date][food].remaining += amount;
    saveToFoodFile();
}



void Food::RemoveFood(string food, Date date)
{
    auto it = FoodMap.find(date);

    if(it == FoodMap.end())
    {
        cout<<"FOOD NOT FOUND IN SELECTED DATE";
        return;
    }

    auto& innerMap = it->second;

    auto fooditerator = innerMap.find(food);

    if(fooditerator == innerMap.end())
    {
        cout<<"FOOD NOT FOUND IN SELECTED DATE";
        return;
    }


    if(CheckReservation(food, date))
    {
        cout<<"FOOD IS RESERVED AND CAN'T BE REMOVED";
        return;
    }
    
    if(it != FoodMap.end())
    {
        it->second.erase(food);

        if(it->second.empty())
        {
            FoodMap.erase(it);
        }
    }

    saveToFoodFile();
}



bool Food::CheckReservation(string food, Date date)
{
    ifstream inreserve("reservations.txt");

    if(!inreserve.is_open())
        return false;

    string line, fusername, ffooddate, filefood, filedate;

    while(getline(inreserve, line))
    {
        if(line.empty())
            continue;

        stringstream ss(line);

        ss >> fusername;

        while(ss >> ffooddate)
        {
            int pos = ffooddate.find(':');

            filefood = ffooddate.substr(0, pos);
            filedate = ffooddate.substr(pos + 1);

            if(filefood == food && filedate == date.toString())
            {
                inreserve.close();
                return true;
             }
        }                
    }

    inreserve.close();
    return false;
}






void Food::ReserveReport(Date startdate, Date enddate)
{
    if(enddate < startdate)
    {
        cout<<"STARTDATE MUST BE BEFORE ENDDATE";
        return;
    }

    for (const auto& datePair : FoodMap) 
    {
        Date currentDate = datePair.first;

        if (!(currentDate < startdate) && !(enddate < currentDate)) 
        {
            bool hasFoodToDisplay = false;
            for (const auto& foodPair : datePair.second)
            {
                if (foodPair.second.remaining > 0)
                {
                    hasFoodToDisplay = true;
                    break;
                }
            }

            if (hasFoodToDisplay) 
            {
                cout << currentDate.toString() << ": ";
                
                bool isFirst = true;
                for (const auto& foodPair : datePair.second)
                {
                    if (foodPair.second.remaining > 0)
                    {
                        if (!isFirst)
                        {
                            cout << ", ";
                        }
                        cout << foodPair.first << ":" << foodPair.second.remaining << " " << foodPair.second.reserved;
                        isFirst = false;
                    }
                }
                cout << "\n";
            }
        }
    }

}





































