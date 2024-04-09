#include "SearchMenu.h"

SearchMenuOption::SearchMenuOption(const string& label, function<User* (shared_ptr<FileHandler>)> func)
{
	label_ = label;
	func_ = func;
}

User* SearchMenuOption::execute(shared_ptr<FileHandler> fileHandler)
{
	return func_(fileHandler);
}

void SearchMenu::addOption(const SearchMenuOption& option)
{
	options_.emplace_back(option);
}

void SearchMenu::run()
{
    if (!isAuthorized_) {
        auto authResult = authorize();
        if (!authResult) {
            return;
        }
    }

    system("cls");
    for (int i = 0; i < options_.size(); ++i) {
        cout << i + 1 << ". " << options_[i].getLabel() << endl;
    }

    cout << options_.size() + 1 << ". Выход\n";

    cout << "Введите Ваш выбор: ";
    int choice;
    cin >> choice;
    if (choice > 0 && choice <= options_.size()) {
        srchUser = options_[choice - 1].execute(fileHandler);
    }
}