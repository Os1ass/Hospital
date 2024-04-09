#include "FileHandler.h"

int equals(string str1, string str2)
{
	transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
	transform(str2.begin(), str2.end(), str2.begin(), ::toupper);

	if (str1 < str2)
		return -1;

	if (str1 > str2)
		return 1;
	else
		return 0;
}

bool FileHandler::checkFile()
{
	if (filename.empty())
		return false;

	fstream file(filename);
	if (!file.is_open())
	{
		cout << "Ошибка! Файл не может быть открыт\n";
		return false;
	}
	return true;
}

int FileHandler::getFileSize()
{
	if (!checkFile())
		return 0;

	ifstream f;
	f.open(filename, ios_base::binary);
	f.seekg(0, ios_base::end);
	int size = f.tellg();
	f.close();

	return size;
}

FileHandler::FileHandler(string filename) : FileHandler()
{
	this->filename = filename;
	this->filename.append(".data");
}

FileHandler::FileHandler(string filename, int userId) : FileHandler(filename)
{
	authorizedUserId = userId;
}

void FileHandler::add(User* user)
{
	usersList.emplace_back(user);
}

vector<User*> FileHandler::getUsersList()
{
	return usersList;
}

bool FileHandler::remove(int id)
{
	auto user = usersList.begin();
	while (user != usersList.end())
	{
		if ((*user)->getId() == id)
		{
			usersList.erase(user);
			return true;
		}
		user++;
	}
	return false;
}

void FileHandler::save()
{
	if (!checkFile())
		return;

	ofstream file(filename, ios::trunc);
	for (auto user : usersList)
		file << user << endl;
	file.close();
}

vector<User*> FileHandler::searchUsersByName(string name)
{
	vector<User*> res;
	for (auto user : usersList)
		if (equals(user->name, name) == 0)
			res.emplace_back(user);
	return res;
}

User* FileHandler::getUserByLogin(string login)
{
	for (auto user : usersList)
	{
		if (equals(user->login, login) == 0)
			return user;
	}
	return nullptr;
}

User* FileHandler::getUserById(int id)
{
	for (auto user : usersList)
	{
		if (user->id == id)
			return user;
	}
	return nullptr;
}

User* FileHandler::getUserByPhoneNumber(string phoneNumber)
{
	for (auto user : usersList)
	{
		if (equals(user->phoneNumber, phoneNumber) == 0)
			return user;
	}
	return nullptr;
}

User* FileHandler::getAuthorizedUser()
{
	return getUserById(authorizedUserId);
}

bool FileHandler::isRegistred(int id)
{
	for (auto user : usersList)
		if (user->id == id)
			return true;
	return false;
}

bool FileHandler::isRegistred(string login)
{
	for (auto user : usersList)
		if (user->login == login)
			return true;
	return false;
}

bool FileHandler::isAuthorized()
{
	return authorizedUserId != 0;
}

bool FileHandler::authorization(string login, string password)
{
	if (!isRegistred(login))
		return false;

	User* user = getUserByLogin(login);
	if (user->password == password)
		authorizedUserId = user->id;
	else
		return false;
	return true;
}

void FileHandler::printAuthorizedUser()
{
	User* user = getUserById(authorizedUserId);
	user->print();
}

void FileHandler::printUsers()
{
	if (usersList.size() == 0)
	{
		cout << "Нет учетных записей\n";
		return;
	}
	sort(usersList.begin(), usersList.end(), 
		[](User const* const a, User const* const b) { return *a < *b; });
	User::printTableHeader();
	int index = 0;
	for (auto user : usersList)
	{
		user->tablePrint(index);
		index++;
		//cout << endl;
	}
}