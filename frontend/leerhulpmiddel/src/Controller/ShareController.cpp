#include "ShareController.h"

void ShareController::shareQuestionSets(QList<QString> questionSetPaths)
{
	m_networkManager->shareQuestionSets(questionSetPaths);
}
