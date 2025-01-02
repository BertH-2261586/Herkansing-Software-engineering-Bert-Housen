#include "addFriendController.h"

#include <cmath>  

addFriendController::addFriendController() : m_networkManager{ new NetworkManager } {
	const int maxUsersPerPage = 10;

	// All users are fetched from the database
	connect(m_networkManager, &NetworkManager::usersFetched, this, [=](int totalCount, QList<QString> userList, QList<bool> areFriendsList, 
																		QList<bool> sentFriendRequest, QList<bool> receivedFriendRequest) {
		m_totalAmountOfPages = (totalCount + maxUsersPerPage - 1) / maxUsersPerPage;		// Calculate the amount of user pages there are
		m_userList = userList;
		m_areFriendsList = areFriendsList;
		m_sentFriendRequest = sentFriendRequest;
		m_receivedFriendRequest = receivedFriendRequest;
		emit usersFetched();
	});
}

void addFriendController::getUsersByPage(const int page, QString userInput) {
	m_currentPage = page;
	m_networkManager->getUsersByPage(page, userInput); 
}

/*
* This function checks if the user is spamming the search button, doing this avoids unnecessarily calling the database
* @param text this is the text the user has put in
* @return bool it displays if the user is allowed to send the given text to the database to find the users
*/
bool addFriendController::allowedToSend(QString text) {
	if (text != m_previousInput) {
		m_previousInput = text;			// Replace the old text with the new
		return true;
	}

	return false;
}