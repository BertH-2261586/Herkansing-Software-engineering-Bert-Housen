#include "ShareController.h"

void ShareController::shareQuestionSets(QList<QString> questionSetPaths)
{
	m_networkManager->shareQuestionSets(questionSetPaths);
}

void ShareController::shareQuestionSetsWithFriends(QList<QString> FriendIds, int code)
{
	m_networkManager->shareQuestionSetsWithFriends(FriendIds, code);
}
