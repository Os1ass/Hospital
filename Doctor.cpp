#include "Doctor.h"

void Doctor::registration()
{
	Admin::registration();
	cout << "Введите специализацию: ";
	if (cin.peek() == '\n')
		cin.get();
	getline(cin, specialization);
}

void Doctor::print()
{
	Admin::print();
	cout << "Специализация: " << specialization << endl;
}

void Doctor::tablePrint(int index)
{
	cout << (index < 9 ? "|  " : "| ") << index + 1 << " | " << setw(17) << name
		<< " | " << setw(9) << login
		<< " |   " << birthDate.getDateString()
		<< "  | " << setw(7) << sex
		<< " | " << setw(16) << phoneNumber
		<< " | " << setw(13) << specialization << " |\n";
	cout << "+----+-------------------+-----------+---------------+---------+------------------+---------------+\n";
}

void Doctor::printReceptions()
{
	if (receptions.size() == 0)
	{
		cout << "Нет записей на прием\n";
		return;
	}
	sort(receptions.begin(), receptions.end(),
		[](Reception const* const a, Reception const* const b) {
			if (a->getDate() == b->getDate())
				return a->getReceptionHour() < b->getReceptionHour();
			return a->getDate() <= b->getDate();
		});
	//cout << "Прошедшие приемы:\n\n";
	Reception::printTableHeader();
	for (int i = 0; i < receptions.size(); i++)
	{
		if (receptions[i]->isDone())
		{
			receptions[i]->tablePrint(i);
			//cout << "| " << receptions[i]->getDate().getDateString()
			//	<< (receptions[i]->getReceptionHour() < 10 ? " 0" : " ") << receptions[i]->getReceptionHour()
			//	<< ":00  | " << setw(40) << receptions[i]->getComplaint()
			//	<< " | " << setw(25) << receptions[i]->getConclusion() << " |\n";
			////receptions[i]->print();
			////cout << endl;
			//cout << "+-------------------+------------------------------------------+---------------------------+\n";
		}
	}
	/*cout << "\nБудущие приемы:\n\n";
	cout
		<< "+-------------------+------------------------------------------+\n"
		<< "|    Дата приема    |            Причина приема : ФИО пациента |\n"
		<< "+-------------------+------------------------------------------+\n";*/
	for (int i = 0; i < receptions.size(); i++)
	{
		if (!receptions[i]->isDone())
			receptions[i]->tablePrint(i);
	}
}

vector<Reception*> Doctor::getReceptionsToDo()
{
	vector<Reception*> ret;
	for (int i = 0; i < receptions.size(); i++)
	{
		if (!receptions[i]->isDone())
			ret.emplace_back(receptions[i]);
	}
	return ret;
}

Reception* Doctor::getReception(Reception* reception)
{
	for (auto curReception : receptions)
		if (curReception->isEqual(reception))
			return curReception;
	return nullptr;
}

bool Doctor::isPatientOnReception(int userId)
{
	for (auto reception : receptions)
		if (reception->getPatientId() == userId)
			return true;
	return false;
}

bool Doctor::isDateFree(Date date, int receptionHour)
{
	for (auto reception : receptions)
	{
		if (reception->isDone())
			continue;
		if (reception->getReceptionHour() == receptionHour &&
			reception->getDate() == date)
			return false;
	}
	return true;
}

Date Doctor::getClosestFreeDate(Date curDate, Date endOfReceptionsDate, int receptionHour)
{
	while (true)
	{
		bool isDupl = false;
		for (auto reception : receptions)
		{
			if (reception->isDone())
				continue;
			if (reception->getReceptionHour() == receptionHour &&
				reception->getDate() == curDate)
			{
				++curDate;
				isDupl = true;
				break;
			}
		}
		if (!isDupl)
			return curDate;
	}
	if (curDate <= endOfReceptionsDate)
		return curDate;
	return Date();
}

ostream& Doctor::getDataStream(ostream& output) const
{
	User::getDataStream(output);
	output << specialization << '|';
	for (int i = 0; i < receptions.size(); i++)
	{
		output << receptions[i];
		if (i != receptions.size() - 1)
			output << '/';
	}
	output << '|';
	return output;
}

istream& Doctor::setDataFromStream(istream& input)
{
	User::setDataFromStream(input);
	getline(input, specialization, '|');
	string allReceptions;
	getline(input, allReceptions, '|');
	stringstream receptionsStream;
	receptionsStream << allReceptions;
	string reception;
	while (getline(receptionsStream, reception, '/'))
		receptions.emplace_back(new Reception(reception));
	return input;
}

void Doctor::printTableHeader()
{
	cout << endl
		<< "+----+-------------------+-----------+---------------+---------+------------------+---------------+\n"
		<< "|  № |        ФИО        |   Логин   | Дата рождения |   Пол   | Телефонный номер | Специализация |\n"
		<< "+----+-------------------+-----------+---------------+---------+------------------+---------------+\n";
}
