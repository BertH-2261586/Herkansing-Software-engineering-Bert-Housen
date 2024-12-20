#include "addFriendController.h"

#include <cmath>  

addFriendController::addFriendController() : m_networkManager{ new NetworkManager } {
	const int maxUsersPerPage = 10;

	connect(m_networkManager, &NetworkManager::usersFetched, this, [=](int totalCount, QList<QString> userList) {
		m_totalAmountOfPages = (totalCount + maxUsersPerPage - 1) / maxUsersPerPage;		// Calculate the amount of user pages there are
		m_userList = userList;
		emit usersFetched();
	});
}

void addFriendController::getUsersByPage(const int page, QString userInput) {
	m_currentPage = page;
	m_networkManager->getUsersByPage(page, userInput); 
}