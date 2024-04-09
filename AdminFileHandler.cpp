#include "AdminFileHandler.h"

void AdminFileHandler::initUsersList()
{
	if (!checkFile())
		return;

	ifstream file(filename);
	string line;
	stringstream lineStream;
	while (getline(file, line))
	{
		lineStream << line;
		User* curUser = new Admin();
		lineStream >> curUser;
		lineStream.clear();
		usersList.push_back(curUser);
	}
	file.close();
}
