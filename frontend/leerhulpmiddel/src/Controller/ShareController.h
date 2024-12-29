#pragma once
#include "../model/manager/NetworkManager.h"



class ShareController : public QObject
{
	Q_OBJECT
public:
	ShareController(NetworkManager* networkManager);
	void shareQuestionSets(QList<QString> questionSetPaths);
	void shareQuestionSetsWithFriends(QList<int> FriendIds,QString code);
	void getFriendUsernames() { m_networkManager->getFriendUsernames(); }
	int getFriendAmount() const { return m_usernames.size(); }
	QString getFriendName(const int index) const { return m_usernames.at(index); }
	int getFriendID(const int index) const { return m_userIDs.at(index); }

signals:
	void friendUsernamesFetched();

private:
	NetworkManager* m_networkManager;

	QList<int> m_userIDs;
	QList<QString> m_usernames;
};

