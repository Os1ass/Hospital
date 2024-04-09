#pragma once
#include "AbstractMenu.h"

class SearchMenuOption : public AbstractMenuOption
{
public:
	SearchMenuOption(const string& label, function<User*(shared_ptr<FileHandler>)> func);
	User* execute(shared_ptr<FileHandler>) override;

private:
	function<User* (shared_ptr<FileHandler>)> func_;

};

class SearchMenu : public AbstractMenu
{
public:
	User* srchUser = nullptr;
	SearchMenu() : AbstractMenu() {}
	SearchMenu(bool isAuthorized) : AbstractMenu(isAuthorized) {}
	SearchMenu(shared_ptr<FileHandler> fileHandler) : AbstractMenu(fileHandler) {}

	void addOption(const SearchMenuOption& option);
	void run() override;

	~SearchMenu() { options_.clear(); }

private:
	vector<SearchMenuOption> options_;

};