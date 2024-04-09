#pragma once
#include "FileHandler.h"
#include "Patient.h"

class PatientFileHandler : public FileHandler
{
private:
	virtual void initUsersList() override;

public:
	PatientFileHandler() : FileHandler() {};
	PatientFileHandler(string filename) 
		: FileHandler(filename) { initUsersList(); }
	PatientFileHandler(string filename, int userId)
		: FileHandler(filename, userId) { initUsersList(); }
	vector<User*> searchPatientsByDisease(string disease);
};