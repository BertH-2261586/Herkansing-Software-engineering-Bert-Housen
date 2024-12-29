#include "ShareController.h"

ShareController::ShareController(NetworkManager* networkManager) : m_networkManager{ networkManager } {
	connect(m_networkManager, &NetworkManager::friendUsernamesFetched, this, [=](QList<int> userIDs, QList<QString> usernames) {
		m_userIDs = userIDs;
		m_usernames = usernames;
		emit friendUsernamesFetched();
	});
};


void ShareController::shareQuestionSets(QList<QString> questionSetPaths)
{
	m_networkManager->shareQuestionSets(questionSetPaths);
}

void ShareController::shareQuestionSetsWithFriends(QList<int> FriendIds, QString code)
{
	m_networkManager->shareQuestionSetsWithFriends(FriendIds, code);
}
