#include "LoginController.h"
#include <QDebug>



void LoginController::login(QString username, QString password)
{
	networkManager->login(username, password);
}

void LoginController::registerUser(QString username, QString password)
{
	networkManager->registerUser(username, password);
}
