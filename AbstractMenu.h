#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<windows.h>
#include<functional>
#include "Patient.h"
#include "Admin.h"
#include "Doctor.h"
#include "FileHandler.h"
#include "PatientFileHandler.h"

using namespace std;

class AbstractMenuOption {
public:
    string getLabel() const;
    virtual User* execute(shared_ptr<FileHandler>) = 0;

protected:
    string label_;
};

class AbstractMenu
{
public:
	shared_ptr<FileHandler> fileHandler;
    AbstractMenu() : AbstractMenu::AbstractMenu(false) {}
    AbstractMenu(bool isAuthorized) { isAuthorized_ = isAuthorized; }
    AbstractMenu(shared_ptr<FileHandler> fileHandler) : AbstractMenu::AbstractMenu(true) {
        this->fileHandler = fileHandler; 
    }

    void addFileHandler(shared_ptr<FileHandler> fileHandler);
    virtual void run() = 0;

    bool authorize();

    ~AbstractMenu() { //delete fileHandler; 
    }

protected:
	bool isAuthorized_;

};