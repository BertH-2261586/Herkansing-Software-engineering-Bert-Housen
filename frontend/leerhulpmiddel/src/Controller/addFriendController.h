#ifndef ADDFRIENDCONTROLLER_H
#define ADDFRIENDCONTROLLER_H

#include "../model/manager/NetworkManager.h"

class addFriendController : public QObject
{
	Q_OBJECT
public:
	addFriendController();

	void getUsersByPage(const int page, QString userInput = "") const { m_networkManager->getUsersByPage(page, userInput); }
	int getUserPageSize() const { return m_userList.size(); }
	QString getUser(const int index) const { return m_userList[index]; }

signals: 
	void usersFetched();

private:
	NetworkManager* m_networkManager;

	QList<QString> m_userList;
};

#endif