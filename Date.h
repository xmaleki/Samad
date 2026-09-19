#pragma once
#include<string>

using namespace std;

class Date
{
    private:
        int year, month, day;

    public:
        explicit Date(string date);
        Date();
        bool operator==(const Date &other) const;
        bool operator<(const Date &other) const;
        bool isOlder(Date);
        string toString() const;
        int getyear();
        int getmonth();
        int getday();
        void setyear(int yr);
        void setmonth(int mn);
        void setday(int dy);
};

















