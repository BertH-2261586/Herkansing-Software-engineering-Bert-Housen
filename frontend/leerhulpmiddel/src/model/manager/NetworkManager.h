#pragma once
#include <QString>
#include <QNetworkAccessManager>
#include <QWidget>

class NetworkManager : public QObject
{
	Q_OBJECT
public:
     NetworkManager();

	void login(QString username, QString password);
	void registerUser(QString username, QString password);
	void getUsersByPage(const int page, const QString userInput);

signals:
	void usersFetched(QList<QString> users);

private:
	QNetworkAccessManager* m_networkManager;

	void saveSessionCookie(QString sessionCookie);
	QString getSessionCookie();

	void receiveUserByPageHandler(QNetworkReply* reply);
};

