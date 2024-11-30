#pragma once
#include <QString>
#include <QNetworkAccessManager>

class NetworkManager
{
	public:
	NetworkManager();
	void login(QString username, QString password);
	void registerUser(QString username, QString password);
};

