#pragma once
#include "../model/manager/NetworkManager.h"



class ShareController
{
public:
	inline ShareController(NetworkManager* networkManager) : m_networkManager{networkManager} {};
	void shareQuestionSets(QList<QString> questionSetPaths);
	void shareQuestionSetsWithFriends(QList<QString> FriendIds,int code);
private:
	NetworkManager* m_networkManager;
};

