#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QGridLayout>
#include <QHBoxLayout>
#include <QWidget>

#include "../../Controller/LoginController.h"
#include "../../model/manager/NetworkManager.h"

class LeerhulpmiddelMainWindow;

#include "questionsetbrowser.h"
#include "../../Controller/questionmanagercontroller.h"
#include "inboxView.h"

class HomeScreen: public QWidget
{
    Q_OBJECT
public:
    HomeScreen(QuestionManagerController* questionManagerController, LeerhulpmiddelMainWindow* parent);
    ~HomeScreen();

private:
    QWidget* GenerateTopButtonBar();

    void setInboxView();
    void setAddFriendButton(QHBoxLayout* container);
    void setInboxButton(QHBoxLayout* container);
    QWidget* setInboxRequestLabel();
    void setIconButton(QPushButton* button, QString iconName);

    void setInboxRequestAmount();

    QuestionsetBrowser* m_vragensetBrowser;
    QWidget* m_rightSideScreen;
    QHBoxLayout* m_container;

    LeerhulpmiddelMainWindow* m_mainWindow;
    QPushButton* m_addFriendButton;
    QPushButton* m_inboxButton;

    InboxView* m_inboxView;
    QLabel* m_requestAmountLabel;
    QPropertyAnimation* m_inboxAnimation;

    NetworkManager* m_networkManager;
    LoginController* m_loginController;
public slots:
    void DisplayWidget(QWidget* displayWidget);
    void setRequestAmount() { setInboxRequestAmount(); }

private slots:
    void startInboxAnimation();
};

#endif // HOMESCREEN_H
