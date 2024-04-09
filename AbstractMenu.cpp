#include "AbstractMenu.h"

string AbstractMenuOption::getLabel() const
{
    return label_;
}

void AbstractMenu::addFileHandler(shared_ptr<FileHandler> fileHandler)
{
    this->fileHandler = fileHandler;
}

bool AbstractMenu::authorize()
{
    string login, password;
    cout << "¬ведите логин: "; cin >> login;
    cout << "¬ведите пароль: ";
    password = User::getPassswordFromConsole();
    return isAuthorized_ = fileHandler->authorization(login, password);
}
