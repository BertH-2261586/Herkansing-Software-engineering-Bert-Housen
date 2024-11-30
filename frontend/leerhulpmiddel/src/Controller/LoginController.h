#pragma once
#include <QString>

class LoginController
{
	public:
	LoginController();
	void login(QString username, QString password);
	void registerUser(QString username, QString password);
};

