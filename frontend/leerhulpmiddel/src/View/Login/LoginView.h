#pragma once
#include "../../Controller/LoginController.h"
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class LoginView : public QWidget
{
	Q_OBJECT
public:
	LoginView(LoginController* loginController,QWidget* parent = nullptr);
private:
	void setupValidators();
	void hideErrorLabels();
	LoginController* loginController;
	QLineEdit* usernameEdit;
	QLineEdit* passwordEdit;
	QLineEdit* passwordConfirmEdit;
	QLabel* usernameErrorLabel;
	QLabel* passwordErrorLabel;
	QLabel* passwordConfirmErrorLabel;
	QLabel* titleLabel;
	QLabel* serverFeedback;
	QPushButton* loginBtn;
	QPushButton* registerBtn;
	QPushButton* goBackToLoginBtn;
private slots:
	void changeToLogin();
	void changeToRegister();
	void onLoginClicked();
public slots:
	void failedLoginFeedback();
	void failedRegisterFeedback();
};

