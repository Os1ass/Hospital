#pragma once
#include <string>
#include <conio.h>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "Date.h"

//#include "Menu.h"
using namespace std;

class User
{
friend class FileHandler;
//friend Menu* createAdminMenu();
public:
	User() { id = rand(); }
	User(User* other);
	//bool compareTo(User* other);
	//void initByConsole();
	int getId();

	virtual void registration();
	virtual void print();
	virtual void tablePrint(int index);
	virtual void printDiseasesHistory() = 0;
	virtual void printReceptions() = 0;
	
	string getName() { return name; }
	string getSex() { return sex; }
	string getPhoneNumber() { return phoneNumber; }
	Date getBirthDate() { return birthDate; }

	void setName(string name) { this->name = name; }
	void setSex(string sex) { this->sex = sex; }
	void setPhoneNumber(string phoneNumber) { this->phoneNumber = phoneNumber; }
	void setBirthDate(Date birthDate) { this->birthDate = birthDate; }

	virtual ostream& getDataStream(ostream& output) const;
	virtual istream& setDataFromStream(istream& input);

	static void printTableHeader();
	static string getPassswordFromConsole();
	static string readPassword();

	friend bool operator<(const User& left, const User& right);

protected:
	int id;
	string login;
	string password;
	string name;
	string sex;
	string phoneNumber;
	Date birthDate;

	void setLogin(string login) { this->login = login; }
	void setPassword(string password) { this->login = login; }

};

ostream& operator<<(ostream& output, const User* user);
istream& operator>>(istream& input, User* user);