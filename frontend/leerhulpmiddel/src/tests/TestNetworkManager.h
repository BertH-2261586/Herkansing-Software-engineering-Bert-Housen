#pragma once

#include <qapplication.h>

#include <QtTest/QtTest>

class TestNetworkManager : public QObject {
	Q_OBJECT
private slots:
    // Test login functionality
    void testRegisterUserFailed();
    void testRegisterUserSuccess();
    void testLogout();
    void testLoginFailed();
    void testLoginSuccess();
    void testGetLoggedInStatus();

    void testShareQuestionSetsSuccess();
    void testShareQuestionSetsFailed();

    void testShareQuestionSetsWithFriendsSuccess();
    void testGetUsersByPage();
};
