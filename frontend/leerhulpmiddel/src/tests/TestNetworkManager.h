#pragma once

#include <qapplication.h>

#include <QtTest/QtTest>

class TestNetworkManager : public QObject {
	Q_OBJECT
private slots:
	void testLoginSuccess();
	void testLoginFailed();
};