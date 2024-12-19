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

	LoginController* m_loginController;
	QLineEdit* m_usernameEdit;
	QLineEdit* m_passwordEdit;
	QLineEdit* m_passwordConfirmEdit;
	QLabel* m_usernameErrorLabel;
	QLabel* m_passwordErrorLabel;
	QLabel* m_passwordConfirmErrorLabel;
	QLabel* m_titleLabel;
	QLabel* m_serverFeedback;
	QPushButton* m_loginBtn;
	QPushButton* m_registerBtn;
	QPushButton* m_goBackToLoginBtn;

private slots:
	void changeToLogin();
	void changeToRegister();
	void onLoginOrRegisterClicked();
public slots:
	void failedLoginFeedback();
	void failedRegisterFeedback();
};

