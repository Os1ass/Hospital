#include "User.h"

void User::printTableHeader()
{
	cout << endl
		<< "+----+-------------------+-----------+---------------+---------+------------------+\n"
		<< "|  № |        ФИО        |   Логин   | Дата рождения |   Пол   | Телефонный номер |\n"
		<< "+----+-------------------+-----------+---------------+---------+------------------+\n";
}

string User::getPassswordFromConsole()
{
	string password;
	char curSymb;
	int length = 0;
	while ((curSymb = _getch()) != '\r')
	{
		if (curSymb == '\b')
		{
			if (length > 0)
			{
				cout << "\b \b";
				password.erase(--password.end());
				length--;
			}
			continue;
		}
		length++;
		cout << '*';
		password += curSymb;
	}
	cout << endl;
	return password;
}

string User::readPassword()
{
	string firstPassword, secondPassword;
	while (true)
	{
		cout << "Введите пароль  : ";
		firstPassword = getPassswordFromConsole();
		cout << "Повторите пароль: ";
		secondPassword = getPassswordFromConsole();
		if (firstPassword == secondPassword)
			return firstPassword;
		cout << "Пароли не совпадают. Попробуйте еще\n";
	}
}

User::User(User* other)
{
	id = other->id;
	login = other->login;
	password = other->password;
	name = other->name;
	sex = other->sex;
	phoneNumber = other->phoneNumber;
	birthDate = Date(other->birthDate.getDateString());
}

int User::getId()
{
	return id;
}

void User::registration()
{
	cout << "Введите логин: "; cin >> login;
	password = readPassword();
	cout << "Введите ФИО: ";
	if (cin.peek() == '\n')
		cin.get();
	getline(cin, name);
	birthDate.initByConsole();
	cout << "Введите пол: "; cin >> sex;
	cout << "Введите номер: "; cin >> phoneNumber;
}

void User::print()
{
	//cout << "Id: " << id << endl;
	cout << "ФИО: " << name << endl;
	cout << "Логин: " << login << endl;
	//cout << "Password: " << password << endl;
	cout << "Дата рождения: " << birthDate.getDateString() << endl;
	cout << "Пол: " << sex << endl;
	cout << "Телефонный номер: " << phoneNumber << endl;
}

void User::tablePrint(int index)
{
		cout << (index < 9 ? "|  " : "| ") << index + 1 << " | " << setw(17) << name
		<< " | " << setw(9) << login
		<< " |   " << birthDate.getDateString()
		<< "  | " << setw(7) << sex
		<< " | " << setw(16) << phoneNumber << " |\n";
	cout << "+----+-------------------+-----------+---------------+---------+------------------+\n";

}

ostream& User::getDataStream(ostream& output) const
{
	output << id << '.' << name << '|' << login << '|' << password << '|'
		<< phoneNumber << '|' << sex << '|' << birthDate.getDateString() << '|';
	return output;
}

istream& User::setDataFromStream(istream& input)
{
	string id;
	if (!getline(input, id, '.')) {
		input.setstate(ios::failbit);
		return input;
	}
	this->id = stoi(id);
	getline(input, name, '|');
	getline(input, login, '|');
	getline(input, password, '|');
	getline(input, phoneNumber, '|');
	getline(input, sex, '|');
	string birthDate;
	getline(input, birthDate, '|');
	this->birthDate = Date(birthDate);

	return input;
}

/*void User::initByConsole()
{
}*/

bool operator<(const User& left, const User& right)
{
	return left.name < right.name;
}

ostream& operator<<(ostream& output, const User* user)
{
	return user->getDataStream(output);
}

istream& operator>>(istream& input, User* user)
{
	return user->setDataFromStream(input);
}
