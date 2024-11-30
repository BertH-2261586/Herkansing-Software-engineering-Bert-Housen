#include "LoginController.h"
#include <QDebug>


LoginController::LoginController()
{
}

void LoginController::login(QString username, QString password)
{
	//** This is where the login logic would go
	//** For now, we will just print the username and password
	qDebug() << "Username: " << username;
	qDebug() << "Password: " << password;
}

void LoginController::registerUser(QString username, QString password)
{
	//** This is where the register logic would go
	//** For now, we will just print the username and password
	qDebug() << "Username register: " << username;
	qDebug() << "Password register: " << password;
}
