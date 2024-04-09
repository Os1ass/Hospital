#include "Reception.h"

Reception::Reception()
{
	date = Date();
	doctorId = 0;
	patientId = 0;
	receptionHour = 0;
}

Reception::Reception(string date, int receptionHour, int doctorId, int patientId, string patientComplaint)
{
	this->date = Date(date);
	this->receptionHour = receptionHour;
	this->doctorId = doctorId;
	this->patientId = patientId;
	this->patientComplaint = patientComplaint;
}

Reception::Reception(string reception)
{
	patientId = doctorId = receptionHour = -1;
	this->date = Date();
	string date, id, comment;
	int i = 0;
	while (i < reception.length())
	{
		if (reception[i] == '-')
			break;
		date += reception[i];
		i++;
	}
	this->date = Date(date);
	if (this->date.day == 0)
		return;
	date.clear(); i++;

	while (i < reception.length())
	{
		if (reception[i] == '-')
			break;
		date += reception[i];
		i++;
	}
	receptionHour = stoi(date);
	if (date.empty())
		return;
	i++;

	while (i < reception.length())
	{
		if (reception[i] == '-')
			break;
		id += reception[i];
		i++;
	}
	doctorId = stoi(id);
	if (id.empty())
		return;
	id.clear(); i++;

	while (i < reception.length())
	{
		if (reception[i] == '-')
			break;
		id += reception[i];
		i++;
	}
	patientId = stoi(id);
	if (id.empty())
		return;
	comment.clear(); i++;

	while (i < reception.length())
	{
		if (reception[i] == '-')
			break;
		comment += reception[i];
		i++;
	}
	patientComplaint = comment;
	if (comment.empty())
		return;
	comment.clear(); i++;

	while (i < reception.length())
	{
		comment += reception[i];
		i++;
	}
	doctorConclusion = comment;
	if (comment.empty())
		done = false;
	else
		done = true;
}

bool Reception::isEqual(Reception* other)
{
	return (date.getDateString() == other->date.getDateString() &&
		receptionHour == other->receptionHour &&
		doctorId == other->doctorId &&
		patientId == other->patientId &&
		patientComplaint == other->patientComplaint &&
		doctorConclusion == other->doctorConclusion);
}

string Reception::getReceptionString() const
{
	return date.getDateString() +
		'-' + to_string(receptionHour) + 
		'-' + to_string(doctorId) +
		'-' + to_string(patientId) +
		'-' + patientComplaint + '-' + doctorConclusion;
}

void Reception::makeReception(string doctorConclusion)
{
	if (done)
		return;
	done = true;
	this->doctorConclusion = doctorConclusion;
}

void Reception::print()
{
	cout << "Дата: " << date.getDateString() 
		<< (receptionHour == 9 ? " 0" : " ") << receptionHour << ":00";
	cout << "\nЖалоба пациента: " << patientComplaint;
	if (done)
		cout << "\nЗаключение доктора: " << doctorConclusion;
	else
		cout << "\nПрием еще не прошел";
	cout << endl;
}

void Reception::printTableHeader()
{
	cout
		<< "+----+-------------------+------------------------------------------+---------------------------+\n"
		<< "|  № |    Дата приема    |            Причина приема : ФИО пациента |        Заключение доктора |\n"
		<< "+----+-------------------+------------------------------------------+---------------------------+\n";
}

void Reception::tablePrint(int index)
{
	cout << (index < 9 ? "|  " : "| ") << index + 1 << " | " << date.getDateString()
		<< (receptionHour < 10 ? " 0" : " ") << receptionHour << ":00 "
		<< " | " << setw(40) << patientComplaint
		<< " | " << setw(25) << (done ? doctorConclusion : "Прием еще не прошел") << " |\n";
	cout << "+----+-------------------+------------------------------------------+---------------------------+\n";
}

ostream& operator<<(ostream& output, const Reception* reception)
{
	output << reception->getReceptionString();
	return output;
}

istream& operator>>(istream& input, Reception* reception)
{
	string date;
	if (!getline(input, date, '-')) {
		input.setstate(ios::failbit);
		return input;
	}
	reception->date = Date(date);
	getline(input, date, '-');
	reception->receptionHour = stoi(date);
	string id;
	getline(input, id, '-');
	reception->doctorId = stoi(id);
	getline(input, id, '-');
	reception->patientId = stoi(id);
	getline(input, reception->patientComplaint, '-');
	getline(input, reception->doctorConclusion);

	return input;
}
