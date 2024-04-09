#include "PatientFileHandler.h"

void PatientFileHandler::initUsersList()
{
	if (!checkFile())
		return;

	ifstream file(filename);
	string line;
	stringstream lineStream;
	while (getline(file, line))
	{
		lineStream << line;
		User* curUser = new Patient();
		lineStream >> curUser;
		lineStream.clear();
		usersList.push_back(curUser);
	}
	file.close();
}

vector<User*> PatientFileHandler::searchPatientsByDisease(string disease)
{
	vector<User*> res;
	for (auto user : usersList)
		if (static_cast<Patient*>(user)->searchDisease(disease))
			res.emplace_back(user);
	return res;
}