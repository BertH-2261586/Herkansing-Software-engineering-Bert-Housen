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
	void logout();
	void registerUser(QString username, QString password);
	void getLoggedInStatus();
	void shareQuestionSets(QList<QString> questionSetPaths);
	void shareQuestionSetsWithFriends(QList<int> FriendIds, QString code);

	void getUsersByPage(const int page, const QString userInput);
	void sendFriendRequest(const QString userToAdd);
	void addFriend(const int sendingUserID);
	void getInboxMessages();
	void removeInboxMessage(const int ID);

private:
	QNetworkAccessManager* m_networkManager;

	void saveSessionCookie(QString sessionCookie);
	QString getSessionCookie();
	void setLoginStatus(bool status);
	void setUserId(int id);
	int getUserId() const;
	int getUserIdByUsername(const QString username);
	void receiveUserByPageHandler(QNetworkReply* reply);

signals:
	void loginFailed();
	void registerFailed();
	void loginSuccess();
	void loggedIn();
	void loggedOut();
	void shareFailed();
	void shareSuccess(QString code);
	void shareQuestionSetsSuccess();
	void usersFetched(int totalCount, QList<QString> users);
	void inboxMessagesFetched(QList<QJsonObject> inboxMessages);};

#endif