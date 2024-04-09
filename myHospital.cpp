#include "Menu.h"
//#include "DoctorFileHandler.h"

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));

    /*User* newD = new Doctor();
    newD->registration();
    auto dfH = DoctorFileHandler("doctors");
    dfH.add(newD);
    dfH.save();
    system("pause");*/

    /*PatientFileHandler fH("doctors", 41);
    fH.printUsers();
    fH.remove(42);
    fH.printUsers();*/



    //Menu* patientMenu = createPatientMenu();
    //patientMenu->run();
    //delete patientMenu;
    



    //Menu<User> menu;
    //Menu<Patient> patientMenu;
    //patientMenu.createPatientMenu();
    //Menu<Admin> adminMenu;
    //adminMenu.createAdminMenu();
    //Menu<Doctor> doctorMenu;
    //doctorMenu.createDoctorMenu();
    ////Menu sortDoctorMenu = createSortDoctorMenu();
    ////Menu searchDoctorMenu = createSearchDoctorMenu();
    ////Menu sortPatientMenu = createSortPatientMenu();
    ////Menu searchPatientMenu = createSearchPatientMenu();
    ////Menu adminPatientChoice = createChoicePatientMenu(searchPatientMenu, sortPatientMenu);
    ////Menu adminDoctorChoice = createChoiceDoctorMenu(searchDoctorMenu, sortDoctorMenu);
    ////Menu adminMenu = createAdminMenu(adminDoctorChoice, adminPatientChoice);

    Menu* menu = new Menu(true);

    shared_ptr<FileHandler> fH = nullptr;
    MenuOption option1("Войти как пациент", [&](shared_ptr<FileHandler> fH) {
        shared_ptr<Menu> patientMenu = createPatientMenu();
        patientMenu->run();
        });
    MenuOption option2("Войти как работник регистратуры", [&](shared_ptr<FileHandler> fH) {
        shared_ptr<Menu> adminMenu = createAdminMenu();
        adminMenu->run();
        });
    MenuOption option4("Создать аккаунт", [&](shared_ptr<FileHandler> fH) {
        User* newUser = new Patient();
        newUser->registration();
        FileHandler* patientFileHandler = new PatientFileHandler("patients");
        patientFileHandler->add(newUser);
        delete patientFileHandler;
        delete newUser;
        });
    MenuOption option3("Войти как врач", [&](shared_ptr<FileHandler> fH) {
        shared_ptr<Menu> doctorMenu = createDoctorMenu();
        doctorMenu->run();
        });

    menu->addOption(option1);
    menu->addOption(option2);
    menu->addOption(option3);
    menu->addOption(option4);
    menu->run();
    //menu.addOption(option3);
    //menu.addOption(option4);
    //menu.run();

    ///*Disease b("01.01.2001-02.02.2001-DD");
    //Disease c("02.03.2001-03.03.2001-AA");

    //FileHandler<Patient> doctorFileHandler("doctors");
    //auto list = doctorFileHandler.getUsersList();
    //for (auto l : list)
    //{
    //    cout << l << endl;
    //}
    //User* a = new Patient();
    //a->login = "aboba";
    //a->password = "123";
    //a->birthDate = Date("01.01.2001");
    //a->sex = "M";
    //a->phoneNumber = "+123";
    //a->name = "Alex";
    //static_cast<Patient*>(a)->diseasesHistory.push_back(b);
    //static_cast<Patient*>(a)->diseasesHistory.push_back(c);
    //doctorFileHandler.add(static_cast<Patient*>(a));

    //User* bb = new Patient();
    //bb->registration();
    //doctorFileHandler.add(static_cast<Patient*>(bb));
    //auto list2 = doctorFileHandler.getUsersList();
    //for (auto l : list)
    //{
    //    cout << l << endl;
    //}*/
}