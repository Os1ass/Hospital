#include "DoctorFileHandler.h"

void DoctorFileHandler::initUsersList()
{
	if (!checkFile())
		return;

	ifstream file(filename);
	string line;
	stringstream lineStream;
	while (getline(file, line))
	{
		lineStream << line;
		User* curUser = new Doctor();
		lineStream >> curUser;
		lineStream.clear();
		usersList.push_back(curUser);
	}
	file.close();
}

DoctorFileHandler::DoctorFileHandler(string filename)
{
	this->filename = filename;
	this->filename.append(".data");
	initUsersList();
}

void DoctorFileHandler::printUsers()
{
	if (usersList.size() == 0)
	{
		cout << "Нет учетных записей\n";
		return;
	}
	sort(usersList.begin(), usersList.end(),
		[](User const* const a, User const* const b) { return *a < *b; });
	Doctor::printTableHeader();
	int index = 0;
	for (auto user : usersList)
	{
		user->tablePrint(index);
		index++;
	}
}

void DoctorFileHandler::editSpecialization(string specialization)
{
	static_cast<Doctor*>(getAuthorizedUser())->setSpecialization(specialization);
}

vector<Doctor*> DoctorFileHandler::getUsersList()
{
	vector<Doctor*> doctors;
	auto users = FileHandler::getUsersList();
	for (auto user : users)
		doctors.emplace_back(static_cast<Doctor*>(user));
	return doctors;
}