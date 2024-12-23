#ifndef INBOXCONTROLLER_H
#define INBOXCONTROLLER_H

#include "../model/manager/NetworkManager.h"

class inboxController : public QObject
{
	Q_OBJECT
public:
	inboxController();
	void getUserInboxMessages() { m_networkManager->getInboxMessages(); }
	void inboxRequestResponse(const int index, const bool accepted);

	QString getInboxMessageType(const int index) const { return m_inboxMessages[index]["type"].toString(); }
	QString getSendingUserName(const int index) const { return m_inboxMessages[index]["sending_user"].toString(); }
	int getAmountOfMessages() const { return m_inboxMessages.size(); }
	void removeInboxMessage(const int index) { m_inboxMessages.removeAt(index); }
	void clearInbox() { m_inboxMessages.clear(); }

signals:
	void inboxMessagesFetched();

private:
	NetworkManager* m_networkManager;

	QList<QJsonObject> m_inboxMessages;
};

#endif