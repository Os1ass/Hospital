#pragma once
#include "SearchMenu.h"

using namespace std;

class MenuOption : public AbstractMenuOption
{
public:
    MenuOption(const string& label, function<void(shared_ptr<FileHandler>)> func);
    User* execute(shared_ptr<FileHandler>) override;

private:
    function<void(shared_ptr<FileHandler>)> func_;

};

class Menu : public AbstractMenu
{
public:
    Menu() : AbstractMenu() {}
    Menu(bool isAuthorized) : AbstractMenu(isAuthorized) {}
    Menu(shared_ptr<FileHandler> fileHandler) : AbstractMenu(fileHandler) {}

    void addOption(const MenuOption& option);
    void run() override;
    
    ~Menu() { options_.clear(); }

private:
    vector<MenuOption> options_;

};

int readNumber();
shared_ptr<Menu> createPatientMenu();
shared_ptr<Menu> createAdminMenu();
shared_ptr<Menu> createDoctorMenu();
shared_ptr<Menu> createUserEditMenu(shared_ptr<FileHandler> fileHandler);
shared_ptr<Menu> createPatientEditMenu(shared_ptr<FileHandler> fileHandler);
shared_ptr<Menu> createDoctorEditMenu(shared_ptr<FileHandler> fileHandler);
shared_ptr<SearchMenu> createUserSearchMenu(shared_ptr<FileHandler> fileHandler);
shared_ptr<SearchMenu> createPatientSearchMenu();
//SearchMenu* createAdminSearchMenu();
shared_ptr<SearchMenu> createDoctorSearchMenu();

template<class T>
T* chooseFromList(vector<T*> list)
{
    if (list.size() == 0)
        return nullptr;
    if (list.size() == 1)
        return list[0];
    T::printTableHeader();
    int index = 0;
    for (auto element : list)
    {
        //cout << "----" << i++ << "----\n";
        element->tablePrint(index);
        index++;
    }
    int choice;
    do
    {
        cout << "\nКакой номер Вас интересует: ";
        choice = readNumber() - 1;
    } while (choice >= list.size());
    return list[choice];
}

//template<class T>
//Menu<T> createPatientMenu()
//{
//    Menu<T> patientMenu(true);
//    MenuOption patientOption1("Просмотр учетной записи", []() {
//        cout << patientMenu.user << endl;
//        });
//    MenuOption patientOption2("Редактирование учетной записи", []() {
//        });
//    MenuOption patientOption3("Запись на прием", []() {
//        });
//
//    patientMenu.addOption(patientOption1);
//    patientMenu.addOption(patientOption2);
//    patientMenu.addOption(patientOption3);
//    return patientMenu;
//}
//template<class T>
//Menu<T> createAdminMenu()
//{
//    Menu<T> adminMenu(true);
//    MenuOption adminOption1("Просмотр учетной записи", []() {
//        cout << adminMenu.user << endl;
//        });
//    MenuOption adminOption2("Редактирование учетной записи", []() {
//        });
//    MenuOption adminOption3("Поиск учетной записи пациента", []() {
//        });
//    MenuOption adminOption4("Редактирование учетной записи пациента", []() {
//        });
//    MenuOption adminOption5("Запись пациента на прием", []() {
//        });
//
//    adminMenu.addOption(adminOption1);
//    adminMenu.addOption(adminOption2);
//    adminMenu.addOption(adminOption3);
//    adminMenu.addOption(adminOption4);
//    adminMenu.addOption(adminOption5);
//    return adminMenu;
//}
//template<class T>
//Menu<T> createDoctorMenu()
//{
//    Menu<T> doctorMenu(true);
//    MenuOption adminOption1("Просмотр учетной записи", []() {
//        cout << doctorMenu.user << endl;
//        });
//    MenuOption adminOption2("Редактирование учетной записи", []() {
//        });
//    return doctorMenu;
//}
//Menu createSearchPatientMenu();
//Menu createSortPatientMenu();
//Menu createSearchDoctorMenu();
//Menu createSortDoctorMenu();
//Menu createAdminMenu(Menu& adminDoctorChoice, Menu& adminPatientChoice);
//Menu createChoiceDoctorMenu(Menu& searchDoctorMenu, Menu& sortDoctorMenu);
//Menu createChoicePatientMenu(Menu& searchPatientMenu, Menu& sortPatientMenu);
//Dima loh