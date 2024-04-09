#pragma once
#include "User.h"
#include "Reception.h"
#include "Disease.h"
#include "FileHandler.h"

class Patient : public User
{
public:
	Patient();

	virtual void registration() { User::registration(); }
	virtual void print();
	virtual void printDiseasesHistory() override;
	virtual void printReceptions() override;
	virtual Reception* getReception(Reception* reception);
	virtual ostream& getDataStream(ostream& output) const;
	virtual istream& setDataFromStream(istream& input);
	bool searchDisease(string diseaseName);
	void addReception(Reception* reception) { receptions.emplace_back(reception); }
	void addDisease(Disease disease) { diseasesHistory.emplace_back(disease); }


private:
	vector<Disease> diseasesHistory;
	vector<Reception*> receptions;
};