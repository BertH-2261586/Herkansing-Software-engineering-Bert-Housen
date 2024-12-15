#pragma once
#include <QString>
#include "../model/manager/NetworkManager.h"

class LoginController
{
public:
	inline LoginController() : networkManager{new NetworkManager} {};
	void login(QString username, QString password);
	void registerUser(QString username, QString password);

private:
	NetworkManager* networkManager;
};

