#pragma once
#include "Admin.h"
#include "Reception.h"

class Doctor : public Admin
{
public:
	Doctor() : Admin() {}
	void registration();
	void print();
	void tablePrint(int index);
	void printReceptions() override;
	void addReception(Reception* reception) { receptions.emplace_back(reception); }

	string getSpecialization() { return specialization; }
	vector<Reception*> getReceptionsToDo();	
	Reception* getReception(Reception* reception);
	void setSpecialization(string specialization) { this->specialization = specialization; }
	bool isPatientOnReception(int userId);
	bool isDateFree(Date date, int receptionHour);

	Date getClosestFreeDate(Date curDate, Date endOfReceptionsDate, int receptionHour);
	ostream& getDataStream(ostream& output) const;
	istream& setDataFromStream(istream& input);

	static void printTableHeader();

private:
	vector<Reception*> receptions;
	string specialization;
};
