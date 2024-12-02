#include "LoginView.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>


LoginView::LoginView(LoginController* loginController,QWidget* parent) : loginController{ loginController }, QWidget{ parent } {
	
    titleLabel = new QLabel("Login to Your Account", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);


    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");
    usernameEdit->setMaxLength(50);


    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMaxLength(255);


    //Hidden Elements

    passwordConfirmEdit = new QLineEdit(this);
    passwordConfirmEdit->setPlaceholderText("Confirm Password");
    passwordConfirmEdit->setEchoMode(QLineEdit::Password);
    passwordConfirmEdit->setMaxLength(255);
    passwordConfirmEdit->hide();
   

    usernameErrorLabel = new QLabel(this);
    usernameErrorLabel->setStyleSheet("color: red;");
    usernameErrorLabel->hide();

    passwordErrorLabel = new QLabel(this);
    passwordErrorLabel->setStyleSheet("color: red;");
    passwordErrorLabel->hide();

    passwordConfirmErrorLabel = new QLabel(this);
    passwordConfirmErrorLabel->setStyleSheet("color: red;");
    passwordConfirmErrorLabel->hide();

    serverFeedback = new QLabel(this);
    serverFeedback->setStyleSheet("color: red;");
    serverFeedback->hide();

    loginBtn = new QPushButton("Login", this);
	registerBtn = new QPushButton("Register", this);
    goBackToLoginBtn = new QPushButton("Back to Login", this);
    goBackToLoginBtn->hide();

    // Create layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(serverFeedback);
    layout->addWidget(usernameEdit);
    layout->addWidget(usernameErrorLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(passwordErrorLabel);
    layout->addWidget(passwordConfirmEdit);
    layout->addWidget(passwordConfirmErrorLabel);

    layout->addWidget(loginBtn);
    layout->addWidget(registerBtn);
    layout->addWidget(goBackToLoginBtn);




    setLayout(layout);


    connect(loginBtn, &QPushButton::clicked, this, &LoginView::onLoginClicked);
    connect(registerBtn, &QPushButton::clicked, this, &LoginView::changeToRegister);
    connect(goBackToLoginBtn, &QPushButton::clicked, this, &LoginView::changeToLogin);
}

//sets up the validators for the username and password fields for registration's requirements
void LoginView::setupValidators()
{
    QRegularExpression usernameRegex("^[a-zA-Z0-9]{3,50}$");
    QRegularExpressionValidator* usernameValidator = new QRegularExpressionValidator(usernameRegex, this);
    usernameEdit->setValidator(usernameValidator);

    QRegularExpression passwordRegex("^[\\x20-\\x7E]{8,255}$");
    QRegularExpressionValidator* passwordValidator = new QRegularExpressionValidator(passwordRegex, this);
    passwordEdit->setValidator(passwordValidator);
}

//hides all error labels
void LoginView::hideErrorLabels()
{
    usernameEdit->setStyleSheet("");
    passwordEdit->setStyleSheet("");
    passwordConfirmEdit->setStyleSheet("");
    passwordErrorLabel->hide();
    usernameErrorLabel->hide();
    passwordConfirmErrorLabel->hide();
    serverFeedback->hide();
}



void LoginView::changeToLogin() {

    hideErrorLabels();
	usernameEdit->clear();
	passwordEdit->clear();
	passwordConfirmEdit->clear();

	loginBtn->setText("Login");
	registerBtn->show();
	goBackToLoginBtn->hide();
	passwordEdit->setToolTip("Password");
	passwordConfirmEdit->hide();
	usernameEdit->setValidator(nullptr);
    passwordEdit->setValidator(nullptr);
}



void LoginView::changeToRegister() {
    hideErrorLabels();
    usernameEdit->clear();
    passwordEdit->clear();
    passwordConfirmEdit->clear();

    loginBtn->setText("Register");
    registerBtn->hide();
    goBackToLoginBtn->show();
    passwordEdit->setToolTip("Password must be at least 8 characters.");
    passwordConfirmEdit->show();
    setupValidators();
}




void LoginView::onLoginClicked() {
	QString username = usernameEdit->text();
	QString password = passwordEdit->text();
    bool isValid = true;

    hideErrorLabels();

    if (username.isEmpty() ){
        usernameEdit->setStyleSheet("border: 1px solid red;");
        usernameErrorLabel->show();
        usernameErrorLabel->setText("Username is required.");
        isValid = false;
    }

    if (password.isEmpty()) {
		passwordEdit->setStyleSheet("border: 1px solid red;");
		passwordErrorLabel->show();
		passwordErrorLabel->setText("Password is required.");
		isValid = false;
	}

    if (passwordConfirmEdit->isVisible() && password != passwordConfirmEdit->text()) {
		passwordConfirmEdit->setStyleSheet("border: 1px solid red;");
		passwordConfirmErrorLabel->show();
		passwordConfirmErrorLabel->setText("Passwords do not match.");
		isValid = false;
	}

    if (!usernameEdit->hasAcceptableInput()) {
        usernameEdit->setStyleSheet("border: 1px solid red;");
        usernameErrorLabel->show();
        usernameErrorLabel->setText("Invalid username. It must be at least 3 alphanumeric characters.");
        isValid = false;
    }

    if (!passwordEdit->hasAcceptableInput()) {
        passwordEdit->setStyleSheet("border: 1px solid red;");
        passwordErrorLabel->show();
        passwordErrorLabel->setText("Invalid password. It must be at least 8 characters and contain valid characters.");
        isValid = false;
    }



    if (isValid)
    {
        if (passwordConfirmEdit->isVisible()) //if the passwordConfirmEdit is visible, then we are registering
        {
            loginController->registerUser(username, password);
        }
        else {
            loginController->login(username, password);
        }
    }
}

void LoginView::failedLoginFeedback() {
    serverFeedback->setText("Failed To Log in!");
    serverFeedback->show();
}

void LoginView::failedRegisterFeedback() {
    serverFeedback->setText("Username already taken!");
    serverFeedback->show();
}