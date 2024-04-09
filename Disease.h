#pragma once
#include "Date.h"

using namespace std;

class Disease
{
private:
	Date begin;
	Date end;
	string name;


public:
	Disease();
	Disease(string disease);
	Disease(Date begin, Date end, string name) { this->begin = begin; this->end = end; this->name = name; }
	string getBeginDateString() { return begin.getDateString(); }
	string getEndDateString() { return end.getDateString(); }
	string getDiseaseString() const;
	string getDiseaseName() { return name; }

	friend ostream& operator<<(ostream& output, const Disease disease);
	friend istream& operator>>(istream& input, Disease disease);
};
