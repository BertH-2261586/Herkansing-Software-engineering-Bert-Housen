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
	void clearUserList() { m_userList.clear(); }

	void sendFriendRequest(const QString userToAdd) const { m_networkManager->sendFriendRequest(userToAdd); }

signals: 
	void usersFetched();

private:
	NetworkManager* m_networkManager;

	QList<QString> m_userList;
	int m_totalAmountOfPages;
	int m_currentPage;
};

#endif