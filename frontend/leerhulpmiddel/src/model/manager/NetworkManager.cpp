#include "NetworkManager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSettings>

#include "../fileManager.h"

#include "../../Exceptions/NoSavedSessionException.h"

#include <QJsonDocument>
#include <QSettings>
#include <QJsonArray>
#include <QByteArray>
#include <QEventLoop>

NetworkManager::NetworkManager()
{
	m_networkManager = new QNetworkAccessManager(this);
}


/**
 * Logs in a user with the provided username and password.
 * Sends a POST request to the server with the login credentials.
 * If the login is successful, saves the session token.
 * Emits loginSuccess() signal on success, loginFailed() signal on failure.
 *
 * @param username The username of the user.
 * @param password The password of the user.
 */
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
			emit loginFailed();
			return;
		}

		QJsonObject responseData = QJsonDocument::fromJson(reply->readAll()).object();
		if (responseData.contains("token") && responseData["token"].isString()) {
			saveSessionCookie(responseData["token"].toString());
			setLoginStatus(true);
			setUserId(responseData["id"].toInt());
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
* @param username: string of username
* @param password: string of password
*/
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
			QByteArray responseData = reply->readAll();
			reply->deleteLater();
			emit registerFailed();
			return;
		}

		QJsonObject responseData = QJsonDocument::fromJson(reply->readAll()).object();
		if (responseData.contains("token") && responseData["token"].isString()) {
			saveSessionCookie(responseData["token"].toString());
			setLoginStatus(true);
			setUserId(responseData["id"].toInt());
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
* Makes a zip from all questions and sends it to server
* @param questionSetPaths: list of paths to question sets
*/
void NetworkManager::shareQuestionSets(QList<QString> questionSetPaths)
{
	QNetworkRequest request(QUrl("http://localhost:80/questionset/share"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	request.setRawHeader("Token", getSessionCookie().toUtf8());


	FileManager* fileManager = new FileManager();
	QByteArray compressedData = fileManager->createZip(questionSetPaths);

	if (compressedData.isEmpty()) {
		emit shareFailed();
		return;
	}
	

	QNetworkReply* reply = m_networkManager->post(request, compressedData); 
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
void NetworkManager::shareQuestionSetsWithFriends(QList<int> FriendIds, QString code)
{
	QNetworkRequest request(QUrl("http://localhost:80/inbox/add/QuestionSets"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	QJsonObject json = QJsonObject(); 

	QJsonArray friendsArray;
	for (int friendId : FriendIds) {
		friendsArray.append(friendId);
	}

	json["friend_ids"] = friendsArray; 
	json["code"] = code;
	json["id"] = getUserId();

	QByteArray data = QJsonDocument(json).toJson();

	QNetworkReply* reply = m_networkManager->post(request, data);

	connect(reply, &QNetworkReply::finished, [this, reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			QByteArray responseData = reply->readAll();
			reply->deleteLater();
			emit shareFailed();
			return;
		}

		reply->deleteLater();
		emit shareQuestionSetsSuccess();

		//DO IT




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

	if (sessionCookie.isEmpty()) {
		setLoginStatus(false);
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

void NetworkManager::setUserId(int id) {
	QSettings settings = QSettings("groep_7", "leerhulpmiddel");

	settings.setValue("userId", id);
}

int NetworkManager::getUserId() const {
	QSettings settings = QSettings("groep_7", "leerhulpmiddel");
	int userId = settings.value("userId").toInt();

	return userId;
}

// Check if a cookie exists
bool NetworkManager::cookieExists() {
	QSettings settings("groep_7", "leerhulpmiddel");
	QString sessionCookie = settings.value("sessionCookie").toString();

	return !sessionCookie.isEmpty();
}

void NetworkManager::getUsersByPage(const int page, const QString userInput)
{
	// Build the URL
	QNetworkRequest request(QUrl("http://localhost:80/user/getUsers"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	//Post data
	QJsonObject json = QJsonObject();
	json["page"] = page;
	if (!userInput.isEmpty()) {
		json["search"] = userInput;  
	}
	QByteArray data = QJsonDocument(json).toJson();

	// Send the request
	QNetworkReply* reply = m_networkManager->post(request, data);

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
	if (!doc.isObject()) {
		qDebug() << "Error: Expected a JSON array!";
		reply->deleteLater();
		return;
	}

	// Set the list of received users
	QList<QString> userList;
	QJsonObject responseObject = doc.object();
	QJsonArray usersArray = responseObject["users"].toArray();
	for (const QJsonValue& value : usersArray) {
		userList.append(value.toString());
	}

	reply->deleteLater();

	// Send a signal to the controller with all the users 
	emit usersFetched(responseObject["total_count"].toInt(), userList);
}

int NetworkManager::getUserIdByUsername(const QString username) {
	// Build the URL
	QNetworkRequest request(QUrl("http://localhost:80/user/get_user_id"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	// Post data
	QJsonObject json = QJsonObject();
	json["username"] = username;
	qDebug() << "nazengiao" << username;
	QByteArray data = QJsonDocument(json).toJson();

	// Send the request
	QNetworkReply* reply = m_networkManager->post(request, data);

	// Wait for the request to complete
	QEventLoop eventLoop;
	connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	eventLoop.exec();

	// Handle the responses
	if (reply->error() == QNetworkReply::NoError) {
		// Received response
		QByteArray responseData = reply->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
		QJsonObject jsonObj = jsonDoc.object();
		if (jsonObj.contains("user_id")) {
			return jsonObj["user_id"].toInt(); // Return the user ID
		}
		else {
			qWarning() << "User ID not found in response";
		}
	}
	// Error
	else {
		qWarning() << "Failed to fetch user ID:" << reply->errorString();
	}

	// No ID found
	return -1;
}

void NetworkManager::sendFriendRequest(const QString userToAdd)
{
	// Build the URL
	QNetworkRequest request(QUrl("http://localhost:80/inbox/add/FriendRequests"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	// Get the user IDs
	int sendingID = getUserId();
	if (sendingID == -1) {
		return;
		// TODO error handling
	}
	int receivingID = getUserIdByUsername(userToAdd);
	if (receivingID == -1) {
		return;
		// TODO error handling
	}

	QJsonObject json = QJsonObject();
	json["type"] = "friend_request";
	json["sending_user"] = sendingID;
	json["receiving_user"] = receivingID;

	QByteArray data = QJsonDocument(json).toJson();
	QNetworkReply* reply = m_networkManager->post(request, data);
}

void NetworkManager::getInboxMessages() {
	// Build the URL
	QNetworkRequest request(QUrl("http://localhost:80/inbox/GetInboxMessages"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	//Post data
	QJsonObject json = QJsonObject();
	json["id"] = getUserId();

	QByteArray data = QJsonDocument(json).toJson();

	// Send the request
	QNetworkReply* reply = m_networkManager->post(request, data);

	connect(reply, &QNetworkReply::finished, this, [this, reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Network error:" << reply->errorString();
			QByteArray responseData = reply->readAll();
			qDebug() << "Response data:" << responseData;
			reply->deleteLater();
			return;
			//TODO: username already token
		}

		QByteArray responseData = reply->readAll();
		QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
		QList<QJsonObject> inboxItems; // Container to store parsed items

		if (jsonResponse.isArray()) {
			QJsonArray jsonArray = jsonResponse.array();
			qDebug() << "Received an array with" << jsonArray.size() << "items:";
			for (const QJsonValue& value : jsonArray) {
				QJsonObject obj = value.toObject();
				inboxItems.append(obj); // Add each object to the list
				qDebug() << obj; // Log each item
			}
			emit inboxMessagesFetched(inboxItems);
		}
		else if (jsonResponse.isObject()) {
			QJsonObject jsonObject = jsonResponse.object();
			qDebug() << "Received a single object:" << jsonObject;
		}
		else {
			qDebug() << "Unexpected response format. Raw data:" << responseData;
		}

		reply->deleteLater();
	});
}

void NetworkManager::removeInboxMessage(int inboxID) {
	// Build the URL 
	QNetworkRequest request(QUrl("http://localhost:80/inbox/remove?userID=" + QString::number(getUserId()) + "&inbox_message_id=" + QString::number(inboxID)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	// Send the delete request
	QNetworkReply* reply = m_networkManager->deleteResource(request);

	connect(reply, &QNetworkReply::finished, this, [this, reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Network error:" << reply->errorString();
			QByteArray responseData = reply->readAll();
			qDebug() << "Response data:" << responseData;
			reply->deleteLater();
			return;
		}

		reply->deleteLater();
	});
}

void NetworkManager::addFriend(int sendingUserID) {
	// Get the user ID of the user accepting the friend request
	int userID = getUserId();
	if (userID == -1) {
		return;
		// TODO error handling
	}

	// Build URL
	QNetworkRequest request(QUrl("http://localhost:80/friend/add"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	// Create the object
	QJsonObject json = QJsonObject();
	json["user1"] = userID;
	json["user2"] = sendingUserID;

	// Send the object to the backend
	QByteArray data = QJsonDocument(json).toJson();
	QNetworkReply* reply = m_networkManager->post(request, data);
}
