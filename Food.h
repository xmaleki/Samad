#pragma once
#include <string>
#include "Food.h"
#include "Date.h"
#include "CliManager.h"
#include "Food.h"
#include<map>

using namespace std;

class Food
{
    private:
        CliManager cliobj;
        Date dateobj;
        
        struct FoodStock
        {
            int remaining = 0;
            int reserved = 0;
        };

        map<Date, map<string, FoodStock>> FoodMap;


    public:
        Food();
        void View(Date startdate, Date enddate);
        void saveToFoodFile();
        void Reserve(Date date, string food);
        bool ReservationUsernameCheck();
        bool ReserveCheck(Date date, string food);
        void addFoodDateToReservation(string food, Date date);
        bool ReservationInDay(Date date);
        Date StringToDate(string date);
        void AddFood(string food, int amount, Date date);
        void RemoveFood(string food, Date date);
        bool CheckReservation(string food, Date date);
        void ReserveReport(Date startdate, Date enddate);



};




















