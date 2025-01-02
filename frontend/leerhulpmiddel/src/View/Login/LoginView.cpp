#include "LoginView.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>


LoginView::LoginView(LoginController* loginController,QWidget* parent) : m_loginController{ loginController }, QWidget{ parent } {
	
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    QPushButton* goBackbtn = new QPushButton( this);
    goBackbtn->setText("Go Back");
    goBackbtn->setIcon(QIcon::fromTheme("go-home"));
    goBackbtn->setToolTip("Go Back");
    goBackbtn->setIconSize(QSize(30, 30));
    goBackbtn->setCursor(Qt::PointingHandCursor);
    goBackbtn->setStyleSheet("border: none;");

    m_titleLabel = new QLabel("Login to Your Account", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);


    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("Username");
    m_usernameEdit->setMaxLength(50);


    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setMaxLength(255);


    //Hidden Elements

    m_passwordConfirmEdit = new QLineEdit(this);
    m_passwordConfirmEdit->setPlaceholderText("Confirm Password");
    m_passwordConfirmEdit->setEchoMode(QLineEdit::Password);
    m_passwordConfirmEdit->setMaxLength(255);
    m_passwordConfirmEdit->hide();
   

    m_usernameErrorLabel = new QLabel(this);
    m_usernameErrorLabel->setStyleSheet("color: red;");
    m_usernameErrorLabel->hide();

    m_passwordErrorLabel = new QLabel(this);
    m_passwordErrorLabel->setStyleSheet("color: red;");
    m_passwordErrorLabel->hide();

    m_passwordConfirmErrorLabel = new QLabel(this);
    m_passwordConfirmErrorLabel->setStyleSheet("color: red;");
    m_passwordConfirmErrorLabel->hide();

    m_serverFeedback = new QLabel(this);
    m_serverFeedback->setStyleSheet("color: red;");
    m_serverFeedback->hide();

    m_loginBtn = new QPushButton("Login", this);

    m_registerBtn = new QPushButton("Register", this);
    m_goBackToLoginBtn = new QPushButton("Back to Login", this);
    m_goBackToLoginBtn->hide();

    // Create layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(goBackbtn,0, Qt::AlignLeft);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_serverFeedback);
    layout->addWidget(m_usernameEdit);
    layout->addWidget(m_usernameErrorLabel);
    layout->addWidget(m_passwordEdit);
    layout->addWidget(m_passwordErrorLabel);
    layout->addWidget(m_passwordConfirmEdit);
    layout->addWidget(m_passwordConfirmErrorLabel);
    layout->addWidget(m_loginBtn);
    layout->addWidget(m_registerBtn);
    layout->addWidget(m_goBackToLoginBtn);

    setLayout(layout);



    connect(goBackbtn, &QPushButton::clicked, this, [=] {this->close(); });
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginView::onLoginOrRegisterClicked);
    connect(m_registerBtn, &QPushButton::clicked, this, &LoginView::changeToRegister);
    connect(m_goBackToLoginBtn, &QPushButton::clicked, this, &LoginView::changeToLogin);
}

/**
*sets up the validators for the username and password fields for registration's requirements
*/
void LoginView::setupValidators()
{
    QRegularExpression usernameRegex("^[a-zA-Z0-9]{3,50}$");
    QRegularExpressionValidator* usernameValidator = new QRegularExpressionValidator(usernameRegex, this);
    m_usernameEdit->setValidator(usernameValidator);

    QRegularExpression passwordRegex("^[\\x20-\\x7E]{8,255}$");
    QRegularExpressionValidator* passwordValidator = new QRegularExpressionValidator(passwordRegex, this);
    m_passwordEdit->setValidator(passwordValidator);
}

/**
*hides all error labels
*/
void LoginView::hideErrorLabels()
{
    m_usernameEdit->setStyleSheet("");
    m_passwordEdit->setStyleSheet("");
    m_passwordConfirmEdit->setStyleSheet("");
    m_passwordErrorLabel->hide();
    m_usernameErrorLabel->hide();
    m_passwordConfirmErrorLabel->hide();
    m_serverFeedback->hide();
}



/**
* Changes the view to the login view fields
*/
void LoginView::changeToLogin() {

    hideErrorLabels();
	m_usernameEdit->clear();
	m_passwordEdit->clear();
	m_passwordConfirmEdit->clear();

	m_loginBtn->setText("Login");
	m_registerBtn->show();
	m_goBackToLoginBtn->hide();
	m_passwordEdit->setToolTip("Password");
	m_passwordConfirmEdit->hide();
	m_usernameEdit->setValidator(nullptr);
    m_passwordEdit->setValidator(nullptr);
}


/**
* Changes the view to the register view fields
*/
void LoginView::changeToRegister() {
    hideErrorLabels();
    m_usernameEdit->clear();
    m_passwordEdit->clear();
    m_passwordConfirmEdit->clear();

    m_loginBtn->setText("Register");
    m_registerBtn->hide();
    m_goBackToLoginBtn->show();
    m_passwordEdit->setToolTip("Password must be at least 8 characters.");
    m_passwordConfirmEdit->show();
    setupValidators();
}




/**
*  Event when the login button or Register button is clicked depending on if the user is registering or logging in
*/
void LoginView::onLoginOrRegisterClicked() {
	QString username = m_usernameEdit->text();
	QString password = m_passwordEdit->text();
    bool isValid = true;

    hideErrorLabels();

    if (username.isEmpty() ){
        m_usernameEdit->setStyleSheet("border: 1px solid red;");
        m_usernameErrorLabel->show();
        m_usernameErrorLabel->setText("Username is required.");
        isValid = false;
    }

    if (password.isEmpty()) {
		m_passwordEdit->setStyleSheet("border: 1px solid red;");
		m_passwordErrorLabel->show();
		m_passwordErrorLabel->setText("Password is required.");
		isValid = false;
	}

    if (m_passwordConfirmEdit->isVisible() && password != m_passwordConfirmEdit->text()) {
		m_passwordConfirmEdit->setStyleSheet("border: 1px solid red;");
		m_passwordConfirmErrorLabel->show();
		m_passwordConfirmErrorLabel->setText("Passwords do not match.");
		isValid = false;
	}

    if (!m_usernameEdit->hasAcceptableInput()) {
        m_usernameEdit->setStyleSheet("border: 1px solid red;");
        m_usernameErrorLabel->show();
        m_usernameErrorLabel->setText("Invalid username. It must be at least 3 alphanumeric characters.");
        isValid = false;
    }

    if (!m_passwordEdit->hasAcceptableInput()) {
        m_passwordEdit->setStyleSheet("border: 1px solid red;");
        m_passwordErrorLabel->show();
        m_passwordErrorLabel->setText("Invalid password. It must be at least 8 characters and contain valid characters.");
        isValid = false;
    }

    if (isValid)
    {
        if (m_passwordConfirmEdit->isVisible()) //if the m_passwordConfirmEdit is visible, then we are registering
        {
            m_loginController->registerUser(username, password);
        }
        else {
            m_loginController->login(username, password);
        }
    }
}


/**
* Shows the feedback that the login failed from the server
*/
void LoginView::failedLoginFeedback() {
    m_serverFeedback->setText("Failed To Log in!");
    m_serverFeedback->show();
}

/**
* Shows the feedback that the registration failed from the server
*/
void LoginView::failedRegisterFeedback() {
    m_serverFeedback->setText("Username already taken!");
    m_serverFeedback->show();
}