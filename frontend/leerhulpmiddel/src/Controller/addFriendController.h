#ifndef ADDFRIENDCONTROLLER_H
#define ADDFRIENDCONTROLLER_H

#include "../model/manager/NetworkManager.h"

class addFriendController : public QObject
{
	Q_OBJECT
public:
	addFriendController();

	void getUsersByPage(const int page, QString userInput = ""); 
	int getUserPageSize() const { return m_userList.size(); }
	QString getUser(const int index) const { return m_userList[index]; }
	int getPageAmount() const { return m_totalAmountOfPages; }
	int getCurrentPage() const { return m_currentPage; }
	bool getFriendshipStatus(const int index) const { return m_areFriendsList[index]; }
	bool hasSentFriendRequest(const int index) const { return m_sentFriendRequest[index]; }
	bool hasReceivedFriendRequest(const int index) const { return m_receivedFriendRequest[index]; }
	void clearUserList() { m_userList.clear(); }

	void sendFriendRequest(const QString userToAdd) const { m_networkManager->sendFriendRequest(userToAdd); }

	bool allowedToSend(const QString text);

signals: 
	void usersFetched();

private:
	NetworkManager* m_networkManager;

	QList<QString> m_userList;
	QList<bool> m_areFriendsList;
	QList<bool> m_sentFriendRequest;
	QList<bool> m_receivedFriendRequest;
	int m_totalAmountOfPages;
	int m_currentPage;

	QString m_previousInput;
};

#endif