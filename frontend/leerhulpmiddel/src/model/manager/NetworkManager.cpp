#include "NetworkManager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSettings>

#include "../../Exceptions/NoSavedSessionException.h"

NetworkManager::NetworkManager()
{
	networkManager = new QNetworkAccessManager(this);
}

/*
* Sends request to webserver to validate user login
*/
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
			setLoginStatus(true);
		}

		reply->deleteLater();
		emit loginSuccess();
	});

}

/*
* Logs user out
*/
void NetworkManager::logout() {
	setLoginStatus(false);
	saveSessionCookie("");		//Clear session cookie
}

/*
* Send request to webserver to create user
*/
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
			qDebug() << "Network error:" << reply->errorString();
			QByteArray responseData = reply->readAll();
			qDebug() << "Response data:" << responseData;
			reply->deleteLater();
			emit registerFailed();
			return;
			//TODO: username already token
		}

		QJsonObject responseData = QJsonDocument::fromJson(reply->readAll()).object();
		if (responseData.contains("token") && responseData["token"].isString()) {
			saveSessionCookie(responseData["token"].toString());
			setLoginStatus(true);
		}		
		
		reply->deleteLater();
		emit loginSuccess();
	});
}

void NetworkManager::getLoggedInStatus() {
	QSettings settings = QSettings("groep_7", "leerhulpmiddel");
	QString loginStatus = settings.value("loggedIn").toString();

	if (loginStatus == "true") {
		qDebug() << "logged in";
		emit loggedIn();
	}
	else {
		qDebug() << "logged out";
		emit loggedOut();
	}
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

/*
* Sets login status
* @param status: logged in or not?
* @post login status set
*/
void NetworkManager::setLoginStatus(bool status) {
	QSettings settings = QSettings("groep_7", "leerhulpmiddel");

	settings.setValue("loggedIn", status);
}
