#include "NetworkManager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSettings>

#include "../fileManager.h"
#include "../../Exceptions/NoSavedSessionException.h"

NetworkManager::NetworkManager()
{
	networkManager = new QNetworkAccessManager(this);
}

void NetworkManager::login(QString username, QString password)
{
	QNetworkRequest request(QUrl("http://localhost:80/user/login"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QJsonObject json = QJsonObject();
	json["username"] = username;
	json["password"] = password;
	

	QByteArray data = QJsonDocument(json).toJson();

	QNetworkReply* reply = networkManager->post(request, data);
	connect(reply, &QNetworkReply::finished, [this,reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Network error:" << reply->errorString();
			QByteArray responseData = reply->readAll();
			qDebug() << "Response data:" << responseData;
			reply->deleteLater();
			emit loginFailed();
			return;
		}

		QJsonObject responseData = QJsonDocument::fromJson(reply->readAll()).object();
		if (responseData.contains("token") && responseData["token"].isString()) {
			saveSessionCookie(responseData["token"].toString());
		}

		reply->deleteLater();
		emit loginSuccess();
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

	QNetworkReply* reply = networkManager->post(request, data);
	connect(reply, &QNetworkReply::finished, [this, reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			QByteArray responseData = reply->readAll();
			reply->deleteLater();
			emit registerFailed();
			return;
		}

		QJsonObject responseData = QJsonDocument::fromJson(reply->readAll()).object();
		if (responseData.contains("token") && responseData["token"].isString()) {
			saveSessionCookie(responseData["token"].toString());
		}		
		
		reply->deleteLater();
		emit loginSuccess();
	});
}

/*
*
* @param questionSetPaths: list of paths to question sets
*/
void NetworkManager::shareQuestionSets(QList<QString> questionSetPaths)
{
	QNetworkRequest request(QUrl("http://localhost:80/questionset/share"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");


	FileManager* fileManager = new FileManager();
	QByteArray compressedData = fileManager->createZip(questionSetPaths);

	if (compressedData.isEmpty()) {
		emit shareFailed();
		return;
	}

	QNetworkReply* reply = networkManager->post(request, compressedData); 
	connect(reply, &QNetworkReply::finished, [this, reply]() { 

		//TEMP TO CHECK IF IT WORKS REMOVE ONCE API IS DONE
		emit shareSuccess("1234");
		reply->deleteLater();
		return;



		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) { 
			QByteArray responseData = reply->readAll(); 
			reply->deleteLater(); 
			emit shareFailed(); 
			return;
		}

		QJsonObject responseData = QJsonDocument::fromJson(reply->readAll()).object(); 
		QString code = "";
		if (responseData.contains("code") && responseData["code"].isString()) { 
			QString code = responseData["code"].toString();
		}

		reply->deleteLater(); 
		emit shareSuccess(code); 
	});
}

/**
* Notifies the server to share the question sets with the friends
* @param FriendIds: list of friend IDs
* @param code: code of the question set
*/
void NetworkManager::shareQuestionSetsWithFriends(QList<QString> FriendIds, int code)
{
	QNetworkRequest request(QUrl("http://localhost:80/friend/"));

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
QString NetworkManager::getSessionCookie() const {
	QSettings settings = QSettings("groep_7", "leerhulpmiddel");
	QString sessionCookie = settings.value("sessionCookie").toString();

	if (sessionCookie == "") {
		throw NoSavedSessionException();
	}

	return sessionCookie;
}
