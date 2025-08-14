#pragma once
#include <QString>
#include "../model/manager/NetworkManager.h"

class LoginController
{
public:
	inline LoginController(NetworkManager* networkManager) : m_networkManager{networkManager} {};
	inline void login(QString username, QString password) { m_networkManager->login(username, password); }
	inline void logout() { m_networkManager->logout(); }
	inline void getLoggedInStatus() { m_networkManager->getLoggedInStatus(); }
    inline void registerUser(QString username, QString password, bool isDocent) { m_networkManager->registerUser(username, password, isDocent); }

private:
	NetworkManager* m_networkManager;
};

