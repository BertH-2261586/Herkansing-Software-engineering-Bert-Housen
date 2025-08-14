#include "TestNetworkManager.h"
#include "../model/manager/NetworkManager.h"

void TestNetworkManager::testLoginSuccess() {
	NetworkManager manager;
	QSignalSpy loginSuccess = QSignalSpy(&manager, &NetworkManager::loginSuccess);		

	manager.login("thibe", "wachtwoord");

	QVERIFY(loginSuccess.wait(3000));		//Verifies if logged in succes signal received
}

void TestNetworkManager::testLoginFailed() {
	NetworkManager manager;
	QSignalSpy loginFailed = QSignalSpy(&manager, &NetworkManager::loginFailed);

	manager.login("", "");

	QVERIFY(loginFailed.wait(3000));		//Verifies if logged in failed signal received
}

void TestNetworkManager::testLogout() {
	NetworkManager manager;
	QSignalSpy logoutSpy(&manager, &NetworkManager::loggedOut);

	manager.logout();

	QVERIFY(logoutSpy.count() == 1);					// Verify the logout signal was emitted
	QCOMPARE(manager.getSessionCookie(), QString());	// Verify session cookie is cleared
}

void TestNetworkManager::testRegisterUserFailed() {
	NetworkManager manager;
	QSignalSpy failedSpy(&manager, &NetworkManager::registerFailed);

    manager.registerUser("", "", false); // Invalid credentials

	QVERIFY(failedSpy.wait(3000)); // Verifies if registerFailed signal is received
}

void TestNetworkManager::testRegisterUserSuccess() {
	NetworkManager manager;
	QSignalSpy successSpy(&manager, &NetworkManager::loginSuccess);

	// Generate a random name so that you don't create an error for an existing user
	QString randomName;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	for (int i = 0; i < 10; ++i) {
		char randomChar = 'a' + (std::rand() % 26); 
		randomName += randomChar;
	}

    manager.registerUser(randomName, "password", false);

	QVERIFY(successSpy.wait(3000));						// Verifies if loginSuccess signal is received
}

void TestNetworkManager::testGetLoggedInStatus() {
	NetworkManager manager;
	QSignalSpy loggedInSpy(&manager, &NetworkManager::loggedIn);
	QSignalSpy loggedOutSpy(&manager, &NetworkManager::loggedOut);

	manager.setLoginStatus(true);
	manager.getLoggedInStatus();

	QVERIFY(loggedInSpy.count() == 1);			// Verify loggedIn signal was emitted
	QVERIFY(loggedOutSpy.isEmpty());			// Ensure loggedOut signal was not emitted

	manager.setLoginStatus(false);
	manager.getLoggedInStatus();

	QVERIFY(loggedOutSpy.count() == 1);			// Verify loggedOut signal was emitted
}

// This test is dependent on the fact that the question sets exist
void TestNetworkManager::testShareQuestionSetsSuccess() {
	NetworkManager manager;
	QSignalSpy successSpy(&manager, &NetworkManager::shareSuccess);

	QList<QString> paths = { "/questionSet15", "/real questions"};
	manager.shareQuestionSets(paths);

	QVERIFY(successSpy.wait(3000)); // Verifies if shareSuccess signal is received
}

void TestNetworkManager::testShareQuestionSetsFailed() {
	NetworkManager manager;
	QSignalSpy failedSpy(&manager, &NetworkManager::shareFailed);

	QList<QString> paths = {"/bogus_path"}; // Empty list simulates failure
	manager.shareQuestionSets(paths);

	QVERIFY(failedSpy.wait(3000)); // Verifies if shareFailed signal is received
}

void TestNetworkManager::testShareQuestionSetsWithFriendsSuccess() {
	NetworkManager manager;
	QSignalSpy successSpy(&manager, &NetworkManager::shareQuestionSetsSuccess);

	QList<int> friendIds = { 1, 2, 3 };
	QString code = "set123";
	manager.shareQuestionSetsWithFriends(friendIds, code);

	QVERIFY(successSpy.wait(3000)); // Verifies if shareQuestionSetsSuccess signal is received
}

void TestNetworkManager::testGetUsersByPage() {
	NetworkManager manager;
	QSignalSpy fetchedSpy(&manager, &NetworkManager::usersFetched);

	manager.getUsersByPage(1, "");

	QVERIFY(fetchedSpy.wait(3000)); // Verifies if usersFetched signal is received
	QVERIFY(!fetchedSpy.isEmpty()); // Ensure some data was received
}
