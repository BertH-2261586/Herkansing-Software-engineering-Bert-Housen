#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonDocument>
#include <QSettings>
#include <QJsonArray>
#include <QByteArray>
#include <QEventLoop>

#include "../fileManager.h"
#include "../../Exceptions/NoSavedSessionException.h"
#include "../../Exceptions/unzipException.h"
#include "NetworkManager.h"

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
	emit loggedOut();
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
	loginStatus == "true" ? emit loggedIn() : loggedOut();
}

/*
* Makes a zip from all questions and sends it to server
* @param questionSetPaths: list of paths to question sets
*/
void NetworkManager::shareQuestionSets(QList<QString> questionSetPaths)
{
	QNetworkRequest request(QUrl("http://localhost:80/question_set/add"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	FileManager* fileManager = new FileManager();
	QByteArray compressedData = fileManager->createZip(questionSetPaths);

	if (compressedData.isEmpty()) {
		emit shareFailed();
	}

	QNetworkReply* reply = m_networkManager->post(request, compressedData); 
	connect(reply, &QNetworkReply::finished, [this, reply]() { 
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
			code = responseData["code"].toString();
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

/*
* Get a list of users 
* @param page this is the current page the user is trying to reach
* @userInput this is the letters the user has put in to search for a certain user
* @emit usersFetched(...) with all the necessary data for the search page for users
*/
void NetworkManager::getUsersByPage(const int page, const QString userInput)
{
	// Build the URL
	QNetworkRequest request(QUrl("http://localhost:80/user/getUsers"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	//Post data
	QJsonObject json = QJsonObject();
	json["userID"] = getUserId();
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

/*
* This is a subfunction of getUsersByPage, this handles the reply of the database
* @param reply the reply with all the data from the database
* @emit usersFetched(...) with all the necessary data for the search page for users
*/
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
	QJsonObject responseObject = doc.object();
	QJsonArray usersArray = responseObject["users"].toArray();
	QList<QString> userList;
	for (const QJsonValue& value : usersArray) {
		userList.append(value.toString());
	}

	// Get all the users are friends with the current user
	QJsonObject requests = responseObject["are_friends"].toObject();

	// Sort the keys or else the bools dont match up with the user
	QList<QString> sortedKeys = requests.keys();
	std::sort(sortedKeys.begin(), sortedKeys.end(), [](const QString& a, const QString& b) {
		return a.toInt() < b.toInt();
	});
	QList<bool> areFriendsList;
	// Iterate over the keys 
	for (const QString& key : sortedKeys) {
		bool areFriends = requests[key].toBool();

		// Add them to the list
		areFriendsList.append(areFriends);
	}

	// Get all the users that have received a friend request from you or sent a friend request to you
	requests = responseObject["sent_friend_requests"].toObject();

	// Sort the keys or else the bools dont match up with the user
	sortedKeys = requests.keys();
	std::sort(sortedKeys.begin(), sortedKeys.end(), [](const QString& a, const QString& b) {
		return a.toInt() < b.toInt();
		});

	// Prepare lists to store the 'sent' and 'received' statuses
	QList<bool> sentFriendRequests;
	QList<bool> receivedFriendRequests;
	// Iterate over the keys in "sent_friend_requests"
	for (const QString& key : sortedKeys) {
		QJsonObject userStatus = requests[key].toObject();

		// Extract the 'sent' and 'received' values for each user
		bool sent = userStatus["sent"].toBool();
		bool received = userStatus["received"].toBool();

		// Add them to the respective lists
		sentFriendRequests.append(sent);
		receivedFriendRequests.append(received);
	}

	reply->deleteLater();

	// Send a signal to the controller with all the necessary data of the users on the page
	emit usersFetched(responseObject["total_count"].toInt(), userList, areFriendsList, sentFriendRequests, receivedFriendRequests);
}

/*
* Get the ID of a user given a username
* @param the username you're trying to get the ID from
* @return the ID of the user, -1 means that no ID has been found
*/
int NetworkManager::getUserIdByUsername(const QString username) {
	// Build the URL
	QNetworkRequest request(QUrl("http://localhost:80/user/get_user_id"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	// Post data
	QJsonObject json = QJsonObject();
	json["username"] = username;
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

/*
* Send a certain user a friend request
* @param userToAdd the user you're sending the friend request to
* @post the request will be placed in the inbox of the user
*/
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

/*
* Get all the inbox messages of a certain user
* @post loaded all the inbox messages of the user
*/
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
			for (const QJsonValue& value : jsonArray) {
				QJsonObject obj = value.toObject();
				inboxItems.append(obj); // Add each object to the list
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

/*
* Remove a certain inbox item from the users inbox
* @param inboxID the ID of the inbox you want to delete
* @post the item is deleted from the database
*/
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

/*
* Add a friend (from the inbox accept a friend request)
* @param sendingUserID this is the ID of the user that sent the friend request
* @post you're now friends with the user, this information gets added to the database
*/
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

/*
* Accept a question set
* @param code this is the code of a certain question set
* @post get the question set associated with the code
* @emit questionSetFailed() the acquisition or unzipping of the question set failed
* @emit questionSetSucces() the question set is 'downloaded'
*/
void NetworkManager::acceptQuestionSet(QString code) {
	// Build URL
	QNetworkRequest request(QUrl("http://localhost:80/question_set/get_question_set"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	// Create the object
	QJsonObject json = QJsonObject();
	json["code"] = code;

	// Send the object to the backend
	QByteArray data = QJsonDocument(json).toJson();
	QNetworkReply* reply = m_networkManager->post(request, data);

	connect(reply, &QNetworkReply::finished, this, [this, reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Network error:" << reply->errorString();
			QByteArray responseData = reply->readAll();
			qDebug() << "Response data:" << responseData;
			reply->deleteLater();
			emit questionSetFailed();
			return;
			//TODO: username already token
		}

		// Read the response
		QByteArray responseData = reply->readAll();

		// Try to parse the response as JSON
		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &jsonError);

		if (jsonError.error == QJsonParseError::NoError && jsonDoc.isObject()) {
			QJsonObject jsonObj = jsonDoc.object();

			// Check if `question_set` is null
			if (jsonObj.contains("question_set") && jsonObj["question_set"].isNull()) {
				qDebug() << "No question set found for the provided code.";
				emit questionSetFailed();
			}
		}
		else {
			FileManager fileManager = FileManager();
			try {
				fileManager.unzip(responseData);
			}
			catch (const unzipException& e) {
				qWarning() << e.what();
				emit questionSetFailed();
			}
			catch (const std::exception& e) {
				qWarning() << e.what();
				emit questionSetFailed();
			}
		
			emit questionSetSucces();
		}

		reply->deleteLater();
	});
}

/*
* Get the usernames of the users friends
* @emit friendUsernamesFetched() signal that all friends usernames are found, and what the usernames are
*/
void NetworkManager::getFriendUsernames() {
	// Get the user ID of the user accepting the friend request
	int userID = getUserId();
	if (userID == -1) {
		return;
		// TODO error handling
	}

	// Build URL
	QNetworkRequest request(QUrl("http://localhost:80/friend/get_friend_usernames"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Token", getSessionCookie().toUtf8());

	// Create the object
	QJsonObject json = QJsonObject();
	json["id"] = userID;

	// Send the object to the backend
	QByteArray data = QJsonDocument(json).toJson();
	QNetworkReply* reply = m_networkManager->post(request, data);

	connect(reply, &QNetworkReply::finished, this, [this, reply]() {
		// Check for network errors
		if (reply->error() != QNetworkReply::NoError) {
			qDebug() << "Network error:" << reply->errorString();
			QByteArray responseData = reply->readAll();
			qDebug() << "Response data:" << responseData;
			reply->deleteLater();
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
		QJsonObject responseObject = doc.object();
		QJsonArray userIDArray = responseObject["IDs"].toArray();
		QJsonArray usernameArray = responseObject["friend_usernames"].toArray();
		QList<int> userIDList;
		QList<QString> usernameList;
		for (const QJsonValue& value : userIDArray) {
			userIDList.append(value.toInt());
		}
		for (const QJsonValue& value : usernameArray) {
			usernameList.append(value.toString());
		}

		emit friendUsernamesFetched(userIDList, usernameList);

		reply->deleteLater();
	});
}