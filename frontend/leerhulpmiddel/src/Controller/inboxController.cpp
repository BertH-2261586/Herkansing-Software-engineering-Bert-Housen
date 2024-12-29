#include "inboxController.h"
#include "../View/ToastMessage.h"

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
		}
		else if (m_inboxMessages[index]["type"] == "question_set") {
			QString code = m_inboxMessages[index]["code"].toString();
			m_networkManager->acceptQuestionSet(code);
		}
	}

	connect(m_networkManager, &NetworkManager::questionSetFailed, this, [=]() {
		emit questionSetFailed();
	});
	connect(m_networkManager, &NetworkManager::questionSetSucces, this, [=]() {
		emit questionSetSucces();
	});

	m_networkManager->removeInboxMessage(m_inboxMessages[index]["id"].toInt());
}

