#include "LoginController.h"
#include <QDebug>



void LoginController::login(QString username, QString password)
{
	m_networkManager->login(username, password);
}

void LoginController::registerUser(QString username, QString password)
{
	m_networkManager->registerUser(username, password);
}
