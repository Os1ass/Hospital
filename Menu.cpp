#include <iostream>
#include <string>
#include <functional>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <chrono>
#include <map>

#include "Menu.h"
#include "AdminFileHandler.h"
#include "DoctorFileHandler.h"

using namespace std;


using time_point = std::chrono::system_clock::time_point;
const string timeFormat = "%d.%m.%Y";

const string patientsFilename = "patients";
const string adminsFilename = "admins";
const string doctorsFilename = "doctors";

std::string serializeTimePoint(const time_point& time) {
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    std::tm tm;
    localtime_s(&tm, &tt); //Locale time-zone, usually UTC by default.
    std::stringstream ss;
    ss << std::put_time(&tm, timeFormat.c_str());
    return ss.str();
}

time_point getTimeFromString(const string& str) {
    std::tm tm = {};
    std::stringstream ss(str);
    ss >> std::get_time(&tm, timeFormat.c_str());
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return tp;
}

string checkDate() {
    bool isValidDate;
    string copy;
    do {
        string date;
        string array[3];
        size_t position = 0;
        int index = 0;
        bool isLeap = false;
        isValidDate = true;
        int day, month, year;
        cin >> date;
        copy = date;
        while ((position = date.find('.')) != std::string::npos) {
            std::string token = date.substr(0, position);
            array[index] = token;
            date.erase(0, position + 1);
            index += 1;
        }
        if (index != 2) isValidDate = false;
        array[index] = date;
        day = atoi(array[0].c_str());
        month = atoi(array[1].c_str());
        year = atoi(array[2].c_str());
        if (year % 4 == 0) isLeap = true;
        if (year < 1970 || year > 2022) isValidDate = false;
        if (day < 1 || day > 31) isValidDate = false;
        if (month < 1 || month > 12) isValidDate = false;
        if (month == 2 && day > 29 && isLeap == true) isValidDate = false;
        if (month == 2 && day > 28 && isLeap == false) isValidDate = false;
        if (month == 4 || month == 6 || month == 9 || month == 11) {
            if (day > 30) isValidDate = false;
        }
        if (isValidDate == false) cout << "�������, �� ���������� ����. ��������� ������� �����:  ";
    } while (isValidDate == false);
    return copy;
}

void checkPasswordStrength(string& password)
{
    int n = password.length();
    bool hasLower = false, hasUpper = false, hasDigit = false;

    for (int i = 0; i < n; i++) {
        if (islower(password[i]))
            hasLower = true;
        if (isupper(password[i]))
            hasUpper = true;
        if (isdigit(password[i]))
            hasDigit = true;
    }
    cout << "���������� ������-";

    if (hasUpper && hasDigit && hasLower && (n >= 6))
        cout << "�������" << endl;
    else if ((hasLower || hasUpper) && hasDigit && (n >= 6))
        cout << "�������" << endl;
    else
        cout << "������" << endl;
}

MenuOption::MenuOption(const string& label, function<void(shared_ptr<FileHandler> fileHandler)> func) {
    label_ = label;
    func_ = func;
}

User* MenuOption::execute(shared_ptr<FileHandler> fileHandler) {
    func_(fileHandler);
    return nullptr;
}

void Menu::addOption(const MenuOption& option) {
    options_.emplace_back(option);
}

int readNumber()
{
    int number = 0;
    while (true)
    {
        cin >> number;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "������ �����. ������� ����� ������\n";
        }
        else break;
    }
    return number;
}

void Menu::run() {
    if (!isAuthorized_) {
        auto authResult = authorize();
        if (!authResult) {
            cout << "�������� ����� ��� ������\n��������� �������\n";
            system("pause");
            return;
        }
    }

    bool is_exit = false;
    while (!is_exit) {
        system("cls");
        for (int i = 0; i < options_.size(); ++i) {
            cout << i + 1 << ". " << options_[i].getLabel() << endl;
        }

        cout << options_.size() + 1 << ". �����\n";

        cout << "������� ��� �����: ";
        int choice;
        choice = readNumber();
        if (choice > 0 && choice <= options_.size()) {
            options_[choice - 1].execute(fileHandler);
        }
        else if (choice == options_.size() + 1) {
            is_exit = true;
        }
        else
        {
            cout << "����� ����� ���, ���������� ��� ���\n";
            system("pause");
        }
    }
}

Reception* createReception(User* patient, User* doctor)
{
    int receptionHour;
    Date closestDate, date;
    Date curDate = Date(Date::getCurrentDate());
    Date endDate = Date(Date::getEndOfReceptionsDate());
    while (true)
    {
        do
        {
            cout << "������� ������� �����(���) ��� ������ (���� �������� � 9 �� 18): ";
            cin >> receptionHour;
        } while (receptionHour < 9 || receptionHour > 18);
        closestDate =
            static_cast<Doctor*>(doctor)->getClosestFreeDate(++curDate, endDate, receptionHour);
        if (closestDate.getDay() == 0)
            cout << "�� ��� ����� ��������� ��� ���, ���������� ��� ���\n";
        else
            break;
    }
    while (true)
    {
        cout << "��������� ��������� ����: " + closestDate.getDateString() << endl;
        cout << "���� ������\n";
        date.initByConsole();
        if (date >= closestDate &&
            date <= endDate &&
            static_cast<Doctor*>(doctor)->isDateFree(date, receptionHour))
            break;
        cout << "���� �� ��������, ���������� ��� ���\n";
    }
    string complaint;
    cout << "������� ������� ��������� � �������:\n";
    if (cin.peek() == '\n')
        cin.get();
    getline(cin, complaint);
    return new Reception(date.getDateString(), receptionHour, doctor->getId(), patient->getId(), complaint + ": " + patient->getName());
}

shared_ptr<Menu> createPatientPrintMenu(shared_ptr<FileHandler> patientFileHandler)
{
    shared_ptr<Menu> patientPrintMenu = make_shared<Menu>(patientFileHandler);
    MenuOption printOption1("�������� �������� ����������", [](shared_ptr<FileHandler> fileHandler) {
        fileHandler->printAuthorizedUser();
        system("pause");
        });
    MenuOption printOption2("�������� ������� �������", [](shared_ptr<FileHandler> fileHandler) {
        fileHandler->getAuthorizedUser()->printDiseasesHistory();
        system("pause");
        });
    MenuOption printOption3("�������� ������� �� �����", [](shared_ptr<FileHandler> fileHandler) {
        fileHandler->getAuthorizedUser()->printReceptions();
        system("pause");
        });

    patientPrintMenu->addOption(printOption1);
    patientPrintMenu->addOption(printOption2);
    patientPrintMenu->addOption(printOption3);
    return patientPrintMenu;
}

shared_ptr<Menu> createPatientMenu()
{
    shared_ptr<Menu> patientMenu = make_shared<Menu>();
    patientMenu->fileHandler = make_shared<PatientFileHandler>(patientsFilename);
    MenuOption patientOption1("�������� ����� ������� ������", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<Menu> patientPrintMenu = createPatientPrintMenu(fileHandler);
        patientPrintMenu->run();
        });
    MenuOption patientOption2("�������������� ����� ������� ������", [&](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<Menu> patientEditMenu = createPatientEditMenu(fileHandler);
        patientEditMenu->run();
        //patientEditMenu->fileHandler->save();
        });
    MenuOption patientOption3("������ �� �����", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<DoctorFileHandler> doctorFileHandler = make_shared<DoctorFileHandler>(doctorsFilename);
        User* doctor = chooseFromList(doctorFileHandler->getUsersList());
        if (!doctor)
        {
            cout << "������ ������� ���\n";
            system("pause");
            return;
        }
        User* patient = fileHandler->getAuthorizedUser();
        cout << "����, � �������� �� ������ ����������:\n";
        doctor->print();
        cout << endl;
        Reception* reception = createReception(patient, doctor);
        static_cast<Patient*>(patient)->addReception(reception);
        //fileHandler->save();
        static_cast<Doctor*>(doctor)->addReception(reception);
        //doctorSearchMenu->fileHandler->save();
        system("pause");
        });

    patientMenu->addOption(patientOption1);
    patientMenu->addOption(patientOption2);
    patientMenu->addOption(patientOption3);
    return patientMenu;
}

shared_ptr<Menu> createAdminPrintMenu()
{
    shared_ptr<Menu> adminPrintMenu = make_shared<Menu>(true);
    MenuOption printOption1("�������� ������� ������� ���������", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<PatientFileHandler> patientFileHandler =
            make_shared<PatientFileHandler>(patientsFilename);
        patientFileHandler->printUsers();
        system("pause");
        });
    MenuOption printOption2("�������� ������� ������� ������", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<DoctorFileHandler> doctorFileHandler =
            make_shared<DoctorFileHandler>(doctorsFilename);
        doctorFileHandler->printUsers();
        system("pause");
        });

    adminPrintMenu->addOption(printOption1);
    adminPrintMenu->addOption(printOption2);
    return adminPrintMenu;
}

shared_ptr<Menu> createPatientHandlerMenu(shared_ptr<FileHandler> patientFileHandler)
{
    shared_ptr<Menu> patientHandlerMenu = make_shared<Menu>(patientFileHandler);
    MenuOption handlerOption1("�������� ������� ������ ��������", [&](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<Menu> patientPrintMenu = createPatientPrintMenu(fileHandler);
        patientPrintMenu->run();
        });
    MenuOption handlerOption2("�������������� ������� ������ ��������", [&](shared_ptr<FileHandler> fileHandler) {
        User* user = fileHandler->getAuthorizedUser();
        if (!user)
            cout << "����� ������������� ���\n";
        else
        {
            cout << "������� ������ ��� ��������������:\n";
            user->print();
            system("pause");
            shared_ptr<Menu> userEditMenu =
                createPatientEditMenu(fileHandler);
            userEditMenu->run();
            //userEditMenu->fileHandler->save();
            //fileHandler = make_shared<PatientFileHandler>(patientsFilename, user->getId());
        }
        });

    patientHandlerMenu->addOption(handlerOption1);
    patientHandlerMenu->addOption(handlerOption2);
    return patientHandlerMenu;
}

shared_ptr<Menu> createAdminMenu()
{
    shared_ptr<Menu> adminMenu = make_shared<Menu>();
    adminMenu->fileHandler = make_shared<AdminFileHandler>(adminsFilename);
    MenuOption adminOption1("�������� ����� ������� ������", [](shared_ptr<FileHandler> fileHandler) {
        fileHandler->printAuthorizedUser();
        system("pause");
        });
    MenuOption adminOption2("�������������� ����� ������� ������", [&](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<Menu> adminEditMenu = createUserEditMenu(fileHandler);
        adminEditMenu->run();
        //adminEditMenu->fileHandler->save();
        });
    MenuOption adminOption3("����� ������� ������ ��������", [&](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<SearchMenu> patientSearchMenu = createPatientSearchMenu();
        patientSearchMenu->run();
        User* user = patientSearchMenu->srchUser;
        if (user)
        {
            shared_ptr<FileHandler> patientFileHandler = make_shared<PatientFileHandler>(patientsFilename, user->getId());
            shared_ptr<Menu> patientHandlerMenu = createPatientHandlerMenu(patientFileHandler);
            patientHandlerMenu->run();
            return;
        }
        });
    MenuOption adminOption4("�������� ������� �������", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<Menu> adminPrintMenu = createAdminPrintMenu();
        adminPrintMenu->run();
        });
    MenuOption adminOption5("������ �������� �� �����", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<SearchMenu> patientSearchMenu = createPatientSearchMenu();
        patientSearchMenu->run();
        User* patient = patientSearchMenu->srchUser;
        if (!patient)
        {
            cout << "������ ������������ ���\n";
            system("pause");
            return;
        }
        shared_ptr<DoctorFileHandler> doctorFileHandler = make_shared<DoctorFileHandler>(doctorsFilename);
        User* doctor = chooseFromList(doctorFileHandler->getUsersList());
        if (!doctor)
        {
            cout << "������ ������� ���\n";
            system("pause");
            return;
        }
        Reception* reception = createReception(patient, doctor);
        static_cast<Patient*>(patient)->addReception(reception);
        //patientSearchMenu->fileHandler->save();
        static_cast<Doctor*>(doctor)->addReception(reception);
        //doctorSearchMenu->fileHandler->save();
        system("pause");
        });

    adminMenu->addOption(adminOption1);
    adminMenu->addOption(adminOption2);
    adminMenu->addOption(adminOption3);
    adminMenu->addOption(adminOption4);
    adminMenu->addOption(adminOption5);
    return adminMenu;
}

shared_ptr<Menu> createDoctorReceptionsHandlerMenu(shared_ptr<FileHandler> doctorFileHandler)
{
    shared_ptr<Menu> doctorReceptionsHandlerMenu = make_shared<Menu>(doctorFileHandler);
    MenuOption handlerOption1("�������� ����� �������", [](shared_ptr<FileHandler> fileHandler) {
        fileHandler->getAuthorizedUser()->printReceptions();
        system("pause");
        });
    MenuOption handlerOption2("��������� �������� �����", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<SearchMenu> patientSearchMenu = createPatientSearchMenu();
        patientSearchMenu->run();
        User* patient = patientSearchMenu->srchUser;
        if (!patient)
        {
            cout << "����� ������������ ���\n";
            system("pause");
            return;
        }
        User* doctor = fileHandler->getAuthorizedUser();
        cout << "�������, �������� �� ������ ��������� �����:\n";
        patient->print();
        cout << endl;
        Reception* reception = createReception(patient, doctor);
        static_cast<Patient*>(patient)->addReception(reception);
        //patientSearchMenu->fileHandler->save();
        static_cast<Doctor*>(doctor)->addReception(reception);
        //fileHandler->save();
        system("pause");
        });
    MenuOption handlerOption3("�������� �����", [](shared_ptr<FileHandler> fileHandler) {
        auto receptions =
            static_cast<Doctor*>(fileHandler->getAuthorizedUser())->getReceptionsToDo();
        if (receptions.size() == 0)
        {
            cout << "��� �������\n";
            system("pause");
            return;
        }
        auto reception = chooseFromList(receptions);
        auto patientFileHandler = make_shared<PatientFileHandler>(patientsFilename, reception->getPatientId());;
        auto patient = patientFileHandler->getAuthorizedUser();
        if (!patient)
        {
            cout << "�� ������� ����� ��������\n";
            system("pause");
            return;
        }
        auto doctor = fileHandler->getAuthorizedUser();
        auto date = Date();
        Reception* newReception;
        Disease newDisease = Disease();
        string conclusion, diseaseName;
        system("cls");
        cout << "�������:\n";
        patient->print();
        patient->printDiseasesHistory();
        patient->printReceptions();
        //cout << "������� ��������� �������: "; cin >> conclusion;
        cout << "\n1. ������� ������� (��������� ��������� �����)\n"
            << "2. ������� ���������� (�������� ������ � ������� �������)\n"
            << "3. ������� ������\n";
        int choice;
        cout << "������� ��� �����: "; choice = readNumber();
        switch (choice)
        {
        case 1:
            cout << "��������� �����\n";
            newReception = createReception(patient, doctor);
            static_cast<Patient*>(patient)->addReception(newReception);
            static_cast<Doctor*>(doctor)->addReception(newReception);
            break;
        case 2:
            cout << "������ �������\n";
            date.initByConsole();
            cout << "�������� �������\n";
            if (cin.peek() == '\n')
                cin.get();
            getline(cin, diseaseName);
            newDisease = Disease(date, reception->getDate(), diseaseName);
            static_cast<Patient*>(patient)->addDisease(newDisease);
            break;
        default: cout << "����������� ����, ����� �� ��������\n"; return;
        }
        cout << "\n������� ��� ����������� �� ������: ";
        if (cin.peek() == '\n')
            cin.get();
        getline(cin, conclusion);
        auto patientReception = static_cast<Patient*>(patient)->getReception(reception);
        if (patientReception)
            patientReception->makeReception(conclusion);
        reception->makeReception(conclusion);
        system("pause");
        });

    doctorReceptionsHandlerMenu->addOption(handlerOption1);
    doctorReceptionsHandlerMenu->addOption(handlerOption2);
    doctorReceptionsHandlerMenu->addOption(handlerOption3);
    return doctorReceptionsHandlerMenu;
}

shared_ptr<Menu> createDoctorMenu()
{
    shared_ptr<Menu> doctorMenu = make_shared<Menu>();
    doctorMenu->fileHandler = make_shared<DoctorFileHandler>(doctorsFilename);
    MenuOption doctorOption1("�������� ����� ������� ������", [](shared_ptr<FileHandler> fileHandler) {
        fileHandler->printAuthorizedUser();
        system("pause");
        });
    MenuOption doctorOption2("�������������� ����� ������� ������", [&](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<Menu> doctorEditMenu = createDoctorEditMenu(fileHandler);
        doctorEditMenu->run();
        //doctorEditMenu->fileHandler->save();
        });
    MenuOption doctorOption3("������ � ��������", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<Menu> doctorReceptionsHandlerMenu = createDoctorReceptionsHandlerMenu(fileHandler);
        doctorReceptionsHandlerMenu->run();
        });
    MenuOption doctorOption4("�������� ������� ������� ����� ���������", [](shared_ptr<FileHandler> fileHandler) {
        shared_ptr<PatientFileHandler> patientFileHandler =
            make_shared<PatientFileHandler>(patientsFilename);
        vector<User*> users = patientFileHandler->getUsersList();
        User* doctor = fileHandler->getAuthorizedUser();
        bool havePatients = false;
        for (auto user : users)
            if (static_cast<Doctor*>(doctor)->isPatientOnReception(user->getId()))
                havePatients = true;
        if (!havePatients)
        {
            cout << "� ��� ���� ��� ���������\n";
            system("pause");
            return;
        }
        User::printTableHeader();
        int index = 0;
        for (auto user : users)
        {
            if (static_cast<Doctor*>(doctor)->isPatientOnReception(user->getId()))
                user->tablePrint(index);
            index++;
        }
        system("pause");
        });

    doctorMenu->addOption(doctorOption1);
    doctorMenu->addOption(doctorOption2);
    doctorMenu->addOption(doctorOption3);
    doctorMenu->addOption(doctorOption4);
    return doctorMenu;
}

shared_ptr<Menu> createUserEditMenu(shared_ptr<FileHandler> fileHandler)
{
    shared_ptr<Menu> userEditMenu = make_shared<Menu>(fileHandler);
    MenuOption editOption1("������������� �����", [](shared_ptr<FileHandler> fileHandler) {
        string login;
        cout << "������� ����� �����: "; cin >> login;
        fileHandler->editLogin(login);
        });
    MenuOption editOption2("������������� ������", [](shared_ptr<FileHandler> fileHandler) {
        string password = User::readPassword();
        fileHandler->editPassword(password);
        });
    MenuOption editOption3("������������� ���", [](shared_ptr<FileHandler> fileHandler) {
        string name;
        cout << "������� ����� ���: "; 
        if (cin.peek() == '\n')
            cin.get();
        getline(cin, name);
        fileHandler->getAuthorizedUser()->setName(name);
        });
    MenuOption editOption4("������������� ���", [](shared_ptr<FileHandler> fileHandler) {
        string sex;
        cout << "������� ����� ���: "; cin >> sex;
        fileHandler->getAuthorizedUser()->setSex(sex);
        });
    MenuOption editOption5("������������� �����", [](shared_ptr<FileHandler> fileHandler) {
        string phoneNumber;
        cout << "������� ����� �����: "; cin >> phoneNumber;
        fileHandler->getAuthorizedUser()->setPhoneNumber(phoneNumber);
        });
    MenuOption editOption6("������������� ���� ��������", [](shared_ptr<FileHandler> fileHandler) {
        Date birthDate;
        birthDate.initByConsole();
        fileHandler->getAuthorizedUser()->setBirthDate(birthDate);
        });

    userEditMenu->addOption(editOption1);
    userEditMenu->addOption(editOption2);
    userEditMenu->addOption(editOption3);
    userEditMenu->addOption(editOption4);
    userEditMenu->addOption(editOption5);
    userEditMenu->addOption(editOption6);
    return userEditMenu;
}

shared_ptr<Menu> createPatientEditMenu(shared_ptr<FileHandler> fileHandler)
{
    return createUserEditMenu(fileHandler);
}

shared_ptr<Menu> createDoctorEditMenu(shared_ptr<FileHandler> fileHandler)
{
    shared_ptr<Menu> doctorEditMenu = createUserEditMenu(fileHandler);
    MenuOption editOption1("������������� �������������", [](shared_ptr<FileHandler> fileHandler) {
        string specialization;
        cout << "������� ����� �������������: "; cin >> specialization;
        static_cast<DoctorFileHandler*>(fileHandler.get())->editSpecialization(specialization);
        });

    doctorEditMenu->addOption(editOption1);
    return doctorEditMenu;
}

shared_ptr<SearchMenu> createUserSearchMenu(shared_ptr<FileHandler> fileHandler)
{
    shared_ptr<SearchMenu> searchMenu = make_shared<SearchMenu>(true);
    searchMenu->fileHandler = fileHandler;
    SearchMenuOption searchOption1("����� �� ���", [&](shared_ptr<FileHandler> fileHandler) {
        string name;
        cout << "������� ���: ";
        if (cin.peek() == '\n')
            cin.get();
        getline(cin, name);
        auto usersList = fileHandler->searchUsersByName(name);
        return chooseFromList(usersList);
        });
    SearchMenuOption searchOption2("����� �� ������", [&](shared_ptr<FileHandler> fileHandler) {
        string phoneNumber;
        cout << "������� �����: "; cin >> phoneNumber;
        return fileHandler->getUserByPhoneNumber(phoneNumber);
        });
    SearchMenuOption searchOption3("����� �� ������", [&](shared_ptr<FileHandler> fileHandler) {
        string login;
        cout << "������� �����: "; cin >> login;
        return fileHandler->getUserByLogin(login);
        });

    searchMenu->addOption(searchOption1);
    searchMenu->addOption(searchOption2);
    searchMenu->addOption(searchOption3);
    return searchMenu;
}

shared_ptr<SearchMenu> createPatientSearchMenu()
{
    shared_ptr<FileHandler> fileHandler = make_shared<PatientFileHandler>(patientsFilename);
    shared_ptr<SearchMenu> searchMenu = createUserSearchMenu(fileHandler);

    SearchMenuOption searchOption1("����� �� �������", [&](shared_ptr<FileHandler> fileHandler) {
        string disease;
        cout << "������� �������: ";
        if (cin.peek() == '\n')
            cin.get();
        getline(cin, disease);
        auto usersList = static_cast<PatientFileHandler*>(fileHandler.get())->searchPatientsByDisease(disease);
        return chooseFromList(usersList);
        });

    searchMenu->addOption(searchOption1);
    return searchMenu;
}

shared_ptr<SearchMenu> createDoctorSearchMenu()
{
    shared_ptr<FileHandler> fileHandler = make_shared<DoctorFileHandler>(doctorsFilename);
    shared_ptr<SearchMenu> searchMenu = make_shared<SearchMenu>(true);
    searchMenu->fileHandler = fileHandler;
    SearchMenuOption searchOption1("����� �� �������������", [&](shared_ptr<FileHandler> fileHandler) {
        string specialization;
        cout << "������� �������������: "; 
        if (cin.peek() == '\n')
            cin.get();
        getline(cin, specialization);
        auto usersList = static_cast<DoctorFileHandler*>(fileHandler.get())->getUsersList();
        return chooseFromList(usersList);
        });

    searchMenu->addOption(searchOption1);
    return searchMenu;
}

//template<class T> 
//User* chooseUser(vector<User*> usersList)
//{
//    if (usersList.size() == 0)
//        return nullptr;
//    if (usersList.size() == 1)
//        return usersList[0];
//    int i = 1;
//    for (auto user : usersList)
//    {
//        cout << "----" << i++ << "----\n";
//        user->print();
//    }
//    int choice;
//    do
//    {
//        cout << "\n����� ������������ ��� ����������: "; 
//        cin >> choice;
//    } while (choice > usersList.size());
//    return usersList[choice];
//}
//
//Reception* chooseReception(vector<Reception*> receptions)
//{
//    if (receptions.size() == 0)
//        return nullptr;
//    if (receptions.size() == 1)
//        return usersList[0];
//    int i = 1;
//    for (auto user : usersList)
//    {
//        cout << "----" << i++ << "----\n";
//        user->print();
//    }
//    int choice;
//    do
//    {
//        cout << "\n����� ������������ ��� ����������: ";
//        cin >> choice;
//    } while (choice > usersList.size());
//    return usersList[choice];
//}



//Menu createAdminMenu(Menu& adminDoctorChoice, Menu& adminPatientChoice) {
//    Menu adminMenu(true);
//    MenuOption adminOption1("�����", [&]() {
//        adminDoctorChoice.run();
//        });
//    MenuOption adminOption2("��������", [&]() {
//        adminPatientChoice.run();
//        });
//    adminMenu.addOption(adminOption1);
//    adminMenu.addOption(adminOption2);
//    return adminMenu;
//
//}
//
//Menu createChoiceDoctorMenu(Menu& searchDoctorMenu, Menu& sortDoctorMenu) {
//    Database db;
//    Menu adminChoice;
//    MenuOption adminChoice1("�������� ���� ������", [&db]() {
//        Doctor doc;
//        db.doctors().getAll();
//        });
//    MenuOption adminChoice2("�������� ������", [&db]() {
//        int id;
//        Doctor doc;
//        cout << "����� ������ �� ������ �������?" << endl;
//        cin >> id;
//        db.doctors().remove(id);
//        });
//    MenuOption adminChoice3("�������������� ������", [&db]() {
//        int id;
//        Doctor doc;
//        Optional<Doctor> d;
//        cout << " " << endl;
//        cin >> id;
//        d = db.doctors().get(id);
//        if (d.hasValue) {
//            cout << "������� ����� ������ �������\n";
//            cout << "��� �������:";
//            cin.ignore();
//            getline(cin, d.value.name);
//            cout << "������ �������:";
//            getline(cin, d.value.password);
//            checkPassword(d.value.password);
//            cout << "������������� �������:";
//            getline(cin, d.value.specialization);
//            cout << "���� ��������:";
//            d.value.birth_date = checkDate();
//            db.doctors().update(id, doc);
//        }
//        else cout << "����� ������� ������ ���!" << endl;
//        });
//    MenuOption adminChoice4("����� ������", [&]() {
//        searchDoctorMenu.run();
//        });
//    MenuOption adminChoice5("���������� ������", [&]() {
//        sortDoctorMenu.run();
//        });
//    adminChoice.addOption(adminChoice1);
//    adminChoice.addOption(adminChoice2);
//    adminChoice.addOption(adminChoice3);
//    adminChoice.addOption(adminChoice4);
//    adminChoice.addOption(adminChoice5);
//    return adminChoice;
//}
//
//Menu createChoicePatientMenu(Menu& searchPatientMenu, Menu& sortPatientMenu) {
//    Database db;
//    Menu adminChoice;
//    MenuOption adminChoice1("�������� ���� ������", [&db]() {
//        Patient p;
//        db.patients().getAll();
//        });
//    MenuOption adminChoice2("�������� ������", [&db]() {
//        int id;
//        Patient p;
//        cout << "����� ������ �� ������ �������?" << endl;
//        cin >> id;
//        db.patients().remove(id);
//        });
//    MenuOption adminChoice3("�������������� ������", [&db]() {
//        int id;
//        Patient pt;
//        Optional<Patient> p;
//        cout << " " << endl;
//        cin >> id;
//        p = db.patients().get(id);
//        if (p.hasValue) {
//            cout << "������� ����� ������ ��������\n";
//            cout << "��� ��������:";
//            cin.ignore();
//            getline(cin, p.value.name);
//            cout << "������ ��������:";
//            getline(cin, p.value.password);
//            checkPassword(p.value.password);
//            cout << "��� �������:";
//            getline(cin, p.value.sex);
//            cout << "���� ��������:";
//            p.value.birth_date = checkDate();
//            db.patients().update(id, pt);
//        }
//        else cout << "����� ������� ������ ���!" << endl;
//        });
//    MenuOption adminChoice4("����� ������", [&]() {
//        searchPatientMenu.run();
//        });
//    MenuOption adminChoice5("���������� ������", [&]() {
//        sortPatientMenu.run();
//        });
//    adminChoice.addOption(adminChoice1);
//    adminChoice.addOption(adminChoice2);
//    adminChoice.addOption(adminChoice3);
//    adminChoice.addOption(adminChoice4);
//    adminChoice.addOption(adminChoice5);
//    return adminChoice;
//}
//
//Menu createSearchPatientMenu() {
//    Menu searchPatientMenu;
//    MenuOption searchPatientOption1("�� �����", []() {
//        });
//    MenuOption searchPatientOption2("�� ��������", []() {
//        });
//    MenuOption searchPatientOption3("�� ���� ��������", []() {
//        });
//    searchPatientMenu.addOption(searchPatientOption1);
//    searchPatientMenu.addOption(searchPatientOption2);
//    searchPatientMenu.addOption(searchPatientOption3);
//    return searchPatientMenu;
//}
//
//Menu createSortPatientMenu() {
//    Menu sortPatientMenu;
//    MenuOption sortPatientOption1("�� �����", []() {
//        });
//    MenuOption sortPatientOption2("�� ��������", []() {
//        });
//    sortPatientMenu.addOption(sortPatientOption1);
//    sortPatientMenu.addOption(sortPatientOption2);
//    return sortPatientMenu;
//}
//
//Menu createSearchDoctorMenu() {
//    Menu searchDoctorMenu;
//    MenuOption searchDoctorOption1("�� �����", []() {
//        });
//    MenuOption searchDoctorOption2("�� �������������", []() {
//        });
//    searchDoctorMenu.addOption(searchDoctorOption1);
//    searchDoctorMenu.addOption(searchDoctorOption2);
//    return searchDoctorMenu;
//}
//
//Menu createSortDoctorMenu() {
//    Menu sortDoctorMenu;
//    MenuOption sortDoctorOption1("�� �����", []() {
//        });
//    MenuOption sortDoctorOption2("�� ���� ��������", []() {
//        });
//    sortDoctorMenu.addOption(sortDoctorOption1);
//    sortDoctorMenu.addOption(sortDoctorOption2);
//    return sortDoctorMenu;
//}