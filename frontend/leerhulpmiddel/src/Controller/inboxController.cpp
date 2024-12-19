#include "inboxController.h"

inboxController::inboxController() : m_networkManager{ new NetworkManager } {
	connect(m_networkManager, &NetworkManager::inboxMessagesFetched, this, [=](QList<QJsonObject> inboxMessages) {
		m_inboxMessages = inboxMessages;
		emit inboxMessagesFetched();
	});
}

void inboxController::inboxRequestResponse(int index, bool accepted) {
	if (accepted) {
		if (m_inboxMessages[index]["type"] == "friend_request") {
			int sendingUserID = m_inboxMessages[index]["sending_user_ID"].toInt();
			m_networkManager->addFriend(sendingUserID);
			qDebug() << "add as friend";
		}
		else if (m_inboxMessages[index]["type"] == "question_set") {
			qDebug() << "add question set";
		}
	}

	//m_networkManager->removeInboxMessage(m_inboxMessages[index]["id"].toInt());
}

