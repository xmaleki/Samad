#include "Date.h"
#include <iostream>
#include <string>
#include<sstream>
#include<iomanip>

using namespace std;

Date::Date(string date)
{
    int values[3];
    string number;
    int idx {};

    for(const auto ch : date)
    {
        if(ch != '-')
        {
            number += ch;
        }
        else
        {
            values[idx] = stoi(number);
            idx++;
            number.clear();
        }
    }

    values[idx] = stoi(number);

    if(values[1] > 12 || values[2] > 31 || values[1] == 0)
    {
        cout<<"Invalid date, time ,input!";
        return;
    }

    this->year = values[0];
    this->month = values[1];
    this->day = values[2];
}


bool Date::operator==(const Date& other) const
{
    return (year == other.year && month == other.month && day == other.day);
}

bool Date::operator<(const Date& other) const
{
    if(year != other.year)
        return year < other.year;
    
    if(month != other.month)
        return month < other.month;

    return day < other.day;    
}


bool Date::isOlder(Date enddate)
{
    if(getyear() != enddate.getyear())
        return getyear() < enddate.getyear();

    if(getmonth() != enddate.getmonth())
        return getyear() < enddate.getyear();

    if(getday() != enddate.getday())
        return getday() < enddate.getday();
}


string Date::toString() const
{
    ostringstream oss;
    oss << year << "-" << setfill('0') << setw(2) << month << "-" <<setw(2) << day;
    
    return oss.str();
}


Date::Date(): day(1), month(1), year(2000)
{}




int Date::getyear()
{
    return year;
}

int Date::getmonth()
{
    return month;
}

int Date::getday()
{
    return day;
}

void Date::setyear(int yr)
{
    year = yr;
}

void Date::setmonth(int mn)
{
    month = mn;
}

void Date::setday(int dy)
{
    day = dy;
}

































