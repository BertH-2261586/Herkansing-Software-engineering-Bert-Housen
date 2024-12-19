#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QWidget>
#include <QJsonObject>
#include <QNetworkReply>

class NetworkManager : public QObject
{
	Q_OBJECT
public:
     NetworkManager();

	bool cookieExists();

	void login(QString username, QString password);
	void registerUser(QString username, QString password);
	void getUsersByPage(const int page, const QString userInput);

	void sendFriendRequest(const QString userToAdd);
	void addFriend(const int sendingUserID);

	void getInboxMessages();

	void removeInboxMessage(const int ID);

signals:
	void usersFetched(int totalCount, QList<QString> users);
	void inboxMessagesFetched(QList<QJsonObject> inboxMessages);

private:
	QNetworkAccessManager* m_networkManager;

	QString getSessionCookie();
	void saveSessionCookie(QString sessionCookie);
	int getUserIdFromCookie(const QString& sessionCookie);

	int getUserIdByUsername(const QString username);

	void receiveUserByPageHandler(QNetworkReply* reply);
};

#endif