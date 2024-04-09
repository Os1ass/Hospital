#pragma once
#include "User.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

int equals(string str1, string str2);

class FileHandler
{
protected:		
	int authorizedUserId;
	string filename;
	vector<User*> usersList;

	bool checkFile();
	int getFileSize();
	virtual void initUsersList() {}

public:
	FileHandler() { authorizedUserId = 0; }
	FileHandler(string filename);
	FileHandler(string filename, int userId);
	
	vector<User*> getUsersList();

	void add(User* user);
	bool remove(int id);
	void save();
	void editLogin(string login) { getAuthorizedUser()->setLogin(login); }
	void editPassword(string password) { getAuthorizedUser()->setPassword(password); }

	bool isRegistred(int id);
	bool isRegistred(string login);
	bool isAuthorized();
	vector<User*> searchUsersByName(string name);
	User* getUserById(int id);
	User* getUserByPhoneNumber(string phoneNumber);
	User* getAuthorizedUser();
	User* getUserByLogin(string login);
	bool authorization(string login, string password);

	void printAuthorizedUser();
	virtual void printUsers();
	/*void registration()
	{
		T newUser();
		newUser.registration();
	}*/

	~FileHandler() { save(); }
};