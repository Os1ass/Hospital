#pragma once
#include "FileHandler.h"
#include "Admin.h"

class AdminFileHandler : public FileHandler
{
protected:

	virtual void initUsersList() override;

public:
	AdminFileHandler() : FileHandler() {}
	AdminFileHandler(string filename) 
		: FileHandler(filename) { initUsersList(); }
	AdminFileHandler(string filename, int userId) 
		: FileHandler(filename, userId) { initUsersList(); }

	virtual void printUsers() {};
};