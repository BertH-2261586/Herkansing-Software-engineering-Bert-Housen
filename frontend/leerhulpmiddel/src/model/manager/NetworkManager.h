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
	void shareQuestionSets(QList<QString> questionSetPaths);
	void shareQuestionSetsWithFriends(QList<QString> FriendIds, int code);
private:
	QNetworkAccessManager* m_networkManager;

	void saveSessionCookie(QString sessionCookie);
	QString getSessionCookie() const;
signals:
	void loginFailed();
	void registerFailed();
	void loginSuccess();
	void shareFailed();
	void shareSuccess(QString code);
};

