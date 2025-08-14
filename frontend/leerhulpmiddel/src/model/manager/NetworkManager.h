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
    inline NetworkManager() { m_networkManager = new QNetworkAccessManager(this); }

	bool cookieExists();

	void login(QString username, QString password);
	void logout();
    void registerUser(QString username, QString password, bool isDocent);
	void getLoggedInStatus();
	void shareQuestionSets(QList<QString> questionSetPaths);
	void shareQuestionSetsWithFriends(QList<int> FriendIds, QString code);
	void acceptQuestionSet(QString code);

	void getUsersByPage(const int page, const QString userInput);
	void sendFriendRequest(const QString userToAdd);
	void addFriend(const int sendingUserID);
	void getInboxMessages();
	void removeInboxMessage(const int ID);
	void getFriendUsernames();

    void getAllVakData();
    void addVak(QString vakNaam);
    void addDocentToVak(int docentId, int vakId, QString titel);
    void removeDocent(int docentId, int vakId);
    void addExamToVak(QString fileNaam, int vakId);
    void sendScore(int percent, int examId);

	// = PRIVATE, this is for unit test purposes
	void setLoginStatus(bool status);
	QString getSessionCookie();

private:
	QNetworkAccessManager* m_networkManager;

	void saveSessionCookie(QString sessionCookie);
	void setUserId(int id);
	int getUserId() const;
	int getUserIdByUsername(const QString username);
	void receiveUserByPageHandler(QNetworkReply* reply);

    void vakkenNetwerkHelper(QNetworkRequest request, QByteArray data);

    void setDocentStatus(bool isDocent);

signals:
	void loginFailed();
	void registerFailed();
	void loginSuccess();
	void loggedIn();
	void loggedOut();
	void shareFailed();
	void shareSuccess(QString code);
	void shareQuestionSetsSuccess();
	void usersFetched(int totalCount, QList<QString> users, QList<bool> areFriendsList, QList<bool> sentFriendRequest, QList<bool> receivedFriendRequest);
	void inboxMessagesFetched(QList<QJsonObject> inboxMessages);
	void friendUsernamesFetched(QList<int> userIDs, QList<QString> usernames);	
	void questionSetSucces();
	void questionSetFailed(const bool codeExists = true);

    void vakkenChanged(QJsonArray jsonData);
    void docentenChanged(QJsonArray jsonData);
};

#endif
