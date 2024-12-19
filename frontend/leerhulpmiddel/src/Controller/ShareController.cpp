#include "ShareController.h"

void ShareController::shareQuestionSets(QList<QString> questionSetPaths)
{
	m_networkManager->shareQuestionSets(questionSetPaths);
}

void ShareController::shareQuestionSetsWithFriends(QList<int> FriendIds, QString code)
{
	m_networkManager->shareQuestionSetsWithFriends(FriendIds, code);
}
