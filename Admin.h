#pragma once
#include "User.h"

class Admin : public User
{
public:
	Admin() : User() {}
	virtual void registration() { User::registration(); };
	virtual void print() { User::print(); };
	virtual void tablePrint(int index) { User::tablePrint(index); }
	void printDiseasesHistory() {};
	void printReceptions() {};

protected:

};