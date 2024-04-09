#define _CRT_SECURE_NO_WARNINGS
#include "Date.h"

Date::Date(){ day = month = year = 0; }

Date::Date(string date)
{
    if (!init(date))
        day = month = year = 0;
}

bool Date::init(string date)
{
    string array[3];
    size_t position = 0;
    int index = 0;
    bool isLeap = false;
    int day, month, year;
    while ((position = date.find('.')) != string::npos) {
        string token = date.substr(0, position);
        array[index] = token;
        date.erase(0, position + 1);
        index += 1;
    }
    if (index != 2) 
        return false;
    array[index] = date;
    day = atoi(array[0].c_str());
    month = atoi(array[1].c_str());
    year = atoi(array[2].c_str());

    if (!isCorrectDate(day, month, year))
        return false;
    this->day = day;
    this->month = month;
    this->year = year;
    return true;
}

void Date::initByConsole()
{
	string date;
	bool isInit = false;
	do
	{
		cout << "Ââåäèòå äàòó (ÄÄ.ÌÌ.ÃÃÃÃ) : ";
		cin >> date;
		isInit = init(date);
	} while (!isInit);
}

string Date::getDateString() const
{
    string date = "";
    date += '0' + day / 10;
    date += '0' + day % 10;
    date += '.';

    date += '0' + month / 10;
    date += '0' + month % 10;
    date += '.';

    date += '0' + year / 1000;
    date += '0' + year / 100 % 10;
    date += '0' + year / 10 % 10;
    date += '0' + year % 10;
    return date;
}

string Date::getCurrentDate()
{
    time_t curtime;
    struct tm* loctime;
    char buffer[12];
    time(&curtime);
    loctime = localtime(&curtime);
    strftime(buffer, 12, "%d.%m.%Y", loctime);
    string date = buffer;
    return date;
}

string Date::getEndOfReceptionsDate()
{
    Date date = Date(getCurrentDate());
    int days = 31;
    while (days--) ++date;
    return date.getDateString();
}

bool Date::isCorrectDate(int day, int month, int year)
{
    bool isLeap = false;
    if (year % 4 == 0) isLeap = true;
    if (year < 1970 || year > 2023) return false;
    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (month == 2 && day > 29 && isLeap == true) return false;
    if (month == 2 && day > 28 && isLeap == false) return false;
    if (month == 4 || month == 6 || month == 9 || month == 11) 
        if (day > 30) return false;
    return true;
}

const Date& operator++(Date& date)
{
    date.day++;
    if (!date.isCorrectDate())
    {
        date.month++;
        date.day = 1;
        if (date.month == 13)
        {
            date.month = 1;
            date.year++;
        }
    }
    return date;
}

bool operator==(const Date& date1, const Date& date2)
{
    return (date1.day == date2.day &&
        date1.month == date2.month &&
        date1.year == date2.year);
}

bool operator<=(const Date& date1, const Date& date2)
{
    if (date1.year > date2.year)
        return false;
    if (date1.year < date2.year)
        return true;
    if (date1.month > date2.month)
        return false;
    if (date1.month < date2.month)
        return true;
    if (date1.day > date2.day)
        return false;
    return true;
}

bool operator>=(const Date& date1, const Date& date2)
{
    if (date1.year < date2.year)
        return false;
    if (date1.year > date2.year)
        return true;
    if (date1.month < date2.month)
        return false;
    if (date1.month > date2.month)
        return true;
    if (date1.day < date2.day)
        return false;
    return true;
}
