#include "NetworkManager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonArray>

#include "../../Exceptions/NoSavedSessionException.h"

NetworkManager::NetworkManager()
{
	m_networkManager = new QNetworkAccessManager(this);
}

void NetworkManager::login(QString username, QString password)
{
	QNetworkRequest request(QUrl("http://localhost:80/user/login"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QJsonObject json = QJsonObject();
	json["username"] = username;
	json["password"] = password;
	

	QByteArray data = QJsonDocument(json).toJson();

	QNetworkReply* reply = m_networkManager->post(request, data);
	connect(reply, &QNetworkReply::finished, [this,reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Network error:" << reply->errorString();
			QByteArray responseData = reply->readAll();
			qDebug() << "Response data:" << responseData;
			reply->deleteLater();
			return;
			//TODO: login failed feedback
		}

		QJsonObject responseData = QJsonDocument::fromJson(reply->readAll()).object();
		if (responseData.contains("token") && responseData["token"].isString()) {
			saveSessionCookie(responseData["token"].toString());
		}

		reply->deleteLater();
	});

}

void NetworkManager::registerUser(QString username, QString password)
{
	QNetworkRequest request(QUrl("http://localhost:80/user/register"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QJsonObject json = QJsonObject();
	json["username"] = username;
	json["password"] = password;

	QByteArray data = QJsonDocument(json).toJson();

	QNetworkReply* reply = m_networkManager->post(request, data);
	connect(reply, &QNetworkReply::finished, [this, reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Network error:" << reply->errorString();
			QByteArray responseData = reply->readAll();
			qDebug() << "Response data:" << responseData;
			reply->deleteLater();
			return;
			//TODO: username already token
		}

		QJsonObject responseData = QJsonDocument::fromJson(reply->readAll()).object();
		if (responseData.contains("token") && responseData["token"].isString()) {
			saveSessionCookie(responseData["token"].toString());
		}		
		
		reply->deleteLater();
	});
}

/*
* Saves session cookie to device
* @param sessionCookie: session cookie in QString format
* @post: session cookie saved locally
*/
void NetworkManager::saveSessionCookie(QString sessionCookie) {
	QSettings settings = QSettings("groep_7", "leerhulpmiddel");

	settings.setValue("sessionCookie", sessionCookie);
}

/*
* Retrieves session cookie from device
* @pre: Session cookie must be saved to be retrieved
* @return session cookie
* @exception: throws NoSavedSessionException when there is no session saved
*/
QString NetworkManager::getSessionCookie() {
	QSettings settings = QSettings("groep_7", "leerhulpmiddel");
	QString sessionCookie = settings.value("sessionCookie").toString();

	if (sessionCookie == "") {
		throw NoSavedSessionException();
	}

	return sessionCookie;
}

void NetworkManager::getUsersByPage(const int page, const QString userInput)
{
	// Build the URL
	QString url = "http://localhost:80/user/getUsers?page=" + QString::number(page);
	if (!userInput.isEmpty()) {
		// Append userInput if it's not empty
		url += "&search=" + QUrl::toPercentEncoding(userInput);			
	}
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	// Send the request
	QNetworkReply* reply = m_networkManager->get(request);

	connect(reply, &QNetworkReply::finished, this, [this, reply]() { 
		receiveUserByPageHandler(reply);
	});
}

void NetworkManager::receiveUserByPageHandler(QNetworkReply* reply) {
	// Check if an error has occurred
	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Error fetching users:" << reply->errorString();
		return;
	}

	// Parse the response data as a JSON array
	QByteArray response = reply->readAll();
	QJsonDocument doc = QJsonDocument::fromJson(response);
	// Check if the received data is an array of users
	if (!doc.isArray()) {
		qDebug() << "Error: Expected a JSON array!";
		reply->deleteLater();
		return;
	}

	// Set the list of received users
	QList<QString> userList;
	QJsonArray usersArray = doc.array();
	for (const QJsonValue& value : usersArray) {
		userList.append(value.toString());
	}

	reply->deleteLater();

	// Send a signal to the controller with all the users 
	emit usersFetched(userList);
}

