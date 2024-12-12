#pragma once
#include "../model/manager/NetworkManager.h"



class ShareController
{
public:
	inline ShareController(NetworkManager* networkManager) : m_networkManager{networkManager} {};
	void shareQuestionSets(QList<QString> questionSetPaths);
private:
	NetworkManager* m_networkManager;
};

