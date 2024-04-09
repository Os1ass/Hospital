#pragma once
#include "AdminFileHandler.h"
#include "Doctor.h"

class DoctorFileHandler : public AdminFileHandler
{
private:

	void initUsersList() override;

public:
	DoctorFileHandler() : AdminFileHandler() {}
	DoctorFileHandler(string filename);
	DoctorFileHandler(string filename, int userId) : DoctorFileHandler(filename) { authorizedUserId = userId; }

	void printUsers();
	void editSpecialization(string specialization);
	vector<Doctor*> getUsersList();
};