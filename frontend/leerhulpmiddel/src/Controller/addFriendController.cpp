#include "addFriendController.h"


addFriendController::addFriendController() : m_networkManager{ new NetworkManager } {
	connect(m_networkManager, &NetworkManager::usersFetched, this, [=](QList<QString> userList) {
		m_userList = userList;
		emit usersFetched();
	});
}
