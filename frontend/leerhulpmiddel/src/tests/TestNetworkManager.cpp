#include "TestNetworkManager.h"


#include "../model/manager/NetworkManager.h"

void TestNetworkManager::testLoginSuccess() {
	NetworkManager manager;
	QSignalSpy loginSuccess = QSignalSpy(&manager, &NetworkManager::loginSuccess);		

	manager.login("TestUser", "12345678");

	QVERIFY(loginSuccess.wait(3000));		//Verifies if logged in signal received
}

void TestNetworkManager::testLoginFailed() {
	NetworkManager manager;
	QSignalSpy loginFailed = QSignalSpy(&manager, &NetworkManager::loginFailed);

	manager.login("", "");

	QVERIFY(loginFailed.wait(3000));		//Verifies if logged in signal received
}

