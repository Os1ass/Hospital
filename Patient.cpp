#include "Patient.h"

Patient::Patient() : User()
{
}

void Patient::printDiseasesHistory()
{
	if (diseasesHistory.size() == 0)
	{
		cout << "История болезни пуста\n";
		return;
	}
	cout << "История болезни " + name + ":\n\n";
	cout 
		<< "+----+---------------------+--------------------+---------------------------+\n"
		<< "|  № | Дата начала болезни | Дата конца болезни |          Название болезни |\n"
		<< "+----+---------------------+--------------------+---------------------------+\n";
	for (int i = 0; i < diseasesHistory.size(); i++)
	{
		cout << (i < 9 ? "|  " : "| ") << i + 1 << " |      " << diseasesHistory[i].getBeginDateString()
			<< "     |     " << diseasesHistory[i].getEndDateString()
			<< "     | " << setw(25) << diseasesHistory[i].getDiseaseName() << " |\n";
		//cout << diseasesHistory[i].getDiseaseString() << endl;
		cout << "+----+---------------------+--------------------+---------------------------+\n";
	}
}

void Patient::printReceptions()
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

Reception* Patient::getReception(Reception* reception)
{
	for (auto curReception : receptions)
		if (curReception->isEqual(reception))
			return curReception;
	return nullptr;
}

void Patient::print()
{
	User::print();
	//printDiseasesHistory();
}

ostream& Patient::getDataStream(ostream& output) const
{
	User::getDataStream(output);
	for (int i = 0; i < diseasesHistory.size(); i++)
	{
		output << diseasesHistory[i];
		if (i != diseasesHistory.size() - 1)
			output << '/';
	}
	output << '|';
	for (int i = 0; i < receptions.size(); i++)
	{
		output << receptions[i];
		if (i != receptions.size() - 1)
			output << '/';
	}
	output << '|';
	return output;
}

istream& Patient::setDataFromStream(istream& input)
{
	User::setDataFromStream(input);
	string allDiseases;
	getline(input, allDiseases, '|');
	stringstream diseasesStream;
	diseasesStream << allDiseases;
	string disease;
	while (getline(diseasesStream, disease, '/'))
		diseasesHistory.emplace_back(Disease(disease));
	string allReceptions;
	getline(input, allReceptions, '|');
	stringstream receptionsStream;
	receptionsStream << allReceptions;
	string reception;
	while (getline(receptionsStream, reception, '/'))
		receptions.emplace_back(new Reception(reception));
	return input;
}

bool Patient::searchDisease(string diseaseName)
{
	for (int i = 0; i < diseasesHistory.size(); i++)
		if (equals(diseasesHistory[i].getDiseaseName(), diseaseName) == 0)
			return true;
	return false;
}
